#include <iostream>
#include <string>
#include "world.h"

using namespace sf;

void init_world();
void logic_update();
void render();


int main() {
	double timestep = 1.0 / 60.0;
	double accumulator = 0;
    Clock clock;
    Time lastTime = clock.getElapsedTime();

    RenderWindow* window;
    init_world(); // Inicializa o mundo, carrega a janela, etc

    while (window->isOpen())
    {
        Time now = clock.getElapsedTime();
        Time deltaT = now - lastTime;
        accumulator += deltaT.asSeconds();

        while (accumulator >= timestep) {
            // verifica se jogador mudou de posição, se apertou para sair, se bot moveu, se alguem pegou o tesouro
            logic_update();
            accumulator -= timestep;
        }
        render(); // renderiza o proprio mundo, os jogadores, os inimigos, etc
    }

    return 0;
}


void init_world() { // Carrega página inicial
    
}

void logic_update() {
    // input
    // jogadores
    // inimigos
}
void render() {
    // clear e display mudanças
}