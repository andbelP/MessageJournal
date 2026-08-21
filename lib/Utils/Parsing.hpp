#pragma once

#include <optional>

#include "ILogger/ILogger.hpp"

inline std::optional<ImportanceLevel> ParseImportanceLevel(std::string_view value) {
    if (value == "[LOW]") {
        return ImportanceLevel::kLow;
    }
    if (value == "[MEDIUM]") {
        return ImportanceLevel::kMedium;
    }
    if (value == "[HIGH]") {
        return ImportanceLevel::kHigh;
    }
    return std::nullopt;
}