#include "LibraryMap.hpp"
#include "Catalog.hpp"
#include <cmath>
#include <cstring>

LibraryMap::LibraryMap() {
    initialize();
}

void LibraryMap::initialize() {
    collisionBoxes.clear();
    shelves.clear();

    // Map bounds: 800 x 600
    // Outer walls collision boxes
    collisionBoxes.push_back(sf::FloatRect({ 0.f, 0.f }, { 800.f, 25.f }));   // Top wall
    collisionBoxes.push_back(sf::FloatRect({ 0.f, 575.f }, { 360.f, 25.f })); // Bottom left wall
    collisionBoxes.push_back(sf::FloatRect({ 440.f, 575.f }, { 360.f, 25.f }));// Bottom right wall (doorway in center)
    collisionBoxes.push_back(sf::FloatRect({ 0.f, 0.f }, { 20.f, 600.f }));   // Left wall
    collisionBoxes.push_back(sf::FloatRect({ 780.f, 0.f }, { 20.f, 600.f }));  // Right wall

    // Bookshelf 1: Fiction (Top Left)
    Shelf s1;
    s1.id = 1;
    strncpy_s(s1.label, "Shelf A1", sizeof(s1.label));
    strncpy_s(s1.category, "FICTION", sizeof(s1.category));
    s1.x = 45.0f;
    s1.y = 45.0f;
    s1.width = 135.0f;
    s1.height = 75.0f;
    shelves.push_back(s1);
    collisionBoxes.push_back(sf::FloatRect({ s1.x, s1.y }, { s1.width, s1.height }));

    // Bookshelf 2: Sci-Fi (Top Center-Left)
    Shelf s2;
    s2.id = 2;
    strncpy_s(s2.label, "Shelf B1", sizeof(s2.label));
    strncpy_s(s2.category, "SCI-FI", sizeof(s2.category));
    s2.x = 225.0f;
    s2.y = 45.0f;
    s2.width = 135.0f;
    s2.height = 75.0f;
    shelves.push_back(s2);
    collisionBoxes.push_back(sf::FloatRect({ s2.x, s2.y }, { s2.width, s2.height }));

    // Bookshelf 3: Academic (Top Center-Right)
    Shelf s3;
    s3.id = 3;
    strncpy_s(s3.label, "Shelf C1", sizeof(s3.label));
    strncpy_s(s3.category, "ACADEMIC", sizeof(s3.category));
    s3.x = 425.0f;
    s3.y = 45.0f;
    s3.width = 135.0f;
    s3.height = 75.0f;
    shelves.push_back(s3);
    collisionBoxes.push_back(sf::FloatRect({ s3.x, s3.y }, { s3.width, s3.height }));

    // Bookshelf 4: Reference / Quiet Zone (Top Right)
    Shelf s4;
    s4.id = 4;
    strncpy_s(s4.label, "Shelf D1", sizeof(s4.label));
    strncpy_s(s4.category, "QUIET ARCHIVE", sizeof(s4.category));
    s4.x = 615.0f;
    s4.y = 45.0f;
    s4.width = 135.0f;
    s4.height = 75.0f;
    shelves.push_back(s4);
    collisionBoxes.push_back(sf::FloatRect({ s4.x, s4.y }, { s4.width, s4.height }));

    // Reading Table 1 (Left study area)
    collisionBoxes.push_back(sf::FloatRect({ 75.f, 290.f }, { 110.f, 65.f }));

    // Reading Table 2 (Center study area)
    collisionBoxes.push_back(sf::FloatRect({ 615.f, 290.f }, { 110.f, 65.f }));

    // Computer Catalog Terminal Desk (Right wall area)
    computerDeskBounds = sf::FloatRect({ 675.f, 430.f }, { 75.f, 55.f });
    collisionBoxes.push_back(computerDeskBounds);

    // Central Checkout Counter
    counterBounds = sf::FloatRect({ 260.f, 450.f }, { 280.f, 55.f });
    collisionBoxes.push_back(counterBounds);
}

