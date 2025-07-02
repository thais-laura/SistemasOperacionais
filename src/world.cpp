#include "world.h"

void World::initTextures() {
    sf::Texture tempTexture;
	// Carregando a textura do cenario
    if (tempTexture.loadFromFile("..\\assets\\cenario.png")) this->cenarioTexture = tempTexture;
}

void World::initSprite() { 
	// Sprite do cenario
    this->cenarioSprite = new sf::Sprite(this->cenarioTexture);
    this->cenarioSprite->setScale({ 3.f,3.f });
}

void World::clearMap() {
    // Marcando todo mada desocupado
    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int y = 0; y < WORLD_HEIGHT; y++) {
            occupied[x][y] = false;
        }
    }

    // Marcando as paredes como ocupado
    for (int x = 0; x < WORLD_WIDTH; x++) {
        occupied[x][0] = true;
        occupied[x][WORLD_HEIGHT - 1] = true;
    }
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        occupied[0][y] = true;
        occupied[WORLD_WIDTH - 1][y] = true;
    }

}

World::World() {
    this->initTextures();
    this->initSprite();

    srand(time(0));

    clearMap();
}

World::~World() {}

void World::render(sf::RenderTarget* target) {
    target->draw(*this->cenarioSprite);
}

bool World::isOccupied(int x, int y) {
    return occupied[x][y];
}

void World::setOccupied(int x, int y, bool value) {
    occupied[x][y] = value;
}

void World::printMap() {
    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int y = 0; y < WORLD_HEIGHT; y++) {
            if (occupied[x][y]) std::cout << 1;
            else std::cout << 0;
        }
        std::cout << "\n";
    }
}

void World::spawnTreasure() {
    Vector2i p = randomPoint();
    treasure_x = p.x;
    treasure_y = p.y;
 
}

int World::catchTreasure(int x, int y) {
    if (treasure_x == x && treasure_y == y) {
        int points = 100;

        // Gerando um novo tesouro
        spawnTreasure();

        return points;
    }
    return 0;
}

// Retorna um ponto (x, y) aleat�rio que nao esta ocupado no mapa
sf::Vector2i  World::randomPoint() {
    int x = 0;
    int y = 0;
    while (occupied[x][y] == true) {
        x = rand() % WORLD_WIDTH;
        y = rand() % WORLD_HEIGHT;
    }
    return sf::Vector2i({ x, y });
}

sf::Vector2i  World::getTreasurePosition() {
    return sf::Vector2i({treasure_x, treasure_y});
}