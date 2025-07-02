#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "world.h"
#include "Bau.h"

#define THREAD_NAME_WIDTH 288
#define THREAD_NAME_HEIGHT 95

class Personagem {
public:
    enum class Estado {
        Parado,
        Animando
    };

private:
    sf::Sprite* m_sprite;
    sf::Sprite* m_nomeSprite;
    sf::Texture m_texture;
    sf::Texture m_nameTexture;
    sf::IntRect m_threadNameRect;

    Estado m_estado;
    int m_frameCounter;
    int m_currentFrame;
    int id;
    int score;
    int gameSpeed;
    float m_animationSpeed;
    float m_moveSpeed_x;
    float m_moveSpeed_y;
    int map_x;
    int map_y;

    std::vector<sf::IntRect> m_animationFrames;

    const int FRAME_WIDTH = 32;
    const int FRAME_HEIGHT = 32;

    const int THREAD_NAME_SPRITE_WIDTH = 288;
    const int THREAD_NAME_SPRITE_HEIGHT = 95;

    void initSprites(std::string textureDir, std::string nameTextureDir);

public:
    Personagem(std::string textureDir, sf::Vector2f position, float characterScale,
        std::string nameTextureDir, float nameScale = 1.0f, int id = 0);

    Personagem(const Personagem&) = delete;
    Personagem& operator=(const Personagem&) = delete;
    Personagem(Personagem&&) = default;
    Personagem& operator=(Personagem&&) = default;


void iniciarAnimacao();
    

void updateNameSpritePosition(float characterScale);

void mapMove(World* world, int direction);

void randomMovement(World* world);
    

void handleKeyPress(sf::Event::KeyPressed keyPressed, World* world);

void checkGotTreasure(World* world, Bau* bau);

void update(World* world, Bau* bau);

void render(sf::RenderTarget* target);

sf::FloatRect getGlobalBounds();

Estado getEstado();

int getScore();

sf::Vector2i getPosMap();
};