bool LibraryMap::checkCollision(const sf::FloatRect& entityBounds) const {
    for (const auto& box : collisionBoxes) {
        if (box.findIntersection(entityBounds).has_value()) {
            return true;
        }
    }
    return false;
}

int LibraryMap::getInteractingShelf(const sf::Vector2f& pos, float radius) const {
    for (const auto& s : shelves) {
        sf::FloatRect expanded({ s.x - radius, s.y - radius }, { s.width + radius * 2.f, s.height + radius * 2.f });
        if (expanded.contains(pos)) {
            return s.id;
        }
    }
    return 0;
}

bool LibraryMap::isNearCheckoutCounter(const sf::Vector2f& pos, float radius) const {
    sf::FloatRect expanded({ counterBounds.position.x - radius, counterBounds.position.y - radius },
                           { counterBounds.size.x + radius * 2.f, counterBounds.size.y + radius * 2.f });
    return expanded.contains(pos);
}

bool LibraryMap::isNearComputerTerminal(const sf::Vector2f& pos, float radius) const {
    sf::FloatRect expanded({ computerDeskBounds.position.x - radius, computerDeskBounds.position.y - radius },
                           { computerDeskBounds.size.x + radius * 2.f, computerDeskBounds.size.y + radius * 2.f });
    return expanded.contains(pos);
}

sf::Vector2f LibraryMap::getCounterQueueStart() const {
    // Patrons stand at the customer side of the counter (e.g. Y = 410)
    return sf::Vector2f(320.0f, 410.0f);
}

