cmake_minimum_required (VERSION 3.5)
set (CMAKE_DISABLE_SOURCE_CHANGES ON)
set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)


set (PROJECT_NAME dukdemo)
project (${PROJECT_NAME})


include(lib/duktape.cmake)


set (EXTRA_CXX_FLAGS
	"-Werror -std=c++1y -Wall -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wno-int-conversions -Wsign-promo")


# TODO: Check the gdb version (how?) and pick the appropriate flags
# accordingly.
# See http://stackoverflow.com/questions/12595631/debugging-with-gdb-on-a-program-with-no-optimization-but-still-there-is-no-symbo#12595773 
# set (CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb ${EXTRA_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 -gdwarf-2 ${EXTRA_CXX_FLAGS}")

if(${CMAKE_COMPILER_IS_GNUCXX})
	set(EXTRA_CXX_FLAGS "${EXTRA_CXX_FLAGS} -Wno-pmf-conversions")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")


# find_library(LIB_SDL SDL2)
# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)


message(STATUS "duktape dir ${DUKTAPE_INCLUDE_DIR}")
include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  "${DUKTAPE_INCLUDE_DIRS}"
)


# TODO: add ${LIB_SDL} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
set(DEP_LIBS ${DUKTAPE_LIBRARY})
set(ALL_LIBS ${DEP_LIBS})


add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
message(STATUS "linking target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})")
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
