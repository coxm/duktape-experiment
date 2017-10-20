cmake_minimum_required (VERSION 3.5)
set (CMAKE_DISABLE_SOURCE_CHANGES ON)
set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)


set (PROJECT_NAME dukdemo)
project (${PROJECT_NAME} LANGUAGES CXX C VERSION 1.0.0)


# Compiler flags.
list(APPEND MAIN_CXX_FLAGS
	-Werror -std=c++1z -Wall -Wold-style-cast -Woverloaded-virtual
	-Wno-int-conversions -Wsign-promo -Wno-pmf-conversions
)
if(CMAKE_COMPILER_IS_GNUCXX)
	list(APPEND MAIN_CXX_FLAGS -Weffc++)
endif()
if(CMAKE_BUILD_TYPE EQUAL "Debug")
	list(APPEND MAIN_CXX_FLAGS -O0 -ggdb3)
endif()


# System dependencies.
find_library(LIB_SDL SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Local dependencies.
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
include_directories(
	"${CMAKE_SOURCE_DIR}/include"
	"${DUKTAPE_INCLUDE_DIRS}"
	"${EASYLOGGINGPP_INCLUDE_DIRS}"
)
target_compile_options("${PROJECT_NAME}" PUBLIC ${MAIN_CXX_FLAGS})
