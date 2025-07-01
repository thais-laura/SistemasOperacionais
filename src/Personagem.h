#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <map>
#include <chrono>
#include <memory>
#include <string>
#include <iostream>
#include "world.h"

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
    float gameSpeed;
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

    void initSprites(std::string textureDir, std::string nameTextureDir) {
        m_texture.loadFromFile(textureDir);
        m_nameTexture.loadFromFile(nameTextureDir);

        m_threadNameRect = sf::IntRect(sf::Vector2i(0, id * THREAD_NAME_HEIGHT), sf::Vector2i(THREAD_NAME_WIDTH, THREAD_NAME_HEIGHT));

        m_sprite = new sf::Sprite(m_texture);
        m_nomeSprite = new sf::Sprite(m_nameTexture);
    }

public:
    Personagem(std::string textureDir, sf::Vector2f position, float characterScale,
        std::string nameTextureDir, float nameScale = 1.0f, int id = 0)
        :m_estado(Estado::Parado),
        m_currentFrame(0),
        m_frameCounter(0),
        gameSpeed(3.0),
        id(id),
        score(0)
    {

        this->initSprites(textureDir, nameTextureDir);

        m_sprite->setScale(sf::Vector2f(characterScale, characterScale));
        m_sprite->setPosition(position);

        const int START_Y_FOR_THIRD_ROW = 2 * FRAME_HEIGHT;

        for (int i = 0; i < 8; ++i) {
            m_animationFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, START_Y_FOR_THIRD_ROW),
                sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        m_sprite->setTextureRect(m_animationFrames[0]);

        m_nomeSprite->setTextureRect(m_threadNameRect);

        m_nomeSprite->setScale(sf::Vector2f(nameScale, nameScale));

        m_nomeSprite->setOrigin(sf::Vector2f(THREAD_NAME_SPRITE_WIDTH / 2.0f, THREAD_NAME_SPRITE_HEIGHT / 2.0f));

        updateNameSpritePosition(characterScale);

        map_x = (int)position.x / 32.f;
        map_y = (int)position.y / 32.f;
    }

    Personagem(const Personagem&) = delete;
    Personagem& operator=(const Personagem&) = delete;
    Personagem(Personagem&&) = default;
    Personagem& operator=(Personagem&&) = default;


    void iniciarAnimacao() {
        if (m_estado == Estado::Parado) {
            m_estado = Estado::Animando;
            m_currentFrame = 0;
            m_frameCounter = 0;
        }
    }

    void updateNameSpritePosition(float characterScale) {
        float scaledCharacterWidth = FRAME_WIDTH * characterScale;
        float scaledCharacterHeight = FRAME_HEIGHT * characterScale;


        float nameSpriteX = m_sprite->getPosition().x + scaledCharacterWidth / 2.0f;

        float nameSpriteY = m_sprite->getPosition().y - scaledCharacterHeight + 40.0f;

        m_nomeSprite->setPosition(sf::Vector2f(nameSpriteX, nameSpriteY));
    }

    void mapMove(World* world, int direction) {
        this->m_estado = Estado::Animando;

        switch (direction) {
        case 0:
            this->m_moveSpeed_x = -4.0f / gameSpeed;
            this->m_moveSpeed_y = 0.0f / gameSpeed;
            world->setOccupied(map_x, map_y, false);
            map_x--;
            world->setOccupied(map_x, map_y, true);
            break;
        case 1:
            this->m_moveSpeed_x = 4.0f / gameSpeed;
            this->m_moveSpeed_y = 0.0f / gameSpeed;
            world->setOccupied(map_x, map_y, false);
            map_x++;
            world->setOccupied(map_x, map_y, true);
            break;
        case 2:
            this->m_moveSpeed_x = 0.0f / gameSpeed;
            this->m_moveSpeed_y = -4.0f / gameSpeed;
            world->setOccupied(map_x, map_y, false);
            map_y--;
            world->setOccupied(map_x, map_y, true);
            break;
        case 3:
            this->m_moveSpeed_x = 0.0f / gameSpeed;
            this->m_moveSpeed_y = 4.0f / gameSpeed;
            world->setOccupied(map_x, map_y, false);
            map_y++;
            world->setOccupied(map_x, map_y, true);
            break;
        }
    }

    void randomMovement(World* world) {
        if (m_estado == Estado::Animando) return;

        int treasure_x = world->getTreasurePosition().x;
        int treasure_y = world->getTreasurePosition().y;

        int p = (rand() % 100) + 1;

        if (p > 31) {
            for (int i = 0; i < 10; i++) {
                int direction = (rand() % 7);
                if (direction == 0 && !world->isOccupied(map_x - 1, map_y)) {
                    mapMove(world, direction);
                    break;
                }
                if (direction == 1 && !world->isOccupied(map_x + 1, map_y)) {
                    mapMove(world, direction);
                    break;
                }
                if (direction == 2 && !world->isOccupied(map_x, map_y - 1)) {
                    mapMove(world, direction);
                    break;
                }
                if (direction == 3 && !world->isOccupied(map_x, map_y + 1)) {
                    mapMove(world, direction);
                    break;
                }
                if (direction > 3) {
                    break;
                }
            }
        }
        else {
            if (treasure_x < map_x && !world->isOccupied(map_x - 1, map_y)) {
                mapMove(world, 0);
            }
            if (treasure_x > map_x && !world->isOccupied(map_x + 1, map_y)) {
                mapMove(world, 1);
            }
            if (treasure_y < map_y && !world->isOccupied(map_x, map_y - 1)) {
                mapMove(world, 2);
            }
            if (treasure_y > map_y && !world->isOccupied(map_x, map_y + 1)) {
                mapMove(world, 3);
            }
        }
    }

    void handleKeyPress(sf::Event::KeyPressed keyPressed, World* world) {

        if (m_estado == Estado::Animando) return;

        if (keyPressed.scancode == sf::Keyboard::Scancode::Left && !world->isOccupied(map_x-1,map_y)) {
            mapMove(world, 0);
        }
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Right && !world->isOccupied(map_x + 1, map_y) ) {
            mapMove(world, 1);
        }
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Up && !world->isOccupied(map_x, map_y - 1) ) {
            mapMove(world, 2);
        }
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Down && !world->isOccupied(map_x, map_y + 1) ) {
            mapMove(world, 3);
        }
        std::cout << world->getTreasurePosition().x << " " << world->getTreasurePosition().y << "\n";
    }

    void update() {
        if (m_estado == Estado::Animando) {
            m_frameCounter++;
            if (m_frameCounter == gameSpeed) {
                m_frameCounter = 0;
                m_currentFrame++;
            }
                if (m_currentFrame >= m_animationFrames.size()) {
                    m_currentFrame = 0;
                    m_estado = Estado::Parado;
                    m_moveSpeed_x = 0;
                    m_moveSpeed_y = 0;
                }
                m_sprite->setTextureRect(m_animationFrames[m_currentFrame]);

                m_sprite->setPosition({ m_sprite->getPosition().x + m_moveSpeed_x,m_sprite->getPosition().y + m_moveSpeed_y });
             
        }
        updateNameSpritePosition(m_sprite->getScale().x);
    }

    void render(sf::RenderTarget* target) {
        target->draw(*m_sprite);
        target->draw(*m_nomeSprite);
    }

    sf::FloatRect getGlobalBounds()  {
        return m_sprite->getGlobalBounds();
    }

    Estado getEstado()  {
        return m_estado;
    }

    sf::Vector2i getPosMap() {
        return sf::Vector2i({ this->map_x,this->map_y });
    }
};