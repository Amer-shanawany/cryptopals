# target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
	bytes_test
	bytes_test.c
)

target_link_libraries(
	bytes_test
    PRIVATE
    libbytes
	cunit
)

add_test(NAME bytes_test COMMAND  $<TARGET_FILE:bytes_test>)
