#include "PatronManager.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

PatronManager::PatronManager() {
}

void PatronManager::initialize(Catalog& catalog) {
    patrons.clear();

    for (int i = 0; i < 4; ++i) {
        spawnNewPatron(catalog);
    }
}

void PatronManager::spawnNewPatron(Catalog& catalog) {
    if (patrons.size() >= 8) return;

    Patron p;
    p.id = nextPatronId++;

    const char* names[] = { "Ahmad", "Bilal", "Zainab", "Fatima", "Hamza", "Ayesha", "Usman", "Sarah" };
    strncpy_s(p.name, names[p.id % 8], sizeof(p.name));

    p.position = Vector2D(370.0f + static_cast<float>(rand() % 60), 555.0f);
    p.velocity = Vector2D(0.0f, 0.0f);
    p.state = PatronState::ENTERING;
    p.served = false;
    // Give beginner players more time to search, collect, and issue a book.
    p.maxPatience = 150.0f + static_cast<float>(rand() % 61);
    p.patience = p.maxPatience;

    p.colorR = static_cast<uint8_t>(100 + rand() % 140);
    p.colorG = static_cast<uint8_t>(80 + rand() % 140);
    p.colorB = static_cast<uint8_t>(120 + rand() % 130);

    const auto& books = catalog.getBooks();
    if (!books.empty()) {
        int idx = rand() % books.size();
        p.requestedIsbn = books[idx].isbn;
        strncpy_s(p.requestedTitle, books[idx].title, sizeof(p.requestedTitle));
    } else {
        p.requestedIsbn = 1004;
        strncpy_s(p.requestedTitle, "Computer Systems", sizeof(p.requestedTitle));
    }

    patrons.push_back(p);
}

void PatronManager::updatePatronAI(Patron& p, float dt, const LibraryMap& map) {
    const float walkSpeed = 2.0f;

    switch (p.state) {
        case PatronState::ENTERING: {

            Vector2D queueSlot(320.0f, 410.0f - p.queueIndex * 52.0f);
            if (p.queueIndex < 0) queueSlot = Vector2D(320.0f, 410.0f);

            float dx = queueSlot.x - p.position.x;
            float dy = queueSlot.y - p.position.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > 5.0f) {
                p.position.x += (dx / dist) * walkSpeed;
                p.position.y += (dy / dist) * walkSpeed;
            } else {
                p.position = queueSlot;
                p.state = (p.queueIndex == 0) ? PatronState::WAITING_AT_COUNTER : PatronState::IN_QUEUE;
            }
            break;
        }

        case PatronState::IN_QUEUE: {

            p.patience -= dt * 1.0f;

            Vector2D queueSlot(320.0f, 410.0f - p.queueIndex * 52.0f);
            float dx = queueSlot.x - p.position.x;
            float dy = queueSlot.y - p.position.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > 4.0f) {
                p.position.x += (dx / dist) * walkSpeed;
                p.position.y += (dy / dist) * walkSpeed;
            } else {
                p.position = queueSlot;
                if (p.queueIndex == 0) {
                    p.state = PatronState::WAITING_AT_COUNTER;
                }
            }
            break;
        }

        case PatronState::WAITING_AT_COUNTER: {

            p.position = Vector2D(320.0f, 410.0f);
            p.patience -= dt * 1.5f;
            break;
        }

        case PatronState::LEAVING_SATISFIED: {

            float dx = 400.0f - p.position.x;
            float dy = 570.0f - p.position.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > 6.0f) {
                p.position.x += (dx / dist) * (walkSpeed * 1.4f);
                p.position.y += (dy / dist) * (walkSpeed * 1.4f);
            }
            break;
        }

        case PatronState::LEAVING_ANGRY: {

            float dx = 400.0f - p.position.x;
            float dy = 570.0f - p.position.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > 6.0f) {
                p.position.x += (dx / dist) * (walkSpeed * 1.8f);
                p.position.y += (dy / dist) * (walkSpeed * 1.8f);
            }
            break;
        }

        case PatronState::BROWSING: {

            break;
        }
    }
}

void PatronManager::update(float dt, Catalog& catalog, const LibraryMap& map, int& scoreDelta, int& servedDelta, int& lostDelta) {
    scoreDelta = 0;
    servedDelta = 0;
    lostDelta = 0;

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        spawnNewPatron(catalog);
    }

    int qIdx = 0;
    for (auto& p : patrons) {
        if (p.state == PatronState::ENTERING || p.state == PatronState::IN_QUEUE || p.state == PatronState::WAITING_AT_COUNTER) {
            p.queueIndex = qIdx++;
        } else {
            p.queueIndex = -1;
        }
    }

    for (auto it = patrons.begin(); it != patrons.end();) {
        updatePatronAI(*it, dt, map);

        if ((it->state == PatronState::IN_QUEUE || it->state == PatronState::WAITING_AT_COUNTER) && it->patience <= 0.0f) {
            it->state = PatronState::LEAVING_ANGRY;
            scoreDelta -= 50;
            lostDelta++;
            std::cout << "[PATRON DISAPPOINTED]: " << it->name << " left angry. Patience expired for ISBN: " << it->requestedIsbn << "!\n";
        }

        if ((it->state == PatronState::LEAVING_SATISFIED || it->state == PatronState::LEAVING_ANGRY) && it->position.y >= 565.0f) {
            it = patrons.erase(it);
        } else {
            ++it;
        }
    }
}

