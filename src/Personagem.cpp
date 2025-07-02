#include "Personagem.h"

void Personagem::initSprites(std::string textureDir, std::string nameTextureDir) {
    m_texture.loadFromFile(textureDir);
    m_nameTexture.loadFromFile(nameTextureDir);

    m_threadNameRect = sf::IntRect(sf::Vector2i(0, id * THREAD_NAME_HEIGHT), sf::Vector2i(THREAD_NAME_WIDTH, THREAD_NAME_HEIGHT));

    m_sprite = new sf::Sprite(m_texture);
    m_nomeSprite = new sf::Sprite(m_nameTexture);
}

// Construtor
Personagem::Personagem(std::string textureDir, sf::Vector2f position, float characterScale,
        std::string nameTextureDir, float nameScale, int id)
        :m_estado(Estado::Parado),
        m_currentFrame(0),
        m_frameCounter(0),
        gameSpeed(3),
        id(id),
        score(0)
    {

        this->initSprites(textureDir, nameTextureDir);

        m_sprite->setScale(sf::Vector2f(characterScale, characterScale));
        m_sprite->setPosition(position);

        const int START_Y_FOR_THIRD_ROW = 2 * FRAME_HEIGHT;

        // Popula o mapa com os jogadores
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


    void Personagem::iniciarAnimacao() {
        if (m_estado == Estado::Parado) {
            m_estado = Estado::Animando;
            m_currentFrame = 0;
            m_frameCounter = 0;
        }
    }
	// Centraliza o nome acima do personagem
    void Personagem::updateNameSpritePosition(float characterScale) {
        float scaledCharacterWidth = FRAME_WIDTH * characterScale;
        float scaledCharacterHeight = FRAME_HEIGHT * characterScale;


        float nameSpriteX = m_sprite->getPosition().x + scaledCharacterWidth / 2.0f;

        float nameSpriteY = m_sprite->getPosition().y - scaledCharacterHeight + 40.0f;

        m_nomeSprite->setPosition(sf::Vector2f(nameSpriteX, nameSpriteY));
    }

	// Sempre atualiza as coordenadas do mapa para ocupado/desocupado
    void Personagem::mapMove(World* world, int direction) {
        this->m_estado = Estado::Animando;

        switch (direction) {
        case 0:
            this->m_moveSpeed_x = -1.0f;
            this->m_moveSpeed_y = 0.0f;
            world->setOccupied(map_x, map_y, false);
            map_x--;
            world->setOccupied(map_x, map_y, true);
            break;
        case 1:
            this->m_moveSpeed_x = 1.0f;
            this->m_moveSpeed_y = 0.0f;
            world->setOccupied(map_x, map_y, false);
            map_x++;
            world->setOccupied(map_x, map_y, true);
            break;
        case 2:
            this->m_moveSpeed_x = 0.0f;
            this->m_moveSpeed_y = -1.0f;
            world->setOccupied(map_x, map_y, false);
            map_y--;
            world->setOccupied(map_x, map_y, true);
            break;
        case 3:
            this->m_moveSpeed_x = 0.0f;
            this->m_moveSpeed_y = 1.0f;
            world->setOccupied(map_x, map_y, false);
            map_y++;
            world->setOccupied(map_x, map_y, true);
            break;
        }
    }
	// Movimento autônomo dos "inimigos" guiado pela posição do tesouro
    void Personagem::randomMovement(World* world) {
        if (m_estado == Estado::Animando) return;

        if (m_frameCounter > 0) {
            m_frameCounter--;
            return;
        }

        int treasure_x = world->getTreasurePosition().x;
        int treasure_y = world->getTreasurePosition().y;

        int p = (rand() % 100) + 1;

        if (p > 69) {
            for (int i = 0; i < 10; i++) {
                int direction = (rand() % 7);
                if (direction == 0 && !world->isOccupied(map_x - 1, map_y)) {
                    mapMove(world, direction);
                    break;
                }
                else if (direction == 1 && !world->isOccupied(map_x + 1, map_y)) {
                    mapMove(world, direction);
                    break;
                }
                else if (direction == 2 && !world->isOccupied(map_x, map_y - 1)) {
                    mapMove(world, direction);
                    break;
                }
                else if (direction == 3 && !world->isOccupied(map_x, map_y + 1)) {
                    mapMove(world, direction);
                    break;
                }
                else if (direction > 3) {
                    m_frameCounter = 32;
                    break;
                }
            }
        }
        else {
            if (treasure_x < map_x && !world->isOccupied(map_x - 1, map_y)) {
                mapMove(world, 0);
            }
            else if (treasure_x > map_x && !world->isOccupied(map_x + 1, map_y)) {
                mapMove(world, 1);
            }
            else if (treasure_y < map_y && !world->isOccupied(map_x, map_y - 1)) {
                mapMove(world, 2);
            }
            else if (treasure_y > map_y && !world->isOccupied(map_x, map_y + 1)) {
                mapMove(world, 3);
            }
        }
    }

    // Movimentação pelo teclado
    void Personagem::handleKeyPress(sf::Event::KeyPressed keyPressed, World* world) {
        if (m_estado == Estado::Animando) return;

        if ((keyPressed.scancode == sf::Keyboard::Scancode::Left || keyPressed.scancode == sf::Keyboard::Scancode::A) && !world->isOccupied(map_x - 1, map_y)) {
            mapMove(world, 0);
        }
        else if ((keyPressed.scancode == sf::Keyboard::Scancode::Right || keyPressed.scancode == sf::Keyboard::Scancode::D) && !world->isOccupied(map_x + 1, map_y)) {
            mapMove(world, 1);
        }
        else if ((keyPressed.scancode == sf::Keyboard::Scancode::Up || keyPressed.scancode == sf::Keyboard::Scancode::W) && !world->isOccupied(map_x, map_y - 1)) {
            mapMove(world, 2);
        }
        else if ((keyPressed.scancode == sf::Keyboard::Scancode::Down || keyPressed.scancode == sf::Keyboard::Scancode::S) && !world->isOccupied(map_x, map_y + 1)) {
            mapMove(world, 3);
        }
    }

	// Adiciona o prêmio à pontuação se conseguir pegar o tesouro
    void Personagem::checkGotTreasure(World* world, Bau* bau) {
        if (this->getPosMap() == world->getTreasurePosition()) {
            this->score += bau->getScore();
            world->spawnTreasure();
            bau->setNextPosition({ world->getTreasurePosition().x * CELL_SIZE, world->getTreasurePosition().y * CELL_SIZE });
        }
    }

    // Atualização dos sprites de acordo com a movimentação do personagem
    void Personagem::update(World* world, Bau* bau) {
		
        if (m_estado == Estado::Animando) {
            m_frameCounter++;
            if (m_frameCounter == gameSpeed) {
                m_frameCounter = 0;
                m_currentFrame++;
                m_sprite->setPosition({ m_sprite->getPosition().x + m_moveSpeed_x,m_sprite->getPosition().y + m_moveSpeed_y });
            }

            if (m_currentFrame >= m_animationFrames.size()) {
                m_sprite->setPosition({ m_sprite->getPosition().x + m_moveSpeed_x,m_sprite->getPosition().y + m_moveSpeed_y });
                m_currentFrame = 0;
                m_frameCounter = 0;
                m_estado = Estado::Parado;
                m_moveSpeed_x = 0;
                m_moveSpeed_y = 0;
            }
            m_sprite->setTextureRect(m_animationFrames[m_currentFrame]);

            m_sprite->setPosition({ m_sprite->getPosition().x + m_moveSpeed_x,m_sprite->getPosition().y + m_moveSpeed_y });
        }
        updateNameSpritePosition(m_sprite->getScale().x);

        checkGotTreasure(world, bau);
    }

    void Personagem::render(sf::RenderTarget* target) {
        target->draw(*m_sprite);
        target->draw(*m_nomeSprite);
    }

    // Retorna atributos do objeto

    sf::FloatRect Personagem::getGlobalBounds() {
        return m_sprite->getGlobalBounds();
    }

    Personagem::Estado Personagem::getEstado() {
        return m_estado;
    }

    int Personagem::getScore() {
        return score;
    }

    sf::Vector2i Personagem::getPosMap() {
        return sf::Vector2i({ this->map_x,this->map_y });
    }
