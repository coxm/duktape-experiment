#ifndef DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
#define DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
#include <SDL2/SDL.h>
#include <duk_config.h>


struct SDL_Window;


namespace dukdemo {
namespace util {


struct DukContextDeleter
{
	void operator()(duk_context* pContext) noexcept;
};


struct SDLWindowDeleter
{
	void operator()(SDL_Window* pWindow) noexcept;
};


struct GLContextDeleter
{
	void operator()(SDL_GLContext pGLContext) noexcept;
};


} // namespace util
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
