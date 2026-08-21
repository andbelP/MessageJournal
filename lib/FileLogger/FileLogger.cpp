#include "FileLogger/FileLogger.hpp"

#include <ctime>
#include <iomanip>

FileLogger::FileLogger(std::string file_name,
                       ImportanceLevel default_importance_level)
    : journal_(file_name, std::ios::app),
      default_importance_level_(default_importance_level) {}

ErrorCode FileLogger::Log(std::string_view message, ImportanceLevel level) {
    std::lock_guard lock(mutex_);

    if (level < default_importance_level_) {
        return ErrorCode::kOk;
    }

    if (!journal_.is_open()) {
        return ErrorCode::kFilesystemError;
    }

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    journal_ << '[' << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << "] ["
             << ImportanceLevelToString(level) << "] " << message << '\n';
    journal_.flush();

    return journal_ ? ErrorCode::kOk : ErrorCode::kFilesystemError;
}

ErrorCode FileLogger::Log(std::string_view message) {
    ImportanceLevel level;
    {
        std::lock_guard lock(mutex_);
        level = default_importance_level_;
    }

    return Log(message, level);
}

void FileLogger::SetDefaultImportanceLevel(ImportanceLevel level) {
    std::lock_guard lock(mutex_);
    default_importance_level_ = level;
}

bool FileLogger::JournalIsOpen() const noexcept {
    return journal_.is_open();
}

std::string FileLogger::ImportanceLevelToString(
    ImportanceLevel level) noexcept {
    switch (level) {
        case ImportanceLevel::kHigh:
            return "HIGH";
        case ImportanceLevel::kMedium:
            return "MEDIUM";
        case ImportanceLevel::kLow:
            return "LOW";
    }

    return "UNKNOWN";
}
