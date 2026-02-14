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

add_library(
    libbase64
	SHARED
	base64.c
)

target_include_directories(
    libbase64
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    libbase64
    PUBLIC
    libbytes
)

add_library(
    libutils
	SHARED
	utils.c
)

target_include_directories(
    libutils
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    libutils
    PUBLIC
    libbytes
)

add_library(
    libcryptopals
    INTERFACE
)

target_link_libraries(
    libcryptopals
    INTERFACE
    libbytes
    libbase64
    libutils
)

target_include_directories(
    libcryptopals
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
)