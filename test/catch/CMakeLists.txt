if(BUILD_TESTS)
	file(
		GLOB_RECURSE
		CATCH_TEST_CPPS
		"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
	)

	add_executable(testmain "testmain.cpp" ${CATCH_TEST_CPPS})
	target_link_libraries(testmain ${ALL_LIBS})
	target_compile_options(testmain PUBLIC ${MAIN_CXX_FLAGS})

	if(MEMCHECK_TESTS)
		set(MEMCHECK_OPTIONS --leak-check=full --show-reachable=yes)
		add_test(
			memcheck
			valgrind ${MEMCHECK_OPTIONS} "${CMAKE_CURRENT_BINARY_DIR}/testmain"
		)
	endif()
endif()
