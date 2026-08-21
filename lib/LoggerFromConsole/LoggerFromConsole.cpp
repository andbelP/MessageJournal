#include "LoggerFromConsole/LoggerFromConsole.hpp"

#include <iostream>
#include <string_view>

namespace {

std::optional<ImportanceLevel> ParseImportanceLevel(std::string_view value) {
    if (value == "low") {
        return ImportanceLevel::kLow;
    }
    if (value == "medium") {
        return ImportanceLevel::kMedium;
    }
    if (value == "high") {
        return ImportanceLevel::kHigh;
    }
    return std::nullopt;
}

}  // namespace

void LoggerFromConsole::Run() {
    std::thread worker(&LoggerFromConsole::WorkerLoop, this);

    std::string line;
    while (std::getline(std::cin, line) && line != "/exit") {
        if (line.empty()) {
            continue;
        }

        Task task{line, std::nullopt};
        std::size_t separator = line.find(' ');
        std::string_view first_word =
            std::string_view(line).substr(0, separator);

        auto level = ParseImportanceLevel(first_word);
        if (level) {
            task.level = level;
            if (separator == std::string::npos) {
                task.message = "";
            } else {
                task.message = line.substr(separator + 1);
            }
        }

        {
            std::lock_guard lock(mutex_);
            tasks_.push(std::move(task));
        }
        condition_.notify_one();
    }

    {
        std::lock_guard lock(mutex_);
        finished_ = true;
    }
    condition_.notify_one();
    worker.join();
}

void LoggerFromConsole::WorkerLoop() {
    while (true) {
        Task task;

        {
            std::unique_lock lock(mutex_);
            condition_.wait(lock,
                            [this] { return finished_ || !tasks_.empty(); });

            if (tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        if (task.level) {
            logger_.Log(task.message, *task.level);
        } else {
            logger_.Log(task.message);
        }
    }
}
