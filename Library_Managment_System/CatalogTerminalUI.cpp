#include "CatalogTerminalUI.hpp"
#include <iostream>
#include <sstream>

CatalogTerminalUI::CatalogTerminalUI() {
}

void CatalogTerminalUI::executeISBNSearch(Catalog& catalog) {
    hasSearched = true;
    int targetIsbn = 0;
    try {
        if (!isbnInput.empty()) {
            targetIsbn = std::stoi(isbnInput);
        }
    } catch (...) {
        targetIsbn = 0;
    }

    lastResult = catalog.searchByISBN(targetIsbn);

    std::cout << "\n[SFML CATALOG TERMINAL - LINEAR ISBN SEARCH]\n";
    std::cout << "Target ISBN: " << targetIsbn << "\n";
    if (lastResult.found) {
        std::cout << "Search Status: MATCH FOUND (Executed using linear search)\n";
        std::cout << "Title   : " << lastResult.matchedBook.title << "\n";
        std::cout << "Author  : " << lastResult.matchedBook.author << "\n";
        std::cout << "Location: " << lastResult.matchedBook.location
                  << " [Grid: X=" << lastResult.matchedBook.gridX << ", Y=" << lastResult.matchedBook.gridY << "]\n";
        std::cout << "In Stock: " << lastResult.matchedBook.copies << " Copies\n\n";
    } else {
        std::cout << "Search Status: NOT FOUND (Executed in " << lastResult.operationsCount << " operations)\n\n";
    }
}

void CatalogTerminalUI::handleEvent(const sf::Event& event, Catalog& catalog) {
    if (!isOpen) return;

    // Mouse Clicks
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));

            if (closeBtnBounds.contains(clickPos)) {
                close();
                return;
            }
            if (isbnBoxBounds.contains(clickPos)) {
                activeInputBox = 0;
            }
            else if (binaryBtnBounds.contains(clickPos)) {
                executeISBNSearch(catalog);
            }
        }
    }

    // Text typing input
    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = textEntered->unicode;

        if (unicode == 8) { // Backspace
            if (activeInputBox == 0 && !isbnInput.empty()) {
                isbnInput.pop_back();
            } else if (activeInputBox == 1 && !titleInput.empty()) {
                titleInput.pop_back();
            }
        }
        else if (unicode == 13) { // Enter key
            executeISBNSearch(catalog);
        }
        else if (unicode >= 32 && unicode < 127) {
            char ch = static_cast<char>(unicode);
            if (activeInputBox == 0 && isbnInput.size() < 10) {
                if (std::isdigit(static_cast<unsigned char>(ch))) {
                    isbnInput.push_back(ch);
                }
            }
        }
    }

    // Keyboard Shortcuts
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            close();
        }
    }
}