Patron* PatronManager::getHeadOfQueue() {
    for (auto& p : patrons) {
        if (p.state == PatronState::WAITING_AT_COUNTER || (p.queueIndex == 0 && p.state != PatronState::LEAVING_SATISFIED && p.state != PatronState::LEAVING_ANGRY)) {
            return &p;
        }
    }
    return nullptr;
}

bool PatronManager::serveHeadPatron(int carriedIsbn, Catalog& catalog, std::string& feedbackMsg) {
    Patron* p = getHeadOfQueue();
    if (!p) {
        feedbackMsg = "No patron is waiting at the counter!";
        return false;
    }

    if (carriedIsbn == p->requestedIsbn) {
        p->served = true;
        p->state = PatronState::LEAVING_SATISFIED;
        feedbackMsg = "SUCCESS! Issued '" + std::string(p->requestedTitle) + "' to " + std::string(p->name) + " (+150 Pts)";
        return true;
    } else {
        feedbackMsg = "WRONG BOOK! Patron requested ISBN: " + std::to_string(p->requestedIsbn);
        return false;
    }
}

int PatronManager::getWaitingPatronCount() const {
    int count = 0;
    for (const auto& p : patrons) {
        if (p.state == PatronState::IN_QUEUE || p.state == PatronState::WAITING_AT_COUNTER || p.state == PatronState::ENTERING) {
            count++;
        }
    }
    return count;
}

void PatronManager::render(sf::RenderWindow& window, const sf::Font& font) {
    for (const auto& p : patrons) {
        float px = p.position.x;
        float py = p.position.y;

        sf::CircleShape shadow(12.f);
        shadow.setScale(sf::Vector2f(1.0f, 0.45f));
        shadow.setFillColor(sf::Color(0, 0, 0, 60));
        shadow.setPosition(sf::Vector2f(px - 12.f, py + 8.f));
        window.draw(shadow);

        sf::RectangleShape body(sf::Vector2f(20.f, 24.f));
        body.setFillColor(sf::Color(p.colorR, p.colorG, p.colorB));
        body.setOutlineColor(sf::Color(40, 40, 40));
        body.setOutlineThickness(1.5f);
        body.setPosition(sf::Vector2f(px - 10.f, py - 10.f));
        window.draw(body);

        sf::CircleShape head(8.f);
        head.setFillColor(sf::Color(235, 195, 155));
        head.setOutlineColor(sf::Color(140, 100, 70));
        head.setOutlineThickness(1.f);
        head.setPosition(sf::Vector2f(px - 8.f, py - 23.f));
        window.draw(head);

        sf::Text nameText(font, p.name, 9);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(sf::Vector2f(px - 12.f, py - 32.f));
        window.draw(nameText);

        if (p.state == PatronState::IN_QUEUE || p.state == PatronState::WAITING_AT_COUNTER) {
            float barW = 32.f;
            float barH = 4.f;
            float pct = std::max(0.0f, std::min(1.0f, p.patience / p.maxPatience));

            sf::RectangleShape barBg(sf::Vector2f(barW, barH));
            barBg.setPosition(sf::Vector2f(px - barW / 2.f, py - 37.f));
            barBg.setFillColor(sf::Color(50, 50, 50));
            barBg.setOutlineColor(sf::Color::Black);
            barBg.setOutlineThickness(1.f);
            window.draw(barBg);

            sf::RectangleShape barFill(sf::Vector2f(barW * pct, barH));
            barFill.setPosition(sf::Vector2f(px - barW / 2.f, py - 37.f));
            if (pct > 0.5f) barFill.setFillColor(sf::Color(50, 220, 50));
            else if (pct > 0.25f) barFill.setFillColor(sf::Color(240, 200, 30));
            else barFill.setFillColor(sf::Color(230, 40, 40));
            window.draw(barFill);

            sf::RectangleShape bubble(sf::Vector2f(180.f, 42.f));
            bubble.setPosition(sf::Vector2f(px + 16.f, py - 42.f));
            bubble.setFillColor(sf::Color(255, 255, 255, 240));
            bubble.setOutlineColor(sf::Color(30, 30, 30));
            bubble.setOutlineThickness(1.5f);
            window.draw(bubble);

            sf::RectangleShape bookIcon(sf::Vector2f(10.f, 14.f));
            bookIcon.setPosition(sf::Vector2f(px + 22.f, py - 29.f));
            bookIcon.setFillColor(sf::Color(200, 30, 30));
            window.draw(bookIcon);

            std::string shortTitle = p.requestedTitle;
            if (shortTitle.size() > 20) shortTitle = shortTitle.substr(0, 18) + "..";
            std::string reqMsg = "FIND: \"" + shortTitle + "\"\nISBN: " + std::to_string(p.requestedIsbn);
            sf::Text bubbleText(font, reqMsg, 11);
            bubbleText.setFillColor(sf::Color(20, 20, 20));
            bubbleText.setStyle(sf::Text::Bold);
            bubbleText.setPosition(sf::Vector2f(px + 38.f, py - 39.f));
            window.draw(bubbleText);
        }
    }
}
