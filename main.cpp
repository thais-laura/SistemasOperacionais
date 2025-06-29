#include <iostream>
#include <string>
#include "world.h"

using namespace sf;

void init_world();
void logic_update(float timestep);
void render();


int main() {
	double timestep = 1.0 / 60.0;
	double accumulator = 0;
    Clock clock;
    float lastTime = clock.getElapsedTime().asSeconds();

    RenderWindow window;
    init_world(); // Inicializa o mundo, carrega a janela, etc

    while (window.isOpen())
    {
        float now = clock.getElapsedTime().asSeconds;
        float deltaT = now - lastTime;
        accumulator += deltaT;

        // não sei se coloca pra pegar o input aqui ou dentro da logica mesmo

        while (accumulator >= timestep) {
            // verifica se jogador mudou de posição, se apertou para sair, se bot moveu, se alguem pegou o tesouro
            logic_update(timestep);
            accumulator -= timestep;
        }
        render(); // renderiza o proprio mundo, os jogadores, os inimigos, etc
    }

    return 0;
}


void init_world() { // Carrega página inicial
    
}

void logic_update(float timestep) {
    // input
    // jogadores
    // inimigos
}
void render() {
    // clear e display mudanças
}