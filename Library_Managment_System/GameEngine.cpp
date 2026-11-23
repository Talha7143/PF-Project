#include "GameEngine.hpp"
#include <iostream>
#include <iomanip>
 #include <sstream>

GameEngine::GameEngine()
    : window(sf::VideoMode({ 800, 600 }), "Bibliotheca Quest: The Ultimate Library Simulation & Management Game")
{
    window.setFramerateLimit(60);
}

GameEngine::~GameEngine() {

    BinaryFileManager::saveCatalog(catalog.getBooks());
}

bool GameEngine::initialize() {

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {
            std::cerr << "[WARNING]: Failed to load system font. UI text might not render.\n";
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    } else {
        fontLoaded = true;
    }

    std::vector<Book> loadedBooks;
    if (BinaryFileManager::loadCatalog(loadedBooks) && !loadedBooks.empty()) {
        catalog.setBooks(loadedBooks);
    } else {
        catalog.initializeDefaultCatalog();
        BinaryFileManager::saveCatalog(catalog.getBooks());
    }

    map.initialize();
    patronMgr.initialize(catalog);
    BinaryFileManager::loadHighScores(highScores);

    std::cout << "[SYSTEM]: Bibliotheca Quest Engine initialized successfully.\n";
    return true;
}

void GameEngine::showNotification(const std::string& msg, float duration) {
    notificationMsg = msg;
    notificationTimer = duration;
}

void GameEngine::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        if (notificationTimer > 0.0f) {
            notificationTimer -= dt;
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                BinaryFileManager::saveCatalog(catalog.getBooks());
                BinaryFileManager::saveGameState(player.getData(), patronMgr.getPatrons());
                window.close();
            }

            switch (currentState) {
                case GameState::MainMenuState:
                    handleMainMenuInput(*event);
                    break;
                case GameState::GameplayState:
                case GameState::CatalogUIState:
                    handleGameplayInput(*event);
                    break;
                case GameState::PauseState:
                    handlePauseInput(*event);
                    break;
                case GameState::GameOverState:
                    handleGameOverInput(*event);
                    break;
                case GameState::LeaderboardState:
                    handleLeaderboardInput(*event);
                    break;
                case GameState::HowToPlayState:
                    handleHowToPlayInput(*event);
                    break;
            }
        }

        if (currentState == GameState::GameplayState || currentState == GameState::CatalogUIState) {
            player.update(dt, map);

            int scoreDelta = 0, servedDelta = 0, lostDelta = 0;
            patronMgr.update(dt, catalog, map, scoreDelta, servedDelta, lostDelta);

            if (scoreDelta != 0) player.addScore(scoreDelta);
            if (servedDelta != 0) player.incrementServed();
            if (lostDelta != 0) {
                player.incrementLost();
                showNotification("PATRON LOST! Impatience penalty applied (-50 pts)");
                if (player.getData().patronsLost >= 5) {
                    currentState = GameState::GameOverState;
                }
            }
        }

        window.clear(sf::Color(215, 200, 175));

        switch (currentState) {
            case GameState::MainMenuState:
                renderMainMenu();
                break;
            case GameState::GameplayState:
            case GameState::CatalogUIState:
                renderGameplay();
                break;
            case GameState::PauseState:
                renderGameplay();
                renderPauseMenu();
                break;
            case GameState::GameOverState:
                renderGameOver();
                break;
            case GameState::LeaderboardState:
                renderLeaderboard();
                break;
            case GameState::HowToPlayState:
                renderHowToPlay();
                break;
        }

        window.display();
    }
}

