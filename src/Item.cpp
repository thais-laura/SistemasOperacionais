#include "Item.h"

// Inicialização
Item::Item(std::string textureDir, sf::Vector2f position, float scale, sf::Vector2f originalOffset, int Value)
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

void Item::setVisible(bool visible) {
    m_isVisible = visible;
}

bool Item::isVisible() {
    return m_isVisible;
}

void Item::render(sf::RenderTarget* target) {
    if (m_isVisible) {
        target->draw(*m_sprite);
    }
}

void Item::setValue(int value) {
    this->Value = value;
}

int Item::getValue() {
    return this->Value;
}

void Item::setPosition(sf::Vector2f position) {
    m_sprite->setPosition(position);
}

sf::Vector2f Item::getOriginalOffset() {
    return m_originalOffset;
}
