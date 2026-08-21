#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>

#include "ILogger/ILogger.hpp"

class LoggerFromConsole {
   public:
    explicit LoggerFromConsole(ILogger& logger) : logger_(logger) {}

    void Run();

   private:
    struct Task {
        std::string message;
        std::optional<ImportanceLevel> level;
    };

    void WorkerLoop();

    ILogger& logger_;
    std::queue<Task> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool finished_ = false;
};
