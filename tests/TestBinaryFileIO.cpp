#include "../Library_Managment_System/BinaryFileManager.hpp"
#include <iostream>
#include <cassert>

// ============================================================================
// Unit Test: Binary File Serialization & Deserialization
// Tests saving & loading catalog.dat and savegame.bin structs
// ============================================================================

void testCatalogSaveAndLoad() {
    std::vector<Book> original;
    original.emplace_back(9001, "Test Book One", "Test Author", "Academic", "Shelf A1", 1, 10, 4, 3);
    original.emplace_back(9002, "Test Book Two", "Test Author B", "Fiction", "Shelf B1", 2, 20, 4, 5);

    bool saveOk = BinaryFileManager::saveCatalog(original, "test_catalog.dat");
    assert(saveOk == true);

    std::vector<Book> restored;
    bool loadOk = BinaryFileManager::loadCatalog(restored, "test_catalog.dat");
    assert(loadOk == true);
    assert(restored.size() == 2);
    assert(restored[0].isbn == 9001);
    assert(std::string(restored[0].title) == "Test Book One");
    assert(restored[1].isbn == 9002);

    std::cout << "[PASS]: testCatalogSaveAndLoad\n";
}

void testGameStateSaveAndLoad() {
    Player p;
    p.score = 3450;
    p.day = 8;
    p.position = Vector2D(250.0f, 380.0f);

    std::vector<Patron> patrons;
    Patron p1;
    p1.id = 42;
    p1.patience = 85.0f;
    patrons.push_back(p1);

    bool saveOk = BinaryFileManager::saveGameState(p, patrons, "test_savegame.bin");
    assert(saveOk == true);

    Player restoredP;
    std::vector<Patron> restoredPatrons;
    bool loadOk = BinaryFileManager::loadGameState(restoredP, restoredPatrons, "test_savegame.bin");
    assert(loadOk == true);
    assert(restoredP.score == 3450);
    assert(restoredP.day == 8);
    assert(restoredPatrons.size() == 1);
    assert(restoredPatrons[0].id == 42);

    std::cout << "[PASS]: testGameStateSaveAndLoad\n";
}

int main() {
    std::cout << "Running Binary File I/O Unit Tests...\n";
    testCatalogSaveAndLoad();
    testGameStateSaveAndLoad();
    std::cout << "All File I/O Tests Passed Successfully!\n";
    return 0;
}
