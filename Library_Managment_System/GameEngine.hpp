#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Structs.hpp"
#include "Catalog.hpp"
#include "LibraryMap.hpp"
#include "Player.hpp"
#include "PatronManager.hpp"
#include "CatalogTerminalUI.hpp"
#include "BinaryFileManager.hpp"

class GameEngine {
private:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded{ false };

    GameState currentState{ GameState::MainMenuState };

    Catalog catalog;
    LibraryMap map;
    PlayerEntity player;
    PatronManager patronMgr;
    CatalogTerminalUI terminalUI;

    std::vector<HighScoreRecord> highScores;
    std::string notificationMsg{ "" };
    float notificationTimer{ 0.0f };

    int selectedMenuIndex{ 0 };
    std::vector<std::string> menuOptions{ "START SIMULATION", "LOAD SAVED GAME", "LEADERBOARD", "HOW TO PLAY", "EXIT" };

    int selectedPauseIndex{ 0 };
    std::vector<std::string> pauseOptions{ "RESUME", "SAVE GAME (savegame.bin)", "CATALOG TERMINAL", "MAIN MENU" };

    void handleMainMenuInput(const sf::Event& event);
    void handleGameplayInput(const sf::Event& event);
    void handlePauseInput(const sf::Event& event);
    void handleGameOverInput(const sf::Event& event);
    void handleLeaderboardInput(const sf::Event& event);
    void handleHowToPlayInput(const sf::Event& event);

    void renderMainMenu();
    void renderGameplay();
    void renderHUD();
    void renderPauseMenu();
    void renderGameOver();
    void renderLeaderboard();
    void renderHowToPlay();
    void showNotification(const std::string& msg, float duration = 3.0f);

public:
    GameEngine();
    ~GameEngine();

    bool initialize();
    void run();
};
