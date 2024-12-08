cmake_minimum_required(VERSION 3.24)
project(Cryptopals)

include(FetchContent)
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

add_subdirectory(set1)

