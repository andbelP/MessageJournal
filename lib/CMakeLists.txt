add_library(Logger FileLogger/FileLogger.cpp)

target_sources(
    Logger
    PUBLIC
    FILE_SET HEADERS
    FILES
        ErrorHandling/ErrorCode.hpp
        ILogger/ILogger.hpp
        FileLogger/FileLogger.hpp
)

target_compile_features(Logger PUBLIC cxx_std_17)

add_library(LoggerFromConsole LoggerFromConsole/LoggerFromConsole.cpp)

target_sources(
    LoggerFromConsole
    PUBLIC
    FILE_SET HEADERS
    FILES LoggerFromConsole/LoggerFromConsole.hpp
)

target_link_libraries(LoggerFromConsole PUBLIC Logger)
target_compile_features(LoggerFromConsole PUBLIC cxx_std_17)
