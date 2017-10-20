cmake_minimum_required (VERSION 3.5)
set (CMAKE_DISABLE_SOURCE_CHANGES ON)
set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)


set (PROJECT_NAME dukdemo)
project (${PROJECT_NAME})


find_library(LIB_SDL SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


include(lib/duktape.cmake)
include(lib/easyloggingpp.cmake)


set(
	DEP_LIBS
	${DUKTAPE_LIBRARY}
	${EASYLOGGINGPP_LIBRARY}
	${LIB_SDL}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)
set(ALL_LIBS ${DEP_LIBS})


file (
	GLOB
	CPP_SOURCES
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/render/*.cpp"
)
add_executable(${PROJECT_NAME} ${CPP_SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_SOURCE_DIR}/include"
	"${DUKTAPE_INCLUDE_DIRS}"
	"${EASYLOGGINGPP_INCLUDE_DIRS}"
)


# Compiler flags.
set(CXX_FLAGS_BASE -Werror -std=c++1z -Wall -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wno-int-conversions -Wsign-promo -Wno-pmf-conversions)
set(CXX_FLAGS_GNU ${CXX_FLAGS_BASE})
set(CXX_FLAGS_CLANG ${CXX_FLAGS_BASE})

set (CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 ${EXTRA_CXX_FLAGS}")


if(${CMAKE_COMPILER_IS_GNUCXX})
	set(EXTRA_CXX_FLAGS ${EXTRA_CXX_FLAGS} ${CXX_FLAGS_GNU})
else()
	set(EXTRA_CXX_FLAGS ${EXTRA_CXX_FLAGS} ${CXX_FLAGS_CLANG})
endif()
target_compile_options(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}
)
