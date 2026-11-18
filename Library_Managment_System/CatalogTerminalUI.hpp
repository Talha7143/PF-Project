#pragma once

#include <SFML/Graphics.hpp>
#include "Structs.hpp"
#include "Catalog.hpp"
#include <string>

// ============================================================================
// CatalogTerminalUI: In-Game Interactive Dual-Engine Search Interface
// Features Text Input Boxes, Clickable Algorithm Triggers, and Real-Time Results
// ============================================================================

class CatalogTerminalUI {
private:
    bool isOpen{ false };
    std::string isbnInput{ "1004" };
    std::string titleInput{ "Algorithms" };
    int activeInputBox{ 0 }; // 0: ISBN, 1: Title
    SearchResult lastResult;
    bool hasSearched{ false };

    // Layout rectangles
    sf::FloatRect modalBounds{ { 480.f, 220.f }, { 300.f, 260.f } };
    sf::FloatRect isbnBoxBounds{ { 530.f, 260.f }, { 230.f, 24.f } };
    sf::FloatRect titleBoxBounds{ { 530.f, 292.f }, { 230.f, 24.f } };
    sf::FloatRect binaryBtnBounds{ { 510.f, 326.f }, { 115.f, 26.f } };
    sf::FloatRect linearBtnBounds{ { 635.f, 326.f }, { 115.f, 26.f } };
    sf::FloatRect closeBtnBounds{ { 750.f, 225.f }, { 22.f, 20.f } };

public:
    CatalogTerminalUI();

    void toggle() { isOpen = !isOpen; }
    void open() { isOpen = true; }
    void close() { isOpen = false; }
    bool isVisible() const { return isOpen; }

    void handleEvent(const sf::Event& event, Catalog& catalog);
    void executeBinarySearch(Catalog& catalog);
    void executeLinearSearch(Catalog& catalog);

    void render(sf::RenderWindow& window, const sf::Font& font);
};
