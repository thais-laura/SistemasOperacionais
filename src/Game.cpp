#include "Game.h"

// Funções estáticas

// Funções de inicialização 
void Game::initPersonagens() {
    
    std::string dirPersonagem1Texture = "../assets/personagem1.png";
    std::string dirPersonagem2Texture = "../assets/personagem2.png";
    std::string dirPersonagem3Texture = "../assets/personagem3.png";
    std::string dirThreadNamesTexture = "../assets/thread.png";

    const float THREAD_NAME_SCALE = 0.25f;

    Personagem* thread1 = new Personagem(dirPersonagem1Texture, sf::Vector2f(CELL_SIZE * 1, CELL_SIZE * 9), 2.0f, dirThreadNamesTexture, THREAD_NAME_SCALE, 0);
    Personagem* thread2 = new Personagem(dirPersonagem2Texture, sf::Vector2f(CELL_SIZE * 12, CELL_SIZE * 17), 2.0f, dirThreadNamesTexture, THREAD_NAME_SCALE, 1);
    Personagem* thread3 = new Personagem(dirPersonagem3Texture, sf::Vector2f(CELL_SIZE * 15, CELL_SIZE * 17), 2.0f, dirThreadNamesTexture, THREAD_NAME_SCALE, 2);
    
    this->personagens.push_back(std::move(thread1));
    this->personagens.push_back(std::move(thread2));
    this->personagens.push_back(std::move(thread3));
    
}

void Game::initBau() {
    this->world->spawnTreasure();
    sf::Vector2f initPos = { this->world->getTreasurePosition().x * CELL_SIZE, this->world->getTreasurePosition().y*CELL_SIZE};
    this->bau = new Bau(initPos, 1.0);
}

//Função para inicializar a janela de renderização do jogo
void Game::initWindow() {
    this->window = new sf::RenderWindow(sf::VideoMode({ 1200, 800 }), "Visualizar Cenario", sf::Style::Close);
    this->window->setFramerateLimit(120);
    this->window->setVerticalSyncEnabled(false);
}



//Construtor
//Construir o jogo, inicializando a Tela de renderização, os Estados do jogo e o Jogador
Game::Game() {
    this->initWindow();
    this->initPersonagens();
    this->world = new World();
    this->initBau();
    this->accumulator = 0.0;
    this->timeStep = 1.0 / 60.0;
}

//Destrutor
Game::~Game() {
    delete this->window;

    for (auto it = this->personagens.begin(); it != this->personagens.end(); it++) {
        delete *it;
    }

    delete this->world;
    delete this->bau;

}

//Funções
//Quando o Jogador sair do jogo, pelo botão Quit Game ou apertando Close do windows, Salvar os dados
void Game::endApplication() {
    this->window->close();
}

//Atualizar variavel dt com tempo entre cada frame. Usado para calculos de produção dos Animais
void Game::incrementAccumulator() {
    this->dt = this->dtClock.restart().asSeconds();
    this->accumulator += this->dt;
}

//Função para cuidar de eventos reconhecidos pelo SFML, como fechar janela e Leitura de Teclado
void Game::updateSFMLEvents() {
    while (const std::optional event = this->window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            this->endApplication();
        }
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            this->personagens[0]->handleKeyPress(*keyPressed, this->world);
        }
    }

}

void Game::updateRandomEnemies() {
    this->personagens[1]->randomMovement(this->world);
    this->personagens[2]->randomMovement(this->world);
}

//Atualizar o Jogo, alterando o Estado em que se encontra,
// pegando os Eventos de entrada do jogador, Atualizando os recursos do Jogador pela produção dos Animais,
// e atualizando o próprio estado em que se encontra com os novos dados do Jogador
void Game::update()
{

    while (this->accumulator >= this->timeStep) {
        this->accumulator -= this->timeStep;

        this->updateSFMLEvents();

        this->updateRandomEnemies();

        for (auto it = this->personagens.begin(); it != this->personagens.end(); it++) {
            (*it)->update(this->world, this->bau);
        }
        
        this->bau->update();
    }
}


//Função para renderizar os graficos na janela. Limpa a janela e renderiza os dados especificos do
//Estado em que está (Botões, Menu, Sprites...) e do Jogador (Sprites e textos de recursos)
void Game::render()
{
    this->window->clear();

    //Render
    this->world->render(this->window);
    
    for (auto it = this->personagens.begin(); it != this->personagens.end(); it++) {
        (*it)->render(this->window);
    }

    this->bau->render(this->window);

    this->window->display();
}
//Ciclo do jogo. Atualiza o tempo, depois todos os dados no Jogo e então Renderiza
void Game::run() {

    while (this->window->isOpen()) {
        this->incrementAccumulator();
        this->update();
        this->render();
    }

}