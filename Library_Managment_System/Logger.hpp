#pragma once

#include <string>
#include <fstream>
#include <iostream>

// ============================================================================
// Logger: Unified Diagnostic Logging Subsystem
// Formats console timestamps and handles system log categorization
// ============================================================================

enum class LogLevel {
    INFO,
    SYSTEM,
    FILE_IO,
    SEARCH,
    WARNING,
    ERR
};

class Logger {
private:
    static std::string getTimestamp();

public:
    static void log(LogLevel level, const std::string& message);
    static void logSearch(const std::string& engine, int target, int ops, bool success);
    static void logFileIO(const std::string& action, const std::string& filename, size_t count);
};
