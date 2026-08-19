#pragma once

#include <expected>

#include "Error.hpp"

enum class ImportanceLevel {
    kHigh,
    kMedium,
    kLow
};

class ILogger {
public:

    virtual std::expected<void, Error> Log(std::string_view message, ImportanceLevel level) = 0;

};