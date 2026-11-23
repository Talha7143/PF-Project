#pragma once

#include <SFML/Graphics.hpp>
#include "Structs.hpp"
#include "Catalog.hpp"
#include "LibraryMap.hpp"
#include <vector>

class PatronManager {
private:
    std::vector<Patron> patrons;
    float spawnTimer{ 0.0f };
    float spawnInterval{ 6.0f };
    int nextPatronId{ 1 };

    void spawnNewPatron(Catalog& catalog);
    void updatePatronAI(Patron& p, float dt, const LibraryMap& map);

public:
    PatronManager();

    void initialize(Catalog& catalog);
    void update(float dt, Catalog& catalog, const LibraryMap& map, int& scoreDelta, int& servedDelta, int& lostDelta);
    void render(sf::RenderWindow& window, const sf::Font& font);

    Patron* getHeadOfQueue();
    bool serveHeadPatron(int carriedIsbn, Catalog& catalog, std::string& feedbackMsg);

    int getWaitingPatronCount() const;
    const std::vector<Patron>& getPatrons() const { return patrons; }
    std::vector<Patron>& getPatrons() { return patrons; }
    void setPatrons(const std::vector<Patron>& loadedPatrons) { patrons = loadedPatrons; }
};
