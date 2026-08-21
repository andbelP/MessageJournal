cmake_minimum_required(VERSION 3.23)

project(LogLibrary)

include(CTest)

add_subdirectory(lib)
add_subdirectory(bin)

# if(BUILD_TESTING)
#     add_subdirectory(tests)
# endif()
