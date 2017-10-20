if(NOT DUKTAPE_PATH)
	message(SEND_ERROR "Please define DUKTAPE_PATH")
endif()


set(DUKTAPE_VERSION "2.2.0")
set(DUKTAPE_LIBRARY "duktape")
set(
	DUKTAPE_INCLUDE_DIRS
	"${DUKTAPE_PATH}/src"
	# "${DUKTAPE_PATH}/extras/logging"
	# "${DUKTAPE_PATH}/extras/console"
	"${DUKTAPE_PATH}/extras/print-alert"
)
set(DUKTAPE_C_SOURCES
	"${DUKTAPE_PATH}/src/duktape.c"
	# "${DUKTAPE_PATH}/extras/logging/duk_logging.c"
	# "${DUKTAPE_PATH}/extras/console/duk_console.c"
	"${DUKTAPE_PATH}/extras/print-alert/duk_print_alert.c"
)


add_library(${DUKTAPE_LIBRARY} SHARED ${DUKTAPE_C_SOURCES})
set_target_properties(
	${DUKTAPE_LIBRARY}
	PROPERTIES SOVERSION
	${DUKTAPE_VERSION}
)
target_include_directories(${DUKTAPE_LIBRARY} PUBLIC ${DUKTAPE_INCLUDE_DIRS})
target_compile_options(
	${DUKTAPE_LIBRARY}
	PUBLIC
	${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}
)
