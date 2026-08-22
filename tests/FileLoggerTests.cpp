#include "FileLogger/FileLogger.hpp"

#include <gtest/gtest.h>

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

TEST(FileLoggerTest, RespectsImportanceLevelAndDefaultLevel) {
    const char* journal_path = "/tmp/file_logger_importance_test.log";
    std::remove(journal_path);

    {
        FileLogger logger(journal_path, ImportanceLevel::kLow);

        EXPECT_EQ(logger.Log("low message", ImportanceLevel::kLow),
                  ErrorCode::kOk);

        logger.SetDefaultImportanceLevel(ImportanceLevel::kMedium);
        EXPECT_EQ(logger.Log("filtered message", ImportanceLevel::kLow),
                  ErrorCode::kOk);
        EXPECT_EQ(logger.Log("medium message"), ErrorCode::kOk);
        EXPECT_EQ(logger.Log("high message", ImportanceLevel::kHigh),
                  ErrorCode::kOk);
    }

    std::ifstream journal(journal_path);
    std::vector<std::string> lines;
    for (std::string line; std::getline(journal, line);) {
        lines.push_back(line);
    }

    ASSERT_EQ(lines.size(), 3U);
    EXPECT_NE(lines[0].find("[LOW] low message"), std::string::npos);
    EXPECT_NE(lines[1].find("[MEDIUM] medium message"), std::string::npos);
    EXPECT_NE(lines[2].find("[HIGH] high message"), std::string::npos);

    std::remove(journal_path);
}

TEST(FileLoggerTest, ReportsErrorWhenJournalCannotBeOpened) {
    FileLogger logger("/tmp", ImportanceLevel::kLow);

    EXPECT_FALSE(logger.JournalIsOpen());
    EXPECT_EQ(logger.Log("message", ImportanceLevel::kHigh),
              ErrorCode::kFilesystemError);
}

TEST(FileLoggerTest, IsThreadSafe) {
    constexpr int kThreadCount = 4;
    constexpr int kMessagesPerThread = 10;
    const char* journal_path = "/tmp/file_logger_multithread_test.log";

    // The lock file prevents parallel test processes from sharing the journal.
    const int lock_file = open("/tmp/file_logger_multithread_test.lock",
                               O_CREAT | O_RDWR, 0600);
    ASSERT_NE(lock_file, -1);
    ASSERT_EQ(flock(lock_file, LOCK_EX), 0);
    std::remove(journal_path);

    {
        FileLogger logger(journal_path, ImportanceLevel::kLow);
        ASSERT_TRUE(logger.JournalIsOpen());

        std::vector<std::thread> threads;
        for (int thread = 0; thread < kThreadCount; ++thread) {
            threads.emplace_back([&logger, thread] {
                for (int message = 0; message < kMessagesPerThread; ++message) {
                    logger.Log("message " + std::to_string(thread) + " " +
                                   std::to_string(message),
                               ImportanceLevel::kHigh);
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    std::ifstream journal(journal_path);
    int line_count = 0;
    for (std::string line; std::getline(journal, line);) {
        EXPECT_NE(line.find("[HIGH] message "), std::string::npos);
        ++line_count;
    }
    EXPECT_EQ(line_count, kThreadCount * kMessagesPerThread);

    std::remove(journal_path);
    flock(lock_file, LOCK_UN);
    close(lock_file);
}
