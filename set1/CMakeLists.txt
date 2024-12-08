add_library(
	set1
	STATIC
	set1.h
	set1.c
)

target_include_directories(set1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(
	set1_test
	set1.h
	set1_test.c
)

target_link_libraries(
	set1_test
	set1
	cunit
)

add_test(NAME set1_test COMMAND set1_test)
