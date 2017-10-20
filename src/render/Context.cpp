#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <easylogging++.h>

#include "dukdemo/render/util.h"
#include "dukdemo/render/Context.h"


namespace dukdemo {
namespace render {


Context::Context(
	char const* const pWindowName,
	RenderFn const& onRender,
	Uint32 windowX,
	Uint32 windowY,
	Uint32 windowWidth,
	Uint32 windowHeight,
	Uint32 windowType
)
	:	m_onRender{onRender}
	,	m_pWindow{SDL_CreateWindow(
			pWindowName,
			windowX, windowY,
			windowWidth, windowHeight,
			windowType
		)}
	,	m_pGLContext{nullptr}
{
	if (!m_pWindow)
	{
		throw std::runtime_error{"Failed to initialise window"};
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE
	);
	m_pGLContext.reset(SDL_GL_CreateContext(m_pWindow.get()));

	glewExperimental = GL_TRUE;
	GLenum const glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		char const* const pError = reinterpret_cast<char const*>(
			glewGetErrorString(glewError)
		);
		throw std::runtime_error{pError};
	}

	bool const setSwapIntervalSucceeded = SDL_GL_SetSwapInterval(1) < 0;
	LOG_IF(setSwapIntervalSucceeded, INFO) << "No VSync: " << SDL_GetError();
	LOG_IF(not setSwapIntervalSucceeded, INFO) << "VSync enabled";

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Context::~Context() noexcept
{
}


void
Context::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_onRender();
	SDL_GL_SwapWindow(m_pWindow.get());
}


} // namespace render
} // namespace dukdemo
