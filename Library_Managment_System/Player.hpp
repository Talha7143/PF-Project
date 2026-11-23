#pragma once

#include <SFML/Graphics.hpp>
#include "Structs.hpp"
#include "LibraryMap.hpp"


class PlayerEntity {
private:
    Player data;
    float animTimer{ 0.0f };
    int animFrame{ 0 };

public:
    PlayerEntity();

    void update(float dt, const LibraryMap& map);
    void render(sf::RenderWindow& window, const sf::Font& font);

    // Player state accessors
    Player& getData() { return data; }
    const Player& getData() const { return data; }
    void setData(const Player& p) { data = p; }

    sf::Vector2f getPosition() const { return sf::Vector2f(data.position.x, data.position.y); }
    void setPosition(const sf::Vector2f& pos) { data.position.x = pos.x; data.position.y = pos.y; }

    // Inventory operations
    bool pickUpBook(int isbn, const char* title, int shelfOrigin);
    bool dropOrIssueBook();
    bool isCarrying() const { return data.isCarryingBook; }
    int getCarriedISBN() const { return data.carriedBookIsbn; }
    const char* getCarriedTitle() const { return data.carriedBookTitle; }

    // Metrics
    void addScore(int points);
    void incrementServed();
    void incrementLost();
    sf::FloatRect getBounds() const;
};
