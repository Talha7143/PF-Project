#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct Vector2D {
    float x{ 0.0f };
    float y{ 0.0f };

    Vector2D() = default;
    Vector2D(float _x, float _y) : x(_x), y(_y) {}
};

struct Book {
    int isbn{ 0 };
    char title[64]{ 0 };
    char author[48]{ 0 };
    char category[24]{ 0 };
    char location[24]{ 0 };
    int shelfId{ 0 };
    int gridX{ 0 };
    int gridY{ 0 };
    int copies{ 0 };
    int totalCopies{ 0 };

    Book() = default;
    Book(int _isbn, const char* _title, const char* _author, const char* _category,
         const char* _location, int _shelfId, int _gx, int _gy, int _copies)
        : isbn(_isbn), shelfId(_shelfId), gridX(_gx), gridY(_gy), copies(_copies), totalCopies(_copies)
    {
        setTitle(_title);
        setAuthor(_author);
        setCategory(_category);
        setLocation(_location);
    }

    void setTitle(const char* t) {
        if (!t) return;
        size_t i = 0;
        for (; i < 63 && t[i] != '\0'; ++i) title[i] = t[i];
        title[i] = '\0';
    }

    void setAuthor(const char* a) {
        if (!a) return;
        size_t i = 0;
        for (; i < 47 && a[i] != '\0'; ++i) author[i] = a[i];
        author[i] = '\0';
    }

    void setCategory(const char* c) {
        if (!c) return;
        size_t i = 0;
        for (; i < 23 && c[i] != '\0'; ++i) category[i] = c[i];
        category[i] = '\0';
    }

    void setLocation(const char* loc) {
        if (!loc) return;
        size_t i = 0;
        for (; i < 23 && loc[i] != '\0'; ++i) location[i] = loc[i];
        location[i] = '\0';
    }
};

struct Shelf {
    int id{ 0 };
    char label[16]{ 0 };
    char category[24]{ 0 };
    float x{ 0.0f };
    float y{ 0.0f };
    float width{ 120.0f };
    float height{ 80.0f };
    int bookIsbns[16]{ 0 };
    int bookCount{ 0 };
};

enum class PatronState {
    ENTERING,
    IN_QUEUE,
    WAITING_AT_COUNTER,
    BROWSING,
    LEAVING_SATISFIED,
    LEAVING_ANGRY
};

struct Patron {
    int id{ 0 };
    char name[32]{ 0 };
    Vector2D position;
    Vector2D velocity;
    Vector2D target;
    int requestedIsbn{ 0 };
    char requestedTitle[64]{ 0 };
    float patience{ 100.0f };
    float maxPatience{ 100.0f };
    PatronState state{ PatronState::ENTERING };
    int queueIndex{ -1 };
    bool served{ false };

    uint8_t colorR{ 200 };
    uint8_t colorG{ 100 };
    uint8_t colorB{ 100 };
};

struct Player {
    char name[32]{ "Talha" };
    Vector2D position{ 400.0f, 300.0f };
    Vector2D velocity{ 0.0f, 0.0f };
    float speed{ 4.5f };
    int facing{ 0 };

    bool isCarryingBook{ false };
    int carriedBookIsbn{ 0 };
    char carriedBookTitle[64]{ 0 };
    int carriedShelfOrigin{ 0 };

    int score{ 1250 };
    int day{ 4 };
    float timeOfDay{ 10.75f };
    int patronsServed{ 14 };
    int patronsLost{ 1 };
    int booksCapacity{ 5 };
    int booksHeld{ 0 };
};

struct TransactionRecord {
    int id{ 0 };
    int timestampSeconds{ 0 };
    int patronId{ 0 };
    int isbn{ 0 };
    char action[20]{ 0 };
    int scoreDelta{ 0 };
};

struct HighScoreRecord {
    char playerName[32]{ 0 };
    int score{ 0 };
    int dayReached{ 1 };
    int patronsServed{ 0 };
    char dateString[24]{ 0 };
};

enum class GameState {
    MainMenuState,
    GameplayState,
    CatalogUIState,
    PauseState,
    GameOverState,
    LeaderboardState,
    HowToPlayState
};

enum class SearchType {
    LINEAR_ISBN,
    LINEAR_TITLE
};

struct SearchResult {
    bool found{ false };
    int operationsCount{ 0 };
    SearchType searchType{ SearchType::LINEAR_ISBN };
    Book matchedBook;
    int matchedIndices[32]{ 0 };
    int matchCount{ 0 };
};
