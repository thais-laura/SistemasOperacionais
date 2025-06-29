#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <mutex>

// Definir isso ainda??
#define WORLD_MARGIN_TOP 88
#define WORLD_WIDTH 25
#define WORLD_HEIGHT 15

using namespace sf;

class World {
private:
    // Se a posi��o (x, y) est� ocupada, occupied[x][y] = true
    // S� � permitido que uma thread acesse essa vari�vel por vez.
    bool occupied[WORLD_WIDTH][WORLD_HEIGHT] = {};

    // Vari�veis do tesouro
    int treasure_x = 0;
    int treasure_y = 0;

public:
    // Mutex de acesso � vari�vel occupied
    std::mutex mutex;

    World(const char* path_ground, const char* path_ores);
    ~World();

    void render();
    bool isOccupied(int x, int y);
    void setOccupied(int x, int y, bool value);

    void spawnTreasure();
    int catchTreasure(int x, int y);

    Vector2f randomPoint();
    Vector2f getTreasurePosition();
};

#endif