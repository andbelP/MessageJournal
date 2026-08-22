#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

#include "ILogger/ILogger.hpp"

class FileLogger final : public ILogger {
   public:
    FileLogger(std::string file_name, ImportanceLevel default_importance_level);

    ErrorCode Log(std::string_view message, ImportanceLevel level) override;
    ErrorCode Log(std::string_view message) override;
    void SetDefaultImportanceLevel(ImportanceLevel level) override;

    bool JournalIsOpen() const noexcept;

   private:
    static std::string ImportanceLevelToString(
        ImportanceLevel level) noexcept;

    std::ofstream journal_;
    ImportanceLevel default_importance_level_;
    mutable std::mutex mutex_;
};
