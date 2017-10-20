#include <memory>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include <easylogging++.h>

#include "b2draw/DebugDraw.h"

#include "dukdemo/render/Context.h"
#include "dukdemo/render/util.h"
#include "dukdemo/util/deleters.h"


constexpr int screenWidth{640};
constexpr int screenHeight{480};


constexpr float worldTimeStep{1.0f / 60.0f};
constexpr unsigned velocityIterations{8};
constexpr unsigned positionIterations{3};


constexpr char const* const pPositionAttribName = "position";
constexpr char const* const pColourAttribName = "colour";


INITIALIZE_EASYLOGGINGPP


using duk_context_ptr =
	std::unique_ptr<duk_context, dukdemo::util::DukContextDeleter>;

using sdl_window_ptr =
	std::unique_ptr<SDL_Window, dukdemo::util::SDLWindowDeleter>;

using gl_context_ptr = std::unique_ptr<void, dukdemo::util::GLContextDeleter>;


void run(int argc, char const* const argv[])
{
	dukdemo::render::Context renderContext{"Dukdemo"};
	dukdemo::render::checkGLErrors("Render context created");
	auto const programID = dukdemo::render::createProgram();

	// Set up scene for rendering.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	b2draw::DebugDraw debugDraw{
		programID,
		pPositionAttribName,
		pColourAttribName
	};
	debugDraw.SetFlags(0xff);

	b2Vec2 const gravity{0.0f, -9.8f};
	b2World world{gravity};
	world.SetDebugDraw(&debugDraw);

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0.0f, -4.0f);
		auto const pGroundBody = world.CreateBody(&bodyDef);

		b2PolygonShape box;
		box.SetAsBox(-30.0f, 1.0f);
		pGroundBody->CreateFixture(&box, 0.0f /* Density. */);
	}

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 4.0f);
		auto const pDynamicBody = world.CreateBody(&bodyDef);

		b2PolygonShape box;
		box.SetAsBox(1.0f, 1.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		pDynamicBody->CreateFixture(&fixtureDef);
	}


	constexpr float fieldOfView{45.0f};
	auto const projMat = glm::perspective(
		fieldOfView,
		4.0f / 3.0f,
		0.1f,
		100.0f
	);

	constexpr glm::vec3 eye{0.0f, 0.0f, 40.0f};
	constexpr glm::vec3 focus{0.0f, 0.0f, 0.0f};
	constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};
	auto const viewMat = glm::lookAt(eye, focus, up);

	glm::mat4 const modelMat{1.0f};

	auto const mvpMat{projMat * viewMat * modelMat};
	auto const pMvpMatStart{&mvpMat[0][0]};
	auto const mvpAttribLoc{glGetUniformLocation(programID, "MVP")};
	if (mvpAttribLoc < 0)
	{
		throw std::runtime_error{"Unable to locate uniform 'MVP'"};
	}


	auto const update = [&debugDraw, &world] {
		world.Step(worldTimeStep, velocityIterations, positionIterations);
		debugDraw.Clear();
		world.DrawDebugData();
		debugDraw.BufferData();
	};

	auto const render = [
		&debugDraw,
		programID,
		pSDLWindow = renderContext.window(),
		mvpAttribLoc,
		pMvpMatStart
	] {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(mvpAttribLoc, 1, GL_FALSE, pMvpMatStart);
		debugDraw.Render();
		SDL_GL_SwapWindow(pSDLWindow);
	};

	// Ensure no GL errors.
	dukdemo::render::checkGLErrors("Ready to render");

	update();
	render();

	SDL_Event event;
	bool userQuit{false};

	while (not userQuit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
					userQuit = true;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							userQuit = true;
							break;

						case SDLK_s:
							update();
							render();
							break;

						default:
							break;
					}
					break;

				default:
					break;
			}
		}
	}

	renderContext.reset();
}


int main(int argc, char const* const argv[])
{
	try
	{
		run(argc, argv);
	}
	catch (std::exception const& err)
	{
		LOG(ERROR) << err.what();
		return 1;
	}
	return 0;
}
