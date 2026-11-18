#include "BinaryFileManager.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstring>

bool BinaryFileManager::saveCatalog(const std::vector<Book>& catalog, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "[FILE SYSTEM ERROR]: Failed to open " << filename << " for binary writing.\n";
        return false;
    }

    uint32_t recordCount = static_cast<uint32_t>(catalog.size());
    outFile.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));

    for (const auto& book : catalog) {
        outFile.write(reinterpret_cast<const char*>(&book), sizeof(Book));
    }

    outFile.close();
    std::cout << "[FILE SYSTEM]: Flushing " << catalog.size() << " 'Book' struct records to binary file '" << filename << "'...\n";
    return true;
}

bool BinaryFileManager::loadCatalog(std::vector<Book>& catalog, const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        std::cout << "[FILE SYSTEM]: No existing '" << filename << "' found. Will generate initial catalog.\n";
        return false;
    }

    uint32_t recordCount = 0;
    inFile.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));

    if (recordCount > 1000) { // Safety sanity check
        inFile.close();
        return false;
    }

    catalog.clear();
    catalog.resize(recordCount);

    for (uint32_t i = 0; i < recordCount; ++i) {
        inFile.read(reinterpret_cast<char*>(&catalog[i]), sizeof(Book));
    }

    inFile.close();
    std::cout << "[FILE SYSTEM]: Successfully loaded " << catalog.size() << " 'Book' struct records from '" << filename << "'.\n";
    return true;
}

bool BinaryFileManager::saveGameState(const Player& player, const std::vector<Patron>& patrons, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "[FILE SYSTEM ERROR]: Failed to create " << filename << "\n";
        return false;
    }

    std::cout << "[FILE SYSTEM]: Writing player stats, grid coordinates, and patron queues to '" << filename << "'.\n";

    // 1. Write Player struct
    outFile.write(reinterpret_cast<const char*>(&player), sizeof(Player));

    // 2. Write Patron queue count & records
    uint32_t patronCount = static_cast<uint32_t>(patrons.size());
    outFile.write(reinterpret_cast<const char*>(&patronCount), sizeof(patronCount));

    for (const auto& p : patrons) {
        outFile.write(reinterpret_cast<const char*>(&p), sizeof(Patron));
    }

    outFile.close();
    std::cout << "[FILE SYSTEM]: Save process completed successfully.\n";
    return true;
}

bool BinaryFileManager::loadGameState(Player& player, std::vector<Patron>& patrons, const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        return false;
    }

    inFile.read(reinterpret_cast<char*>(&player), sizeof(Player));

    uint32_t patronCount = 0;
    inFile.read(reinterpret_cast<char*>(&patronCount), sizeof(patronCount));

    patrons.clear();
    if (patronCount > 0 && patronCount < 100) {
        patrons.resize(patronCount);
        for (uint32_t i = 0; i < patronCount; ++i) {
            inFile.read(reinterpret_cast<char*>(&patrons[i]), sizeof(Patron));
        }
    }

    inFile.close();
    std::cout << "[FILE SYSTEM]: Restored player position (" << player.position.x << ", " << player.position.y 
              << "), Score: " << player.score << ", Day: " << player.day << " from '" << filename << "'.\n";
    return true;
}

bool BinaryFileManager::saveHighScores(const std::vector<HighScoreRecord>& scores, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) return false;

    uint32_t count = static_cast<uint32_t>(scores.size());
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& s : scores) {
        outFile.write(reinterpret_cast<const char*>(&s), sizeof(HighScoreRecord));
    }

    outFile.close();
    std::cout << "[FILE SYSTEM]: High scores archived to '" << filename << "'.\n";
    return true;
}

bool BinaryFileManager::loadHighScores(std::vector<HighScoreRecord>& scores, const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        // Generate seed high scores if none exist
        scores.clear();
        HighScoreRecord s1;
        strncpy_s(s1.playerName, "Master Talha", sizeof(s1.playerName));
        s1.score = 2500;
        s1.dayReached = 7;
        s1.patronsServed = 35;
        strncpy_s(s1.dateString, "2026-11-12", sizeof(s1.dateString));
        scores.push_back(s1);

        HighScoreRecord s2;
        strncpy_s(s2.playerName, "Archivist Noman", sizeof(s2.playerName));
        s2.score = 1800;
        s2.dayReached = 5;
        s2.patronsServed = 24;
        strncpy_s(s2.dateString, "2026-11-10", sizeof(s2.dateString));
        scores.push_back(s2);

        HighScoreRecord s3;
        strncpy_s(s3.playerName, "Scholar Haris", sizeof(s3.playerName));
        s3.score = 1450;
        s3.dayReached = 4;
        s3.patronsServed = 18;
        strncpy_s(s3.dateString, "2026-11-08", sizeof(s3.dateString));
        scores.push_back(s3);

        saveHighScores(scores, filename);
        return true;
    }

    uint32_t count = 0;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
    scores.clear();
    scores.resize(count);

    for (uint32_t i = 0; i < count; ++i) {
        inFile.read(reinterpret_cast<char*>(&scores[i]), sizeof(HighScoreRecord));
    }

    inFile.close();
    return true;
}

void BinaryFileManager::logTransaction(const TransactionRecord& tx, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::app);
    if (outFile.is_open()) {
        time_t now = time(nullptr);
        tm ltm;
        localtime_s(&ltm, &now);
        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &ltm);

        outFile << "[" << timeBuf << "] TX#" << tx.id 
                << " | Patron ID: " << tx.patronId 
                << " | ISBN: " << tx.isbn 
                << " | Action: " << tx.action 
                << " | Delta: " << tx.scoreDelta << "\n";
        outFile.close();
    }
}
