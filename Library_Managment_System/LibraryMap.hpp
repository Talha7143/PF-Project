#pragma once

#include <SFML/Graphics.hpp>
#include "Structs.hpp"
#include <vector>


class LibraryMap {
private:
    std::vector<sf::FloatRect> collisionBoxes;
    std::vector<Shelf> shelves;
    sf::FloatRect counterBounds;
    sf::FloatRect computerDeskBounds;

public:
    LibraryMap();

    void initialize();
    void render(sf::RenderWindow& window, const sf::Font& font, int highlightedShelfId = 0);

    // Spatial queries & interaction detection
    bool checkCollision(const sf::FloatRect& entityBounds) const;
    int getInteractingShelf(const sf::Vector2f& pos, float radius = 45.0f) const;
    bool isNearCheckoutCounter(const sf::Vector2f& pos, float radius = 50.0f) const;
    bool isNearComputerTerminal(const sf::Vector2f& pos, float radius = 45.0f) const;

    // Shelf metadata
    const std::vector<Shelf>& getShelves() const { return shelves; }
    sf::Vector2f getCounterQueueStart() const;
};
