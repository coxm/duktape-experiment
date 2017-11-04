#include <SDL2/SDL_video.h>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

#include <duktape.h>

#include "dukdemo/util/deleters.h"


namespace dukdemo {
namespace util {


void
DukContextDeleter::operator()(duk_context* pContext)
	noexcept
{
	duk_destroy_heap(pContext);
}


void
SDLWindowDeleter::operator()(SDL_Window* pWindow)
	noexcept
{
	SDL_DestroyWindow(pWindow);
}


void
GLContextDeleter::operator()(SDL_GLContext pGLContext)
	noexcept
{
	SDL_GL_DeleteContext(pGLContext);
}


void
B2Deleter::operator()(b2Body* pBody)
	noexcept
{
	auto* const pWorld = pBody->GetWorld();
	if (pWorld)
	{
		pWorld->DestroyBody(pBody);
	}
}


} // namespace util
} // namespace dukdemo
