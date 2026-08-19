add_library(Logger INTERFACE)

target_sources(
    Logger
    INTERFACE
    FILE_SET HEADERS
    FILES ErrorCode.hpp ILogger.hpp
)

target_compile_features(Logger INTERFACE cxx_std_23)