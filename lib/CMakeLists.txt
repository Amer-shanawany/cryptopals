add_library(
	libbytes
	SHARED
	bytes.c
)

target_include_directories(
    libbytes
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
)