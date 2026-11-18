#define COMPILED_WITH_MAIN_CPP
#include "GameEngine.hpp"
#include <iostream>

// ============================================================================
// Bibliotheca Quest: The Ultimate Library Simulation & Management Game
// Lead Developer: Muhammad Talha | Roll No: 26L-0755 | Group: 15
// Course: Programming Fundamentals (Fall 2026) - FAST-NUCES Lahore
//
// Linux / macOS / CLI Build command:
// g++ -std=c++17 main.cpp GameEngine.cpp Catalog.cpp BinaryFileManager.cpp \
//     LibraryMap.cpp Player.cpp PatronManager.cpp CatalogTerminalUI.cpp \
//     LoanManager.cpp -o BibliothecaQuest -lsfml-graphics -lsfml-window \
//     -lsfml-system -lsfml-audio
// ============================================================================

#ifdef BUILD_STANDALONE_CLI
int main() {
    std::cout << "========================================================\n";
    std::cout << "           BIBLIOTHECA QUEST - SFML 2D ENGINE           \n";
    std::cout << "========================================================\n";
    std::cout << "[INFO]: Initializing Bibliotheca Quest Simulation Game...\n";

    GameEngine engine;
    if (!engine.initialize()) {
        std::cerr << "[FATAL ERROR]: Failed to initialize game engine subsystems.\n";
        return -1;
    }

    std::cout << "[INFO]: Entering Main Simulation Loop.\n";
    engine.run();

    std::cout << "[INFO]: Application closed cleanly.\n";
    return 0;
}
#endif
