file(GLOB_RECURSE tyche_lib_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.h)
add_library(tyche SHARED ${tyche_lib_srcs})