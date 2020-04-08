cmake_minimum_required(VERSION 3.16)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest")

# Copy test data to cmake binary dir
configure_file(data/flashcards.yaml data/flashcards.yaml COPYONLY)


add_executable(test_parser)
target_sources(test_parser PRIVATE "test_parser.cxx")
target_link_libraries(test_parser PRIVATE gtest_main yaml-cpp::yaml-cpp librevoco)
