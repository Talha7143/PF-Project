#pragma once

#include <cstdint>

namespace Config {

    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 600;
    constexpr unsigned int TARGET_FPS = 60;
    constexpr const char* WINDOW_TITLE = "Bibliotheca Quest: The Ultimate Library Simulation";

    constexpr float PLAYER_DEFAULT_SPEED = 5.5f;
    constexpr float PLAYER_SPRINT_MULTIPLIER = 1.4f;
    constexpr int PLAYER_BASE_CAPACITY = 5;

    constexpr float PATRON_SPAWN_INTERVAL_MIN = 4.5f;
    constexpr float PATRON_SPAWN_INTERVAL_MAX = 7.5f;
    constexpr int MAX_ACTIVE_PATRONS = 8;
    constexpr float PATRON_DEFAULT_PATIENCE = 80.0f;
    constexpr float PATRON_PATIENCE_DECAY_QUEUE = 1.0f;
    constexpr float PATRON_PATIENCE_DECAY_COUNTER = 1.5f;

    constexpr int SCORE_LOAN_SUCCESS = 150;
    constexpr int SCORE_ON_TIME_RETURN = 75;
    constexpr int PENALTY_PATRON_IMPATIENT = -50;
    constexpr int PENALTY_WRONG_BOOK = -25;
    constexpr int MAX_PATRONS_LOST_FAIL = 5;

    constexpr float DAY_START_HOUR = 9.0f;
    constexpr float DAY_END_HOUR = 18.0f;
    constexpr float TIME_SCALE_PER_SEC = 0.005f;

    constexpr const char* FILE_CATALOG = "catalog.dat";
    constexpr const char* FILE_SAVEGAME = "savegame.bin";
    constexpr const char* FILE_SCORES = "scores.dat";
    constexpr const char* FILE_LOG = "transaction.log";
}
