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

add_executable(
	base64_test
	base64_test.c
)

target_link_libraries(
	base64_test
    PRIVATE
    libbase64
	cunit
)

add_test(NAME base64_test COMMAND  $<TARGET_FILE:base64_test>)

add_executable(
	utils_test
	utils_test.c
)

target_link_libraries(
	utils_test
    PRIVATE
	libbytes
    libbase64
	libutils
	cunit
)

add_test(NAME utils_test COMMAND  $<TARGET_FILE:utils_test>)


add_executable(
	aes_test
	aes_test.c
)

target_link_libraries(
	aes_test
    PRIVATE
	libaes
	cunit
)

add_test(NAME aes_test COMMAND  $<TARGET_FILE:aes_test>)
