add_library(gsa dynarr.c tree.c dft.c dft2.c bft.c)

add_executable(dynarr_test dynarr_test.c)
target_link_libraries(dynarr_test gsa)
add_test(dynarr_test dynarr_test)

add_executable(traversal_test traversal_test.c)
target_link_libraries(traversal_test gsa)
add_test(traversal_test traversal_test)
