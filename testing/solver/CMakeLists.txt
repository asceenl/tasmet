# add_executable(newton_raphson_test newton_raphson_test.cpp)
add_executable(brent_test brent_test.cpp)
target_link_libraries(brent_test tasmet_src armadillo openblas pthread)
# target_link_libraries(newton_raphson_test tasmet_src tasmet_src armadillo openblas pthread)
