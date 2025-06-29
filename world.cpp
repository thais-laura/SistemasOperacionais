#include "world.h"

World::World( const char* path_ground, const char* path_ores) {
    //this->engine = engine;

   //Carregando textura do terreno e dos minérios
    /*texture_ground = new Texture(engine, path_ground);
    texture_ground->setPos(0, WORLD_MARGIN_TOP);
    texture_ores = new Texture(engine, path_ores);
    texture_ores->setWidth(32);
    texture_ores->setHeight(32);*/

    // Marcando as paredes como ocupado
    for (int x = 0; x < WORLD_WIDTH; x++) {
        occupied[x][0] = true;
        occupied[x][1] = true;
        occupied[x][WORLD_HEIGHT - 1] = true;
        occupied[x][WORLD_HEIGHT - 2] = true;
    }
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        occupied[0][y] = true;
        occupied[WORLD_WIDTH - 1][y] = true;
    }
}

World::~World() {
    delete texture_ground;
    delete texture_ores;
}

void World::render() {
    texture_ground->render();
    texture_ores->render();
}

bool World::isOccupied(int x, int y) {
    return occupied[x][y];
}

void World::setOccupied(int x, int y, bool value) {
    occupied[x][y] = value;
}

void World::spawnTreasure() {
    Vector2f p = randomPoint();
    treasure_x = p.x;
    treasure_y = p.y;

    //texture_ores->setClip({ 32, 0, 32, 32 });
    texture_ores->setPos(treasure_x * 32, WORLD_MARGIN_TOP + treasure_y * 32);
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

// Retorna um ponto (x, y) aleatório que não está ocupado no mapa
Vector2f  World::randomPoint() {
    int x = 0;
    int y = 0;
    while (occupied[x][y] == true) {
        x = rand() % WORLD_WIDTH;
        y = rand() % WORLD_HEIGHT;
    }
    return { x, y };
}

Vector2f  World::getTreasurePosition() {
    return { treasure_x, treasure_y };
}