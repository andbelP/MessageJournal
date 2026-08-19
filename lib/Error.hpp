#pragma once

#include <string>

enum class ErrorType{
    kFilesystemError,
    kUndefinedError
};

struct Error{
    ErrorType err;
    std::string description;
};