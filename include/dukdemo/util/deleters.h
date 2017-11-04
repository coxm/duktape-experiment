#ifndef DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
#define DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
#include <duk_config.h>


struct SDL_Window;
class b2Body;


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
	void operator()(void* pGLContext) noexcept;
};


struct B2Deleter
{
	void operator()(b2Body* pBody) noexcept;
};


} // namespace util
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__UTIL__DELETERS__H
