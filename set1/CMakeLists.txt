add_executable(
	set1
	set1.c
)

target_link_libraries(
	set1
	libcryptopals
	cunit
)

configure_file(4.txt "${CMAKE_CURRENT_BINARY_DIR}/4.txt" COPYONLY)
configure_file(6.txt "${CMAKE_CURRENT_BINARY_DIR}/6.txt" COPYONLY)
configure_file(7.txt "${CMAKE_CURRENT_BINARY_DIR}/7.txt" COPYONLY)

add_test(NAME set1 COMMAND set1)
