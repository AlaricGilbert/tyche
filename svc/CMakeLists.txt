file(GLOB_RECURSE svc_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.h)
add_executable(svc ${svc_srcs})
target_link_libraries(svc tyche pthread)