void LibraryMap::render(sf::RenderWindow& window, const sf::Font& font, const Catalog& catalog, int highlightedShelfId) {
    // 1. Draw Parquet Wooden Floor Pattern
    const float tileSize = 40.0f;
    for (float x = 0; x < 800.f; x += tileSize) {
        for (float y = 0; y < 600.f; y += tileSize) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(sf::Vector2f(x, y));
            int cellX = static_cast<int>(x / tileSize);
            int cellY = static_cast<int>(y / tileSize);
            if ((cellX + cellY) % 2 == 0) {
                tile.setFillColor(sf::Color(224, 210, 185)); // Light warm oak
            } else {
                tile.setFillColor(sf::Color(214, 198, 172)); // Darker oak
            }
            window.draw(tile);
        }
    }

    // 2. Entrance Rug (Bottom doorway)
    sf::RectangleShape rug(sf::Vector2f(100.f, 35.f));
    rug.setPosition(sf::Vector2f(350.f, 565.f));
    rug.setFillColor(sf::Color(140, 35, 35));
    rug.setOutlineColor(sf::Color(180, 50, 50));
    rug.setOutlineThickness(2.f);
    window.draw(rug);

    // 3. Render Bookshelves with Pixel-Art Style Bindings and Category Signboards
    for (const auto& s : shelves) {
        // Shelf Main Body
        sf::RectangleShape shelfBody(sf::Vector2f(s.width, s.height));
        shelfBody.setPosition(sf::Vector2f(s.x, s.y));
        shelfBody.setFillColor(sf::Color(85, 45, 18)); // Rich dark mahogany
        shelfBody.setOutlineColor(sf::Color(50, 25, 10));
        shelfBody.setOutlineThickness(3.f);
        window.draw(shelfBody);

        // A catalog match keeps its shelf easy to find after the terminal closes.
        if (s.id == highlightedShelfId) {
            sf::RectangleShape highlight(sf::Vector2f(s.width + 10.f, s.height + 30.f));
            highlight.setPosition(sf::Vector2f(s.x - 5.f, s.y - 23.f));
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(4.f);
            window.draw(highlight);
        }

        // Bookshelf Rows (Simulating 3 shelves with colorful book spines)
        for (int row = 0; row < 3; ++row) {
            float rowY = s.y + 12.f + row * 20.f;
            sf::RectangleShape shelfDivider(sf::Vector2f(s.width - 8.f, 3.f));
            shelfDivider.setPosition(sf::Vector2f(s.x + 4.f, rowY + 14.f));
            shelfDivider.setFillColor(sf::Color(60, 30, 10));
            window.draw(shelfDivider);

            // Spines
            for (float bx = s.x + 8.f; bx < s.x + s.width - 12.f; bx += 9.f) {
                sf::RectangleShape spine(sf::Vector2f(7.f, 13.f));
                spine.setPosition(sf::Vector2f(bx, rowY));
                int hashVal = static_cast<int>(bx * 13 + row * 7);
                sf::Color spineCol;
                switch (hashVal % 5) {
                    case 0: spineCol = sf::Color(180, 40, 40); break;   // Crimson
                    case 1: spineCol = sf::Color(40, 100, 170); break;  // Deep Blue
                    case 2: spineCol = sf::Color(40, 140, 60); break;   // Forest Green
                    case 3: spineCol = sf::Color(200, 160, 30); break;  // Gold
                    default: spineCol = sf::Color(130, 60, 140); break; // Purple
                }
                spine.setFillColor(spineCol);
                window.draw(spine);
            }
        }

        // Shelf Signboard on top
        sf::RectangleShape signPlate(sf::Vector2f(s.width - 10.f, 16.f));
        signPlate.setPosition(sf::Vector2f(s.x + 5.f, s.y - 18.f));
        signPlate.setFillColor(sf::Color(35, 35, 35));
        signPlate.setOutlineColor(sf::Color(220, 180, 80));
        signPlate.setOutlineThickness(1.5f);
        window.draw(signPlate);

        sf::Text signText(font, s.category, 10);
        signText.setFillColor(sf::Color::White);
        signText.setStyle(sf::Text::Bold);
        signText.setPosition(sf::Vector2f(s.x + 12.f, s.y - 17.f));
        window.draw(signText);

        // Show real catalog titles beneath each shelf.
        sf::RectangleShape titlePlate(sf::Vector2f(s.width, 48.f));
        titlePlate.setPosition(sf::Vector2f(s.x, s.y + s.height + 5.f));
        titlePlate.setFillColor(sf::Color(25, 30, 38, 235));
        titlePlate.setOutlineColor(sf::Color(190, 160, 95));
        titlePlate.setOutlineThickness(1.f);
        window.draw(titlePlate);

        sf::Text booksHeading(font, "BOOK TITLES", 8);
        booksHeading.setFillColor(sf::Color(255, 220, 130));
        booksHeading.setStyle(sf::Text::Bold);
        booksHeading.setPosition(sf::Vector2f(s.x + 5.f, s.y + s.height + 7.f));
        window.draw(booksHeading);

        int shown = 0;
        int totalOnShelf = 0;
        for (const auto& book : catalog.getBooks()) {
            if (book.shelfId != s.id) continue;
            ++totalOnShelf;
            if (shown >= 3) continue;

            std::string bookTitle = book.title;
            const std::size_t maxChars = 23;
            if (bookTitle.size() > maxChars) bookTitle = bookTitle.substr(0, maxChars - 3) + "...";

            sf::Text titleLine(font, bookTitle, 7);
            titleLine.setFillColor(sf::Color::White);
            titleLine.setPosition(sf::Vector2f(s.x + 5.f, s.y + s.height + 17.f + shown * 9.f));
            window.draw(titleLine);
            ++shown;
        }

        if (totalOnShelf > 3) {
            sf::Text moreLine(font, "+" + std::to_string(totalOnShelf - 3) + " more - TAB", 7);
            moreLine.setFillColor(sf::Color(150, 220, 255));
            moreLine.setPosition(sf::Vector2f(s.x + 5.f, s.y + s.height + 43.f));
            window.draw(moreLine);
        }

        if (s.id == highlightedShelfId) {
            sf::Text foundText(font, "SEARCH RESULT - GO HERE", 10);
            foundText.setFillColor(sf::Color::Yellow);
            foundText.setStyle(sf::Text::Bold);
            foundText.setPosition(sf::Vector2f(s.x, s.y + s.height + 5.f));
            window.draw(foundText);
        }
    }

    // 4. Reading Study Desks with Lamps
    auto drawStudyDesk = [&](float x, float y, float w, float h) {
        sf::RectangleShape desk(sf::Vector2f(w, h));
        desk.setPosition(sf::Vector2f(x, y));
        desk.setFillColor(sf::Color(115, 65, 30));
        desk.setOutlineColor(sf::Color(70, 35, 15));
        desk.setOutlineThickness(2.f);
        window.draw(desk);

        // Desk Lamp
        sf::CircleShape lampBase(6.f);
        lampBase.setPosition(sf::Vector2f(x + w / 2.f - 6.f, y + 8.f));
        lampBase.setFillColor(sf::Color(40, 140, 80)); // Green banker's lamp
        window.draw(lampBase);

        // Open book on desk
        sf::RectangleShape openBook(sf::Vector2f(22.f, 15.f));
        openBook.setPosition(sf::Vector2f(x + 20.f, y + 25.f));
        openBook.setFillColor(sf::Color(240, 235, 220));
        window.draw(openBook);
    };

    drawStudyDesk(75.f, 290.f, 110.f, 65.f);
    drawStudyDesk(615.f, 290.f, 110.f, 65.f);

    // 5. Computer Catalog Terminal Desk (Right Wall)
    sf::RectangleShape compDesk(sf::Vector2f(computerDeskBounds.size.x, computerDeskBounds.size.y));
    compDesk.setPosition(computerDeskBounds.position);
    compDesk.setFillColor(sf::Color(80, 80, 90));
    compDesk.setOutlineColor(sf::Color(40, 40, 50));
    compDesk.setOutlineThickness(2.f);
    window.draw(compDesk);

    // Monitor screen
    sf::RectangleShape monitor(sf::Vector2f(34.f, 24.f));
    monitor.setPosition(sf::Vector2f(computerDeskBounds.position.x + 20.f, computerDeskBounds.position.y + 10.f));
    monitor.setFillColor(sf::Color(20, 40, 70));
    monitor.setOutlineColor(sf::Color(180, 200, 220));
    monitor.setOutlineThickness(2.f);
    window.draw(monitor);

    sf::Text compLabel(font, "[TAB] TERMINAL", 9);
    compLabel.setFillColor(sf::Color::Cyan);
    compLabel.setPosition(sf::Vector2f(computerDeskBounds.position.x - 5.f, computerDeskBounds.position.y - 14.f));
    window.draw(compLabel);

    // 6. Central Checkout Service Counter
    sf::RectangleShape counter(sf::Vector2f(counterBounds.size.x, counterBounds.size.y));
    counter.setPosition(counterBounds.position);
    counter.setFillColor(sf::Color(120, 60, 25));
    counter.setOutlineColor(sf::Color(70, 35, 10));
    counter.setOutlineThickness(3.f);
    window.draw(counter);

    // Counter Top Trim & Cash Register / Loan Ledger
    sf::RectangleShape counterTop(sf::Vector2f(counterBounds.size.x, 12.f));
    counterTop.setPosition(counterBounds.position);
    counterTop.setFillColor(sf::Color(150, 80, 35));
    window.draw(counterTop);

    // Register / Stamp Station
    sf::RectangleShape reg(sf::Vector2f(30.f, 24.f));
    reg.setPosition(sf::Vector2f(counterBounds.position.x + 35.f, counterBounds.position.y + 15.f));
    reg.setFillColor(sf::Color(50, 50, 60));
    window.draw(reg);

    sf::Text counterSign(font, "CHECKOUT & LOAN DESK", 11);
    counterSign.setFillColor(sf::Color(255, 230, 160));
    counterSign.setStyle(sf::Text::Bold);
    counterSign.setPosition(sf::Vector2f(counterBounds.position.x + 85.f, counterBounds.position.y + 18.f));
    window.draw(counterSign);
}
