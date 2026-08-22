cmake_minimum_required(VERSION 3.23)

project(LogLibrary)

include(CTest)

add_subdirectory(lib)
add_subdirectory(bin)


option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
