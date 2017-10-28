cmake_minimum_required (VERSION 3.5)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


set(PROJECT_NAME dukdemo)
set(PROJECT_LIB demolib)
project (${PROJECT_NAME} LANGUAGES CXX C VERSION 1.0.0)


# Compiler flags.
list(APPEND MAIN_CXX_FLAGS
	-Werror -std=c++1z -Wall -Wold-style-cast -Woverloaded-virtual
	-Wsign-promo -Wno-pmf-conversions
)
if(CMAKE_COMPILER_IS_GNUCXX)
	list(APPEND MAIN_CXX_FLAGS -Weffc++)
endif()
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	list(APPEND MAIN_CXX_FLAGS -O0 -ggdb3)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb3")
endif()


# External dependencies.
find_library(LIB_SDL SDL2 REQUIRED PATHS "${SDL2_LIBRARY_DIR}")
find_library(LIB_BOX2D Box2D REQUIRED PATHS "${BOX2D_LIBRARY_DIR}")
find_library(LIB_B2DRAW b2draw REQUIRED PATHS "${B2DRAW_LIBRARY_DIR}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Dependencies we build ourselves.
include(lib/duktape.cmake)
include(lib/easyloggingpp.cmake)


set(
	DEP_LIBS
	${DUKTAPE_LIBRARY}
	${EASYLOGGINGPP_LIBRARY}
	${LIB_B2DRAW}
	${LIB_SDL}
	${LIB_BOX2D}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)
set(ALL_LIBS ${PROJECT_LIB} ${DEP_LIBS})


# Project-wide options.
include_directories(
	"${CMAKE_SOURCE_DIR}/include"
	"${DUKTAPE_INCLUDE_DIRS}"
)
include_directories(SYSTEM "${EASYLOGGINGPP_INCLUDE_DIRS}")
if(SDL2_INCLUDE_DIR)
	include_directories("${SDL2_INCLUDE_DIR}")
endif()
if(BOX2D_INCLUDE_DIR)
	include_directories(SYSTEM "${BOX2D_INCLUDE_DIR}")
endif()
if(B2DRAW_INCLUDE_DIR)
	include_directories("${B2DRAW_INCLUDE_DIR}")
endif()
if(GLM_INCLUDE_DIR)
	include_directories(SYSTEM "${GLM_INCLUDE_DIR}")
endif()


# Configure the project shared library.
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/**/*.cpp")
add_library(${PROJECT_LIB} ${LIB_SOURCES})
target_link_libraries(${PROJECT_LIB} ${DEP_LIBS})
target_compile_options(${PROJECT_LIB} PUBLIC ${MAIN_CXX_FLAGS})


# Configure the project executable.
add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
target_compile_options("${PROJECT_NAME}" PUBLIC ${MAIN_CXX_FLAGS})
