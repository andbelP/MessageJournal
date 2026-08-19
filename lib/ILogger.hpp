#pragma once

#include <string_view>

#include "ErrorCode.hpp"

enum class ImportanceLevel {
    kHigh,
    kMedium,
    kLow
};

class ILogger {
public:

    virtual ErrorCode Log(std::string_view message, ImportanceLevel level) = 0;

    virtual void SetDefaultImportanceLevel(ImportanceLevel level) = 0;

    virtual ~ILogger() = default;

};