void CatalogTerminalUI::render(sf::RenderWindow& window, const sf::Font& font) {
    if (!isOpen) return;

    // Main Modal Box
    sf::RectangleShape modal(sf::Vector2f(modalBounds.size.x, modalBounds.size.y));
    modal.setPosition(modalBounds.position);
    modal.setFillColor(sf::Color(230, 235, 245, 245));
    modal.setOutlineColor(sf::Color(60, 90, 140));
    modal.setOutlineThickness(3.f);
    window.draw(modal);

    // Modal Header Bar
    sf::RectangleShape header(sf::Vector2f(modalBounds.size.x, 28.f));
    header.setPosition(modalBounds.position);
    header.setFillColor(sf::Color(50, 85, 140));
    window.draw(header);

    sf::Text titleText(font, "CATALOG SEARCH", 12);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(sf::Vector2f(modalBounds.position.x + 12.f, modalBounds.position.y + 6.f));
    window.draw(titleText);

    // Close button [X]
    sf::RectangleShape closeBtn(sf::Vector2f(closeBtnBounds.size.x, closeBtnBounds.size.y));
    closeBtn.setPosition(closeBtnBounds.position);
    closeBtn.setFillColor(sf::Color(190, 50, 50));
    window.draw(closeBtn);

    sf::Text xText(font, "X", 11);
    xText.setFillColor(sf::Color::White);
    xText.setStyle(sf::Text::Bold);
    xText.setPosition(sf::Vector2f(closeBtnBounds.position.x + 6.f, closeBtnBounds.position.y + 3.f));
    window.draw(xText);

    // Input labels
    sf::Text isbnLabel(font, "ISBN:", 11);
    isbnLabel.setFillColor(sf::Color(30, 30, 30));
    isbnLabel.setStyle(sf::Text::Bold);
    isbnLabel.setPosition(sf::Vector2f(modalBounds.position.x + 12.f, isbnBoxBounds.position.y + 3.f));
    window.draw(isbnLabel);

    // ISBN Input Box
    sf::RectangleShape isbnBox(sf::Vector2f(isbnBoxBounds.size.x, isbnBoxBounds.size.y));
    isbnBox.setPosition(isbnBoxBounds.position);
    isbnBox.setFillColor(sf::Color::White);
    isbnBox.setOutlineColor(activeInputBox == 0 ? sf::Color(30, 100, 220) : sf::Color(160, 160, 160));
    isbnBox.setOutlineThickness(activeInputBox == 0 ? 2.f : 1.f);
    window.draw(isbnBox);

    sf::Text isbnVal(font, isbnInput + (activeInputBox == 0 ? "_" : ""), 11);
    isbnVal.setFillColor(sf::Color::Black);
    isbnVal.setPosition(sf::Vector2f(isbnBoxBounds.position.x + 6.f, isbnBoxBounds.position.y + 3.f));
    window.draw(isbnVal);

    // Single ISBN linear-search button
    sf::RectangleShape binBtn(sf::Vector2f(binaryBtnBounds.size.x, binaryBtnBounds.size.y));
    binBtn.setPosition(binaryBtnBounds.position);
    binBtn.setFillColor(sf::Color(70, 120, 180));
    binBtn.setOutlineColor(sf::Color(40, 70, 120));
    binBtn.setOutlineThickness(1.5f);
    window.draw(binBtn);

    sf::Text binText(font, "SEARCH ISBN (LINEAR SEARCH)", 10);
    binText.setFillColor(sf::Color::White);
    binText.setStyle(sf::Text::Bold);
    binText.setPosition(sf::Vector2f(binaryBtnBounds.position.x + 20.f, binaryBtnBounds.position.y + 7.f));
    window.draw(binText);

    // Results Box
    sf::RectangleShape resBox(sf::Vector2f(modalBounds.size.x - 24.f, 90.f));
    resBox.setPosition(sf::Vector2f(modalBounds.position.x + 12.f, 362.f));
    resBox.setFillColor(sf::Color(25, 30, 40));
    resBox.setOutlineColor(sf::Color(100, 120, 150));
    resBox.setOutlineThickness(1.5f);
    window.draw(resBox);

    if (hasSearched) {
        if (lastResult.found) {
            std::string statusLine = "STATUS: MATCH FOUND (O(N)) - " +
                std::to_string(lastResult.operationsCount) + " checks";

            sf::Text stText(font, statusLine, 9);
            stText.setFillColor(sf::Color(80, 240, 120));
            stText.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 368.f));
            window.draw(stText);

            std::string t = lastResult.matchedBook.title;
            if (t.size() > 32) t = t.substr(0, 30) + "..";
            sf::Text bTitle(font, "Title: " + t, 9);
            bTitle.setFillColor(sf::Color::White);
            bTitle.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 384.f));
            window.draw(bTitle);

            sf::Text bLoc(font, "Loc: " + std::string(lastResult.matchedBook.location) +
                                " [Shelf #" + std::to_string(lastResult.matchedBook.shelfId) + "]", 9);
            bLoc.setFillColor(sf::Color(240, 200, 80));
            bLoc.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 400.f));
            window.draw(bLoc);

            sf::Text bStock(font, "In Stock: " + std::to_string(lastResult.matchedBook.copies) + " Copies", 9);
            bStock.setFillColor(sf::Color::Cyan);
            bStock.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 416.f));
            window.draw(bStock);
        } else {
            sf::Text notFound(font, "STATUS: NO MATCH FOUND\nExecuted in " + std::to_string(lastResult.operationsCount) + " operations.", 10);
            notFound.setFillColor(sf::Color(255, 100, 100));
            notFound.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 380.f));
            window.draw(notFound);
        }
    } else {
        sf::Text prompt(font, "Enter the patron's ISBN number above,\nthen click SEARCH ISBN.", 9);
        prompt.setFillColor(sf::Color(160, 170, 190));
        prompt.setPosition(sf::Vector2f(modalBounds.position.x + 18.f, 385.f));
        window.draw(prompt);
    }
}
