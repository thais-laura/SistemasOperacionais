#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <map>
#include <chrono>
#include <memory>
#include <string>

class Item {
private:
    sf::Sprite* m_sprite;
    sf::Texture m_texture;
    bool m_isVisible;
    sf::Vector2f m_originalOffset;

    int Value;

public:
    Item(std::string textureDir, sf::Vector2f position, float scale = 1.0f, sf::Vector2f originalOffset = { 0.f, 0.f }, int Value=100)
        :
        m_isVisible(false),
        m_originalOffset(originalOffset),
        Value(Value)
    {

        m_texture.loadFromFile(textureDir);
        m_sprite = new sf::Sprite(m_texture);

        m_sprite->setPosition(position);
        m_sprite->setScale(sf::Vector2f(scale, scale));
    }

    void setVisible(bool visible) {
        m_isVisible = visible;
    }

    bool isVisible() const {
        return m_isVisible;
    }

    void render(sf::RenderTarget* target) const {
        if (m_isVisible) {
            target->draw(*m_sprite);
        }
    }

    void setValue(int value) {
        this->Value = value;
    }

    int getValue() {
        return this->Value;
    }

    void setPosition(sf::Vector2f position) {
        m_sprite->setPosition(position);
    }

    sf::Vector2f getOriginalOffset() const {
        return m_originalOffset;
    }
};