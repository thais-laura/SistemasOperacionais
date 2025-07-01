#pragma once
#ifndef GAME_H
#define GAME_H

#include "world.h"
#include "Bau.h"
#include "Personagem.h"

class Game {
private:
    //Variables
    sf::RenderWindow* window;

    sf::Clock dtClock;
    float dt;
    float accumulator;
    float timeStep;
    
    std::vector<Personagem*> personagens;
    Bau* bau;
    World* world;

    //Initialization
    void initWindow();
    void initPersonagens();
    void initBau();

public:
    //Constructor
    Game();
    //Destructor
    virtual ~Game();

    //Functions
    void endApplication();
    void updateDt();
    void incrementAccumulator();
    void updateSFMLEvents();
    void updateRandomEnemies();
    void update();
    void render();
    void run();

};

#endif