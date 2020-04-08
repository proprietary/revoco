# revoco -- flashcards that test you

cmake_minimum_required(VERSION 3.16)

project(
	revoco
	VERSION 0.0.1
	DESCRIPTION "flashcards that test you"
	LANGUAGES CXX
)


# machine specific settings; not version controlled
include(local_cmake.txt OPTIONAL)

# Dependencies

set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
# include(cmake/ncurses.cmake)

find_package(Threads REQUIRED)

add_subdirectory(
  third_party/flatbuffers
  ${CMAKE_CURRENT_BINARY_DIR}/build_flatbuffers
  EXCLUDE_FROM_ALL
)

set(YAML_BUILD_SHARED_LIBS ON)
add_subdirectory(
  third_party/yaml-cpp
  ${CMAKE_CURRENT_BINARY_DIR}/build_yaml-cpp
  EXCLUDE_FROM_ALL
)

add_subdirectory(
  third_party/gflags
  ${CMAKE_CURRENT_BINARY_DIR}/build_gflags
  EXCLUDE_FROM_ALL
)

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
target_link_libraries(
  librevoco
  PRIVATE
  flatbuffers
  yaml-cpp::yaml-cpp
)
target_compile_features(
  librevoco
  PUBLIC
  cxx_std_20
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
  PRIVATE
  librevoco
  ncurses
  gflags::gflags
)


# Tests

include(CTest)
add_subdirectory(test)