void GameEngine::handleMainMenuInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::W || keyPressed->scancode == sf::Keyboard::Scan::Up) {
            selectedMenuIndex = (selectedMenuIndex - 1 + static_cast<int>(menuOptions.size())) % menuOptions.size();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::S || keyPressed->scancode == sf::Keyboard::Scan::Down) {
            selectedMenuIndex = (selectedMenuIndex + 1) % menuOptions.size();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Enter || keyPressed->scancode == sf::Keyboard::Scan::Space) {
            if (selectedMenuIndex == 0) {
                currentState = GameState::GameplayState;
            }
            else if (selectedMenuIndex == 1) {
                Player pData = player.getData();
                std::vector<Patron> pQueue;
                if (BinaryFileManager::loadGameState(pData, pQueue)) {
                    player.setData(pData);
                    if (!pQueue.empty()) patronMgr.setPatrons(pQueue);
                    showNotification("Game state successfully loaded from savegame.bin!");
                    currentState = GameState::GameplayState;
                } else {
                    showNotification("No savegame.bin file found!");
                }
            }
            else if (selectedMenuIndex == 2) {
                BinaryFileManager::loadHighScores(highScores);
                currentState = GameState::LeaderboardState;
            }
            else if (selectedMenuIndex == 3) {
                currentState = GameState::HowToPlayState;
            }
            else if (selectedMenuIndex == 4) {
                window.close();
            }
        }
    }
}

void GameEngine::handleGameplayInput(const sf::Event& event) {

    if (terminalUI.isVisible()) {
        terminalUI.handleEvent(event, catalog);
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {

        if (keyPressed->scancode == sf::Keyboard::Scan::Tab) {
            terminalUI.toggle();
            currentState = terminalUI.isVisible() ? GameState::CatalogUIState : GameState::GameplayState;
            if (terminalUI.isVisible()) {
                terminalUI.executeBinarySearch(catalog);
            }
        }

        else if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            if (terminalUI.isVisible()) {
                terminalUI.close();
                currentState = GameState::GameplayState;
            } else {
                currentState = GameState::PauseState;
            }
        }

        else if (keyPressed->scancode == sf::Keyboard::Scan::E || keyPressed->scancode == sf::Keyboard::Scan::Space) {
            sf::Vector2f pPos = player.getPosition();

            if (map.isNearComputerTerminal(pPos)) {
                terminalUI.open();
                currentState = GameState::CatalogUIState;
                terminalUI.executeBinarySearch(catalog);
                return;
            }

            if (map.isNearCheckoutCounter(pPos)) {
                if (player.isCarrying()) {
                    std::string feedback;
                    if (patronMgr.serveHeadPatron(player.getCarriedISBN(), catalog, feedback)) {
                        const int issuedIsbn = player.getCarriedISBN();
                        player.dropOrIssueBook();
                        player.addScore(150);
                        player.incrementServed();
                        showNotification(feedback);

                        TransactionRecord tx;
                        tx.id = rand() % 9000 + 1000;
                        tx.isbn = issuedIsbn;
                        strncpy_s(tx.action, "LOAN_ISSUED", sizeof(tx.action));
                        tx.scoreDelta = 150;
                        BinaryFileManager::logTransaction(tx);
                    } else {
                        player.addScore(-25);
                        showNotification(feedback + " (-25 points)", 4.0f);
                    }
                } else {
                    showNotification("You are not carrying any book to issue!");
                }
                return;
            }

            int shelfId = map.getInteractingShelf(pPos);
            if (shelfId > 0) {
                auto shelfBooks = catalog.getBooksByShelf(shelfId);
                if (!shelfBooks.empty()) {
                    const auto& b = shelfBooks[0];
                    if (player.pickUpBook(b.isbn, b.title, shelfId)) {
                        catalog.borrowBook(b.isbn);
                        showNotification("COLLECTED: '" + std::string(b.title) + "' (ISBN " +
                                         std::to_string(b.isbn) + ")", 4.0f);
                    } else {
                        showNotification("Inventory full! Librarian can only hold " + std::to_string(player.getData().booksCapacity) + " books.");
                    }
                }
                return;
            }
        }
    }
}

