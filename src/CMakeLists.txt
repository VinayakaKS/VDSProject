add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)

add_library(Manager Manager.cpp)

include_directories($CMAKE_SOURCE_DIR/src/bench/)
link_directories($CMAKE_SOURCE_DIR/src/bench/)

include_directories($CMAKE_SOURCE_DIR/src/verify/)
link_directories($CMAKE_SOURCE_DIR/src/verify/)