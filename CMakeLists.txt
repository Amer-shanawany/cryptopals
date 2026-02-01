cmake_minimum_required(VERSION 3.24)
project(Cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(CUNIT_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(CUNIT_DISABLE_EXAMPLES ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	cunit
	GIT_REPOSITORY https://gitlab.com/cunity/cunit.git
	GIT_TAG dba8150c5ea350a960bb9126c8d1aa589e42832e # release 3.4.4
)
FetchContent_MakeAvailable(cunit)

FetchContent_GetProperties(cunit)
if (NOT cunit_POPULATED)
	FetchContent_Populate(cunit)
	add_subdirectory(${cunit_SOURCE_DIR} ${cunit_BINARY_DIR})
endif()

include(CTest)
enable_testing()

add_subdirectory(set1)
add_subdirectory(lib)
add_subdirectory(tests)
