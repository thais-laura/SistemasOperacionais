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
    Item(std::string textureDir, sf::Vector2f position, float scale = 1.0f, sf::Vector2f originalOffset = { 0.f, 0.f }, int Value = 100);

    ~Item();

    void setVisible(bool visible);

    bool isVisible();

    void render(sf::RenderTarget* target);

    void setValue(int value);

    int getValue();

    void setPosition(sf::Vector2f position);

    sf::Vector2f getOriginalOffset();
};