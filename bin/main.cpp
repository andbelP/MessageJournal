#include <iostream>

#include "FileLogger/FileLogger.hpp"
#include "ILogger/ILogger.hpp"
#include "LoggerFromConsole/LoggerFromConsole.hpp"
#include "Utils/Parsing.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Invalid count of arguments. Use MainTesting <Name of log "
                     "journal file> <Default importance level "
                     "[HIGH]/[MEDIUM]/[LOW]>;\n";
        return EXIT_FAILURE;
    }

    auto default_importance_level_opt = ParseImportanceLevel(argv[2]);
    if (!default_importance_level_opt.has_value()) {
        std::cerr << "Invalid importance level. Use .../MainTesting <Name of "
                     "log journal file> <Default importance level "
                     "[HIGH]/[MEDIUM]/[LOW]>;\n";
        return EXIT_FAILURE;
    }

    FileLogger file_logger(argv[1], default_importance_level_opt.value());

    if (!file_logger.JournalIsOpen()) {
        std::cerr << "Can't open journal. Filesystem Error;\n";
        return EXIT_FAILURE;
    }

    LoggerFromConsole logger_from_console(file_logger);

    logger_from_console.Run();
}
