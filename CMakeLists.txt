# revoco -- flashcards that test you

cmake_minimum_required(VERSION 3.17)

project(
	revoco
	VERSION 0.0.1
	DESCRIPTION "flashcards that test you"
	LANGUAGES CXX
)


# machine specific settings; not version controlled
include(local_cmake.txt OPTIONAL)

# Source

## shared libraries

add_library(
	librevoco
	SHARED
)
target_compile_features(
	librevoco
	PRIVATE
	cxx_std_20
)
target_sources(
	librevoco
	PRIVATE
	"src/librevoco/cards.cxx"
)
target_include_directories(
	librevoco
	PUBLIC
	"include"
)

## binaries

add_executable(revoco_client)
target_compile_features(
	revoco_client
	PRIVATE
	cxx_std_20
)
target_sources(
	revoco_client
	PRIVATE
	"src/revoco_client/main.cxx"
)
target_link_libraries(
	revoco_client
	librevoco
)


# Tests

include(CTest)
add_subdirectory(test)