void GameEngine::handlePauseInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::W || keyPressed->scancode == sf::Keyboard::Scan::Up) {
            selectedPauseIndex = (selectedPauseIndex - 1 + static_cast<int>(pauseOptions.size())) % pauseOptions.size();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::S || keyPressed->scancode == sf::Keyboard::Scan::Down) {
            selectedPauseIndex = (selectedPauseIndex + 1) % pauseOptions.size();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Enter || keyPressed->scancode == sf::Keyboard::Scan::Space) {
            if (selectedPauseIndex == 0) {
                currentState = GameState::GameplayState;
            }
            else if (selectedPauseIndex == 1) {
                BinaryFileManager::saveCatalog(catalog.getBooks());
                BinaryFileManager::saveGameState(player.getData(), patronMgr.getPatrons());
                showNotification("Game state successfully flushed to catalog.dat & savegame.bin!");
            }
            else if (selectedPauseIndex == 2) {
                terminalUI.open();
                currentState = GameState::CatalogUIState;
            }
            else if (selectedPauseIndex == 3) {
                currentState = GameState::MainMenuState;
            }
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            currentState = GameState::GameplayState;
        }
    }
}

void GameEngine::handleGameOverInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Enter || keyPressed->scancode == sf::Keyboard::Scan::Space) {

            HighScoreRecord rec;
            strncpy_s(rec.playerName, player.getData().name, sizeof(rec.playerName));
            rec.score = player.getData().score;
            rec.dayReached = player.getData().day;
            rec.patronsServed = player.getData().patronsServed;
            strncpy_s(rec.dateString, "2026-11-19", sizeof(rec.dateString));
            highScores.push_back(rec);
            BinaryFileManager::saveHighScores(highScores);

            player = PlayerEntity();
            patronMgr.initialize(catalog);
            currentState = GameState::MainMenuState;
        }
    }
}

void GameEngine::handleLeaderboardInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape || keyPressed->scancode == sf::Keyboard::Scan::Enter) {
            currentState = GameState::MainMenuState;
        }
    }
}

void GameEngine::handleHowToPlayInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape ||
            keyPressed->scancode == sf::Keyboard::Scan::Enter ||
            keyPressed->scancode == sf::Keyboard::Scan::Space) {
            currentState = GameState::MainMenuState;
        }
    }
}

void GameEngine::renderMainMenu() {

    sf::RectangleShape banner(sf::Vector2f(720.f, 90.f));
    banner.setPosition(sf::Vector2f(40.f, 60.f));
    banner.setFillColor(sf::Color(45, 60, 95));
    banner.setOutlineColor(sf::Color(210, 180, 80));
    banner.setOutlineThickness(3.f);
    window.draw(banner);

    sf::Text title(font, "BIBLIOTHECA QUEST", 28);
    title.setFillColor(sf::Color(255, 235, 170));
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(210.f, 75.f));
    window.draw(title);

    sf::Text subtitle(font, "The Ultimate 2D Library Simulation & Management Game", 13);
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition(sf::Vector2f(220.f, 115.f));
    window.draw(subtitle);

    for (size_t i = 0; i < menuOptions.size(); ++i) {
        sf::RectangleShape btn(sf::Vector2f(340.f, 42.f));
        btn.setPosition(sf::Vector2f(230.f, 200.f + i * 58.f));

        if (static_cast<int>(i) == selectedMenuIndex) {
            btn.setFillColor(sf::Color(70, 130, 200));
            btn.setOutlineColor(sf::Color::Yellow);
            btn.setOutlineThickness(2.5f);
        } else {
            btn.setFillColor(sf::Color(50, 60, 75));
            btn.setOutlineColor(sf::Color(100, 110, 130));
            btn.setOutlineThickness(1.5f);
        }
        window.draw(btn);

        sf::Text optText(font, menuOptions[i], 14);
        optText.setFillColor(sf::Color::White);
        optText.setStyle(sf::Text::Bold);
        optText.setPosition(sf::Vector2f(280.f, 210.f + i * 58.f));
        window.draw(optText);
    }

    sf::Text credits(font, "Developed by Muhammad Talha | Roll No: 26L-0755 | Group 15", 11);
    credits.setFillColor(sf::Color(70, 70, 70));
    credits.setPosition(sf::Vector2f(200.f, 560.f));
    window.draw(credits);
}

