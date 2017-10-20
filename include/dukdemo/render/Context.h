#ifndef DUKDEMO_INCLUDE__DUKDEMO__RENDER__CONTEXT__H
#define DUKDEMO_INCLUDE__DUKDEMO__RENDER__CONTEXT__H
#include <type_traits>
#include <memory>
#include <functional>

#include <SDL2/SDL_video.h>


namespace dukdemo {
namespace render {


using GLContext = std::remove_pointer<SDL_GLContext>::type;


struct Deleter
{
	inline void
	operator()(SDL_Window* pWindow) noexcept
	{
		SDL_DestroyWindow(pWindow);
	}

	inline void
	operator()(SDL_GLContext pContext) noexcept
	{
		SDL_GL_DeleteContext(pContext);
	}
};


class Context
{
public:
	constexpr static Uint32 s_defaultWindowX = 0;
	constexpr static Uint32 s_defaultWindowY = 0;
	constexpr static Uint32 s_defaultWindowWidth = 640;
	constexpr static Uint32 s_defaultWindowHeight = 480;
	constexpr static Uint32 s_defaultWindowType =
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	using RenderFn = std::function<void()>;

	Context(
		char const* const pWindowName,
		RenderFn const& onRender = nullptr,
		Uint32 windowX = s_defaultWindowX,
		Uint32 windowY = s_defaultWindowY,
		Uint32 windowWidth = s_defaultWindowWidth,
		Uint32 windowHeight = s_defaultWindowHeight,
		Uint32 windowType = s_defaultWindowType
	);

	Context(Context const&) = delete;
	Context(Context&&) noexcept;
	Context& operator=(Context const&) = delete;
	Context& operator=(Context&&) noexcept;
	~Context() noexcept;

	inline void setOnRender(RenderFn const& onRender)
	{
		m_onRender = onRender;
	}

	void render();

	inline SDL_Window* window() noexcept
	{ return m_pWindow.get(); }
	inline SDL_Window const* window() const noexcept
	{ return m_pWindow.get(); }

	inline SDL_GLContext glContext() noexcept
	{ return m_pGLContext.get(); }
	inline GLContext const* glContext() const noexcept
	{ return m_pGLContext.get(); }

	inline void reset() noexcept
	{
		m_pGLContext.reset();
		m_pWindow.reset();
	}

private:
	RenderFn m_onRender;

	std::unique_ptr<SDL_Window, Deleter> m_pWindow;
	std::unique_ptr<GLContext, Deleter> m_pGLContext;
};


} // namespace render
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__RENDER__CONTEXT__H
