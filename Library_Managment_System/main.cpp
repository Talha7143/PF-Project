#define COMPILED_WITH_MAIN_CPP
#include "GameEngine.hpp"
#include <iostream>

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
