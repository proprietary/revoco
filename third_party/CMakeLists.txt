cmake_minimum_required(VERSION 3.17)

include(ExternalProject)

# ncurses

set(NCURSES_PREFIX_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps/ncurses)

ExternalProject_Add(
  ncurses_static_lib
  PREFIX ${NCURSES_PREFIX_DIR}
  SOURCE_DIR ${NCURSES_PREFIX_DIR}/src
  INSTALL_DIR ${NCURSES_PREFIX_DIR}/dist
  URL ftp://ftp.gnu.org/gnu/ncurses/ncurses-6.2.tar.gz
  URL_HASH "SHA256=30306e0c76e0f9f1f0de987cf1c82a5c21e1ce6568b9227f7da5b71cbea86c9d"
  CONFIGURE_COMMAND ${NCURSES_PREFIX_DIR}/src/ncurses-6.2/configure --prefix ${NCURSES_PREFIX_DIR}/dist --enable-widec
  BUILD_COMMAND make -j
  INSTALL_COMMAND make install
)

add_library(ncurses STATIC IMPORTED GLOBAL)
add_dependencies(ncurses ncurses_static_lib)
target_include_directories(ncurses INTERFACE ${ncurses_static_lib}/dist/include)
target_link_libraries(ncurses INTERFACE
  libformw.a
  libformw.a
  libmenuw.a
  libncurses++w.a
  libncursesw.a
  libpanelw.a
)
