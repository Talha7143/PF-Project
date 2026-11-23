#include "Logger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

std::string Logger::getTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm ltm;
    localtime_s(&ltm, &now);
    std::ostringstream ss;
    ss << "[" << std::put_time(&ltm, "%H:%M:%S") << "]";
    return ss.str();
}

void Logger::log(LogLevel level, const std::string& message) {
    std::string tag = "[INFO]";
    switch (level) {
        case LogLevel::SYSTEM:  tag = "[SYSTEM]"; break;
        case LogLevel::FILE_IO: tag = "[FILE SYSTEM]"; break;
        case LogLevel::SEARCH:  tag = "[SEARCH ENGINE]"; break;
        case LogLevel::WARNING: tag = "[WARNING]"; break;
        case LogLevel::ERR:     tag = "[ERROR]"; break;
        default: break;
    }
    std::cout << getTimestamp() << " " << tag << ": " << message << "\n";
}

void Logger::logSearch(const std::string& engine, int target, int ops, bool success) {
    std::cout << "\n" << getTimestamp() << " [SFML CATALOG TERMINAL - " << engine << "]\n";
    std::cout << "Target: " << target << "\n";
    std::cout << "Status: " << (success ? "MATCH FOUND" : "NOT FOUND")
              << " (Executed in " << ops << " operations)\n\n";
}

void Logger::logFileIO(const std::string& action, const std::string& filename, size_t count) {
    std::cout << getTimestamp() << " [FILE SYSTEM]: " << action << " " << count
              << " records with '" << filename << "'...\n";
}
