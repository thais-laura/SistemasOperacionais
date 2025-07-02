#pragma once
#include "Item.h"

class Bau {
public:
    enum class Estado {
        Fechado,
        Abrindo,
        Aberto,
        Fechando,
        Escondido
    };

private:
    sf::Sprite* m_sprite;
    sf::Texture m_texture;
    Estado m_estado;
    int m_currentFrame;
    int m_frameCounter;
    int revolutions;
    int m_currentItem;

    std::vector<sf::IntRect> m_frames;

    sf::Vector2f m_itemSpawnOffset1;
    sf::Vector2f m_itemSpawnOffset2;
    sf::Vector2f m_bauNextPosition;
    sf::Vector2f m_bauOffset;

    std::vector<Item*> m_itens;

    void initSprite();
    void initItens();

public:
    Bau(sf::Vector2f position, float scale);

    Bau(const Bau&) = delete;
    Bau& operator=(const Bau&) = delete;
    Bau(Bau&&) = default;
    Bau& operator=(Bau&&) = default;


    void addItem(Item* item);

    void abrir();

    void fechar();

    void setEstado(Estado estado);

    int getScore();

    void setNextPosition(sf::Vector2f position);

    void updateItemPositions();

    void update();

    void render(sf::RenderTarget* target);

    sf::FloatRect getGlobalBounds();

    Estado getEstado();
};