void GameEngine::renderGameplay() {

    map.render(window, font, catalog, terminalUI.getHighlightedShelfId());

    patronMgr.render(window, font);

    player.render(window, font);

    renderHUD();

    sf::RectangleShape controlsBox(sf::Vector2f(255.f, 45.f));
    controlsBox.setPosition(sf::Vector2f(530.f, 535.f));
    controlsBox.setFillColor(sf::Color(20, 25, 35, 220));
    controlsBox.setOutlineColor(sf::Color(190, 190, 190));
    controlsBox.setOutlineThickness(1.5f);
    window.draw(controlsBox);

    sf::Text controlsText(font, "MOVE: WASD / ARROWS   ACTION: E\nCATALOG: TAB          PAUSE: ESC", 10);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setPosition(sf::Vector2f(540.f, 543.f));
    window.draw(controlsText);

    if (terminalUI.isVisible()) {
        terminalUI.render(window, font);
    }

    sf::Vector2f pPos = player.getPosition();
    std::string promptStr = "";
    if (map.isNearComputerTerminal(pPos)) {
        promptStr = "[E / TAB] Access Catalog Terminal";
    } else if (map.isNearCheckoutCounter(pPos)) {
        promptStr = player.isCarrying() ? "[E] Issue Book to Waiting Patron" : "Checkout Desk (Need book to issue)";
    } else {
        int sId = map.getInteractingShelf(pPos);
        if (sId > 0) {
            promptStr = "[E] Retrieve Book from Shelf #" + std::to_string(sId);
        }
    }

    if (!promptStr.empty()) {
        sf::RectangleShape promptBox(sf::Vector2f(420.f, 34.f));
        promptBox.setPosition(sf::Vector2f(190.f, 510.f));
        promptBox.setFillColor(sf::Color(20, 20, 20, 210));
        promptBox.setOutlineColor(sf::Color(240, 210, 70));
        promptBox.setOutlineThickness(1.5f);
        window.draw(promptBox);

        sf::Text pText(font, promptStr, 14);
        pText.setFillColor(sf::Color::Yellow);
        pText.setStyle(sf::Text::Bold);
        pText.setPosition(sf::Vector2f(205.f, 517.f));
        window.draw(pText);
    }

    if (notificationTimer > 0.0f) {
        sf::RectangleShape banner(sf::Vector2f(500.f, 32.f));
        banner.setPosition(sf::Vector2f(150.f, 15.f));
        banner.setFillColor(sf::Color(30, 45, 75, 230));
        banner.setOutlineColor(sf::Color::White);
        banner.setOutlineThickness(1.5f);
        window.draw(banner);

        sf::Text notif(font, notificationMsg, 11);
        notif.setFillColor(sf::Color::White);
        notif.setStyle(sf::Text::Bold);
        notif.setPosition(sf::Vector2f(165.f, 22.f));
        window.draw(notif);
    }
}

void GameEngine::renderHUD() {

    sf::RectangleShape hudPanel(sf::Vector2f(300.f, 125.f));
    hudPanel.setFillColor(sf::Color(20, 25, 35, 230));
    hudPanel.setOutlineColor(sf::Color(190, 190, 190));
    hudPanel.setOutlineThickness(2.f);
    hudPanel.setPosition(sf::Vector2f(12.f, 465.f));
    window.draw(hudPanel);

    const auto& p = player.getData();
    int hours = static_cast<int>(p.timeOfDay);
    int minutes = static_cast<int>((p.timeOfDay - hours) * 60.0f);
    std::string ampm = (hours >= 12) ? "PM" : "AM";
    int displayHours = (hours > 12) ? hours - 12 : hours;
    if (displayHours == 0) displayHours = 12;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << displayHours << ":"
       << std::setfill('0') << std::setw(2) << minutes << " " << ampm;

    std::string hudString =
        "LIBRARIAN: " + std::string(p.name) + "\n" +
        "BOOKS: " + std::to_string(p.booksHeld) + "/" + std::to_string(p.booksCapacity) + "\n" +
        "CARRYING: " + std::string(p.isCarryingBook ? p.carriedBookTitle : "Nothing") + "\n" +
        "PATRONS WAITING: " + std::to_string(patronMgr.getWaitingPatronCount()) + "\n" +
        "TIME: " + ss.str() + "\n" +
        "DAY: " + std::to_string(p.day) + " | SCORE: " + std::to_string(p.score);

    sf::Text hudText(font, hudString, 12);
    hudText.setFillColor(sf::Color::White);
    hudText.setStyle(sf::Text::Bold);
    hudText.setPosition(sf::Vector2f(24.f, 473.f));
    window.draw(hudText);
}

