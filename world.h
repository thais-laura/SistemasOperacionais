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
    // Se a posição (x, y) está ocupada, occupied[x][y] = true
    // Só é permitido que uma thread acesse essa variável por vez.
    bool occupied[WORLD_WIDTH][WORLD_HEIGHT] = {};

    // Variáveis do tesouro
    int treasure_x = 0;
    int treasure_y = 0;

public:
    // Mutex de acesso à variável occupied
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