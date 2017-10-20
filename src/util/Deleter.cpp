#include <SDL2/SDL_video.h>
#include <duktape.h>


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
	SDL_GL_DeleteContext(pContext);
}


} // namespace util
} // namespace dukdemo