void GameEngine::renderHowToPlay() {
    sf::RectangleShape panel(sf::Vector2f(650.f, 480.f));
    panel.setPosition(sf::Vector2f(75.f, 55.f));
    panel.setFillColor(sf::Color(30, 40, 55, 245));
    panel.setOutlineColor(sf::Color(210, 180, 70));
    panel.setOutlineThickness(3.f);
    window.draw(panel);

    sf::Text heading(font, "HOW TO PLAY", 26);
    heading.setFillColor(sf::Color::Yellow);
    heading.setStyle(sf::Text::Bold);
    heading.setPosition(sf::Vector2f(285.f, 78.f));
    window.draw(heading);

    const std::string instructions =
        "YOUR JOB\n"
        "Serve each patron before their patience bar becomes empty.\n\n"
        "1. Read the book title and ISBN in the patron's request box.\n"
        "2. Press TAB to open the catalog and search for that book.\n"
        "3. Close the catalog and walk to the highlighted shelf.\n"
        "4. Press E near the shelf to collect the book.\n"
        "5. Walk to the checkout counter and press E to issue it.\n\n"
        "CONTROLS\n"
        "WASD / Arrow Keys  - Move the librarian\n"
        "E or Space         - Collect or issue a book\n"
        "TAB                - Open or close the catalog\n"
        "ESC                - Pause and save\n\n"
        "Correct book: +150 points    Wrong book: -25 points\n\n"
        "Press ENTER, SPACE, or ESC to return to the main menu.";

    sf::Text helpText(font, instructions, 14);
    helpText.setFillColor(sf::Color::White);
    helpText.setPosition(sf::Vector2f(110.f, 130.f));
    window.draw(helpText);
}

void GameEngine::renderPauseMenu() {

    sf::RectangleShape dim(sf::Vector2f(800.f, 600.f));
    dim.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(dim);

    sf::RectangleShape pauseBox(sf::Vector2f(340.f, 300.f));
    pauseBox.setPosition(sf::Vector2f(230.f, 150.f));
    pauseBox.setFillColor(sf::Color(35, 45, 60, 240));
    pauseBox.setOutlineColor(sf::Color(220, 190, 80));
    pauseBox.setOutlineThickness(3.f);
    window.draw(pauseBox);

    sf::Text pauseTitle(font, "GAME PAUSED", 20);
    pauseTitle.setFillColor(sf::Color::Yellow);
    pauseTitle.setStyle(sf::Text::Bold);
    pauseTitle.setPosition(sf::Vector2f(325.f, 170.f));
    window.draw(pauseTitle);

    for (size_t i = 0; i < pauseOptions.size(); ++i) {
        sf::RectangleShape btn(sf::Vector2f(280.f, 36.f));
        btn.setPosition(sf::Vector2f(260.f, 220.f + i * 48.f));

        if (static_cast<int>(i) == selectedPauseIndex) {
            btn.setFillColor(sf::Color(70, 130, 200));
            btn.setOutlineColor(sf::Color::Yellow);
            btn.setOutlineThickness(2.f);
        } else {
            btn.setFillColor(sf::Color(45, 55, 70));
            btn.setOutlineColor(sf::Color(100, 110, 130));
            btn.setOutlineThickness(1.f);
        }
        window.draw(btn);

        sf::Text optText(font, pauseOptions[i], 12);
        optText.setFillColor(sf::Color::White);
        optText.setStyle(sf::Text::Bold);
        optText.setPosition(sf::Vector2f(280.f, 230.f + i * 48.f));
        window.draw(optText);
    }
}

