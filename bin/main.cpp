// main.cpp = file for testing while developing

#include <iostream>
#include "ILogger/ILogger.hpp"
#include "FileLogger/FileLogger.hpp"

int main(){
    FileLogger logger("hahaha.txt", ImportanceLevel::kLow);
    logger.Log("first log");
    logger.Log("first log", ImportanceLevel::kHigh);
}
