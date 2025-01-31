add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)

add_library(Manager Manager.cpp)
add_library(Reachability Reachability.cpp)