void GameEngine::renderGameOver() {
    sf::RectangleShape box(sf::Vector2f(500.f, 360.f));
    box.setPosition(sf::Vector2f(150.f, 120.f));
    box.setFillColor(sf::Color(40, 20, 25, 245));
    box.setOutlineColor(sf::Color(220, 50, 50));
    box.setOutlineThickness(4.f);
    window.draw(box);

    sf::Text goTitle(font, "LIBRARY OPERATIONAL FAILURE", 22);
    goTitle.setFillColor(sf::Color(255, 70, 70));
    goTitle.setStyle(sf::Text::Bold);
    goTitle.setPosition(sf::Vector2f(210.f, 145.f));
    window.draw(goTitle);

    const auto& p = player.getData();
    std::string summary =
        "Patrons' patience expired! The library reputational rating collapsed.\n\n"
        "FINAL STATISTICS:\n"
        "  - Head Librarian  : " + std::string(p.name) + "\n"
        "  - Final Score     : " + std::to_string(p.score) + "\n"
        "  - Days Managed    : " + std::to_string(p.day) + "\n"
        "  - Patrons Served  : " + std::to_string(p.patronsServed) + "\n"
        "  - Patrons Lost    : " + std::to_string(p.patronsLost) + "\n\n"
        "Press [ENTER] or [SPACE] to Save Score & Return to Menu.";

    sf::Text sumText(font, summary, 12);
    sumText.setFillColor(sf::Color::White);
    sumText.setPosition(sf::Vector2f(180.f, 200.f));
    window.draw(sumText);
}

void GameEngine::renderLeaderboard() {
    sf::RectangleShape box(sf::Vector2f(600.f, 440.f));
    box.setPosition(sf::Vector2f(100.f, 80.f));
    box.setFillColor(sf::Color(30, 40, 55, 245));
    box.setOutlineColor(sf::Color(210, 180, 70));
    box.setOutlineThickness(3.f);
    window.draw(box);

    sf::Text lbTitle(font, "HIGH SCORES & HALL OF FAME", 20);
    lbTitle.setFillColor(sf::Color::Yellow);
    lbTitle.setStyle(sf::Text::Bold);
    lbTitle.setPosition(sf::Vector2f(230.f, 105.f));
    window.draw(lbTitle);

    sf::Text head(font, "RANK   LIBRARIAN              SCORE    DAYS   SERVED    DATE", 11);
    head.setFillColor(sf::Color::Cyan);
    head.setStyle(sf::Text::Bold);
    head.setPosition(sf::Vector2f(130.f, 150.f));
    window.draw(head);

    for (size_t i = 0; i < highScores.size() && i < 8; ++i) {
        const auto& hs = highScores[i];
        std::ostringstream ss;
        ss << "#" << (i + 1) << "     "
           << std::left << std::setw(22) << hs.playerName
           << std::right << std::setw(6) << hs.score << "    "
           << std::setw(4) << hs.dayReached << "    "
           << std::setw(6) << hs.patronsServed << "    "
           << hs.dateString;

        sf::Text row(font, ss.str(), 11);
        row.setFillColor(sf::Color::White);
        row.setPosition(sf::Vector2f(130.f, 180.f + i * 28.f));
        window.draw(row);
    }

    sf::Text exitPrompt(font, "Press [ESC] or [ENTER] to return to Main Menu", 11);
    exitPrompt.setFillColor(sf::Color(180, 180, 180));
    exitPrompt.setPosition(sf::Vector2f(250.f, 480.f));
    window.draw(exitPrompt);
}
