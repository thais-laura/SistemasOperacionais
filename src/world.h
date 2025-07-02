#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <mutex>
#include "Bau.h"

// Definir isso ainda??
#define WORLD_MARGIN_TOP 88
#define WORLD_WIDTH 21
#define WORLD_HEIGHT 22
#define CELL_SIZE 32.f

using namespace sf;

class World {
private:
    // Se a posi��o (x, y) est� ocupada, occupied[x][y] = true
    // S� � permitido que uma thread acesse essa vari�vel por vez.
    bool occupied[WORLD_WIDTH][WORLD_HEIGHT] = {};

    sf::Texture cenarioTexture;
    sf::Sprite* cenarioSprite;

    // Vari�veis do tesouro
    int treasure_x = 0;
    int treasure_y = 0;

    void initTextures();
    void initSprite();
    void clearMap();

public:
    // Mutex de acesso � vari�vel occupied
    std::mutex mutex;

    World();
    ~World();

    void render(sf::RenderTarget* target);
    bool isOccupied(int x, int y);
    void setOccupied(int x, int y, bool value);
    void printMap();

    void spawnTreasure();
    int catchTreasure(int x, int y);

    Vector2i randomPoint();
    Vector2i getTreasurePosition();

};

#endif