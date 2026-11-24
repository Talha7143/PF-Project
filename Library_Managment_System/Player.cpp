#include "Player.hpp"
#include "Config.hpp"
#include <cstring>
#include <cmath>
#include <algorithm>

PlayerEntity::PlayerEntity() {
    strncpy_s(data.name, "Talha", sizeof(data.name));
    data.position = Vector2D(400.0f, 320.0f);
    data.speed = Config::PLAYER_DEFAULT_SPEED;
    data.score = 1250;
    data.day = 4;
    data.timeOfDay = 10.75f; // 10:45 AM
    data.patronsServed = 14;
    data.patronsLost = 1;
    // Keep the beginner inventory simple: carry one searched book at a time.
    data.booksCapacity = 1;
    data.booksHeld = 0;
    data.isCarryingBook = false;
    data.carriedBookIsbn = 0;
    data.carriedBookTitle[0] = '\0';
}

sf::FloatRect PlayerEntity::getBounds() const {
    return sf::FloatRect({ data.position.x - 14.f, data.position.y - 18.f }, { 28.f, 36.f });
}

void PlayerEntity::update(float dt, const LibraryMap& map) {
    sf::Vector2f moveVec(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        moveVec.y -= 1.0f;
        data.facing = 1; // Up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        moveVec.y += 1.0f;
        data.facing = 0; // Down
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        moveVec.x -= 1.0f;
        data.facing = 2; // Left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        moveVec.x += 1.0f;
        data.facing = 3; // Right
    }

    // Normalize diagonal velocity
    if (moveVec.x != 0.f || moveVec.y != 0.f) {
        float len = std::sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec /= len;

        animTimer += dt;
        if (animTimer > 0.15f) {
            animFrame = (animFrame + 1) % 4;
            animTimer = 0.0f;
        }

        const float deltaDist = data.speed * (dt * 60.0f);

        // Move in small steps so a fast player cannot skip through furniture.
        const int moveSteps = std::max(1, static_cast<int>(std::ceil(deltaDist / 3.0f)));
        const float stepDistance = deltaDist / static_cast<float>(moveSteps);
        for (int step = 0; step < moveSteps; ++step) {
            sf::FloatRect newBoundsX(
                { (data.position.x + moveVec.x * stepDistance) - 14.f, data.position.y - 18.f },
                { 28.f, 36.f });
            if (!map.checkCollision(newBoundsX)) {
                data.position.x += moveVec.x * stepDistance;
            }

            sf::FloatRect newBoundsY(
                { data.position.x - 14.f, (data.position.y + moveVec.y * stepDistance) - 18.f },
                { 28.f, 36.f });
            if (!map.checkCollision(newBoundsY)) {
                data.position.y += moveVec.y * stepDistance;
            }
        }
    } else {
        animFrame = 0;
    }

    // Keep within world bounds
    if (data.position.x < 25.f) data.position.x = 25.f;
    if (data.position.x > 775.f) data.position.x = 775.f;
    if (data.position.y < 30.f) data.position.y = 30.f;
    if (data.position.y > 565.f) data.position.y = 565.f;

    // Advance time slightly
    data.timeOfDay += dt * 0.005f;
    if (data.timeOfDay >= 18.0f) { // 6:00 PM closing
        data.timeOfDay = 9.0f;     // Next day 9:00 AM
        data.day++;
    }
}

bool PlayerEntity::pickUpBook(int isbn, const char* title, int shelfOrigin) {
    if (data.booksHeld < data.booksCapacity) {
        data.isCarryingBook = true;
        data.carriedBookIsbn = isbn;
        strncpy_s(data.carriedBookTitle, title ? title : "Book", sizeof(data.carriedBookTitle));
        data.carriedShelfOrigin = shelfOrigin;
        data.booksHeld++;
        return true;
    }
    return false;
}

bool PlayerEntity::dropOrIssueBook() {
    if (data.isCarryingBook && data.booksHeld > 0) {
        data.booksHeld--;
        if (data.booksHeld == 0) {
            data.isCarryingBook = false;
            data.carriedBookIsbn = 0;
            data.carriedBookTitle[0] = '\0';
        }
        return true;
    }
    return false;
}

void PlayerEntity::addScore(int points) {
    data.score += points;
    if (data.score < 0) data.score = 0;
}

void PlayerEntity::incrementServed() {
    data.patronsServed++;
}

void PlayerEntity::incrementLost() {
    data.patronsLost++;
}

void PlayerEntity::render(sf::RenderWindow& window, const sf::Font& font) {
    float px = data.position.x;
    float py = data.position.y;

    // Shadow
    sf::CircleShape shadow(14.f);
    shadow.setScale(sf::Vector2f(1.0f, 0.5f));
    shadow.setFillColor(sf::Color(0, 0, 0, 70));
    shadow.setPosition(sf::Vector2f(px - 14.f, py + 10.f));
    window.draw(shadow);

    // Body (Librarian Robe / Blue Apron matching proposal screenshot)
    sf::RectangleShape body(sf::Vector2f(24.f, 26.f));
    body.setFillColor(sf::Color(45, 95, 175)); // Librarian blue
    body.setOutlineColor(sf::Color(25, 60, 120));
    body.setOutlineThickness(2.f);
    body.setPosition(sf::Vector2f(px - 12.f, py - 10.f));
    window.draw(body);

    // Apron tie / collar
    sf::RectangleShape apronTrim(sf::Vector2f(10.f, 16.f));
    apronTrim.setFillColor(sf::Color(230, 230, 240));
    apronTrim.setPosition(sf::Vector2f(px - 5.f, py - 8.f));
    window.draw(apronTrim);

    // Head
    sf::CircleShape head(9.f);
    head.setFillColor(sf::Color(240, 200, 160)); // Skin tone
    head.setOutlineColor(sf::Color(180, 140, 100));
    head.setOutlineThickness(1.5f);
    head.setPosition(sf::Vector2f(px - 9.f, py - 24.f));
    window.draw(head);

    // Hair
    sf::RectangleShape hair(sf::Vector2f(18.f, 7.f));
    hair.setFillColor(sf::Color(60, 40, 25)); // Dark brown hair
    hair.setPosition(sf::Vector2f(px - 9.f, py - 25.f));
    window.draw(hair);

    // If carrying a book, draw book in arms
    if (data.isCarryingBook) {
        sf::RectangleShape bookBadge(sf::Vector2f(14.f, 18.f));
        bookBadge.setFillColor(sf::Color(200, 40, 40)); // Red book cover
        bookBadge.setOutlineColor(sf::Color::White);
        bookBadge.setOutlineThickness(1.f);
        bookBadge.setPosition(sf::Vector2f(px + 4.f, py - 6.f));
        window.draw(bookBadge);

        // Book pages strip
        sf::RectangleShape pages(sf::Vector2f(3.f, 16.f));
        pages.setFillColor(sf::Color(245, 245, 230));
        pages.setPosition(sf::Vector2f(px + 14.f, py - 5.f));
        window.draw(pages);
    }

    // Name badge overhead
    sf::Text nameTag(font, data.name, 10);
    nameTag.setFillColor(sf::Color::Yellow);
    nameTag.setStyle(sf::Text::Bold);
    nameTag.setPosition(sf::Vector2f(px - 14.f, py - 38.f));
    window.draw(nameTag);
}
