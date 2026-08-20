add_library(Logger FileLogger/FileLogger.cpp)

target_sources(
    Logger
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ErrorHandling/ErrorCode.hpp
        ILogger/ILogger.hpp
        FileLogger/FileLogger.hpp
)

target_compile_features(Logger PUBLIC cxx_std_17)
