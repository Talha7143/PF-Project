#include "GameEngine.hpp"
#include <iostream>

#ifndef COMPILED_WITH_MAIN_CPP
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
    std::cout << "[CONTROLS]: WASD / Arrow Keys to move librarian.\n";
    std::cout << "[CONTROLS]: TAB to open Catalog Terminal (Binary/Linear Search).\n";
    std::cout << "[CONTROLS]: E or Spacebar to retrieve/issue books.\n";
    std::cout << "[CONTROLS]: ESC to pause and save to catalog.dat & savegame.bin.\n\n";

    engine.run();

    std::cout << "[INFO]: Save process completed successfully. Terminating SFML RenderWindow.\n";
    return 0;
}
#endif
