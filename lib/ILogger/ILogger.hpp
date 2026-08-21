#pragma once

#include <string_view>

#include "ErrorHandling/ErrorCode.hpp"

enum class ImportanceLevel {
    kHigh = 2,
    kMedium = 1,
    kLow = 0
};

class ILogger {
public:

    virtual ErrorCode Log(std::string_view message, ImportanceLevel level) = 0;

    virtual ErrorCode Log(std::string_view message) = 0;

    virtual void SetDefaultImportanceLevel(ImportanceLevel level) = 0;

    virtual ~ILogger() = default;

};