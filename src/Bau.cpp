#include "Bau.h"

void Bau::initSprite() {
    m_texture.loadFromFile("..\\assets\\bau.png");

    m_sprite = new sf::Sprite(m_texture); // Cria um novo sprite para o baú
}

void Bau::initItens() {
    // Caminho para as imagens
    std::string dirItem1 = "..\\assets\\trofeu.png";
    std::string dirItem2 = "..\\assets\\tesouro.png";

	// Cria os itens com suas posições, escalas e offsets
    const float TROFEU_SCALE = 0.1f;
    const float TROFEU_OFFSET_Y = -96.f;
    const float TROFEU_OFFSET_X = -32.f;

    const float TESOURO_SCALE = 0.15f;
    const float TESOURO_OFFSET_Y = -64.f;
    const float TESOURO_OFFSET_X = -8.f;

	// Adiciona os itens ao baú
    this->addItem(new Item(dirItem1, sf::Vector2f(0.f, 0.f), TROFEU_SCALE,
        sf::Vector2f(TROFEU_OFFSET_X, TROFEU_OFFSET_Y), 100));

    this->addItem(new Item(dirItem2, sf::Vector2f(0.f, 0.f), TESOURO_SCALE,
        sf::Vector2f(TESOURO_OFFSET_X, TESOURO_OFFSET_Y), 250));
}

// Construtor
Bau::Bau(sf::Vector2f position, float scale)
        :m_estado(Estado::Fechado),
        m_bauNextPosition(position),
        m_currentFrame(0),
        m_frameCounter(0),
        revolutions(6),
        m_currentItem(0)
{
    initSprite();
    initItens();

    m_bauOffset = { 8.f, 24.f };

    m_sprite->setScale(sf::Vector2f(scale, scale));
    m_sprite->setPosition(position + m_bauOffset);

    // Define os quadros da animação 
    m_frames.push_back(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(48, 36)));
    m_frames.push_back(sf::IntRect(sf::Vector2i(48, 0), sf::Vector2i(48, 36)));
    m_frames.push_back(sf::IntRect(sf::Vector2i(96, 0), sf::Vector2i(48, 36)));
    m_frames.push_back(sf::IntRect(sf::Vector2i(144, 0), sf::Vector2i(48, 36)));

    m_sprite->setTextureRect(m_frames[m_currentFrame]);

    const float ORIGINAL_BAU_FRAME_WIDTH = 48.0f;
    const float ORIGINAL_BAU_FRAME_HEIGHT = 36.0f;

    float bauScaledWidth = ORIGINAL_BAU_FRAME_WIDTH * scale;

    // Offsets para posicionamento dos itens
    m_itemSpawnOffset1.x = bauScaledWidth / 2.f - 25.f;
    m_itemSpawnOffset1.y = -40.f;

    m_itemSpawnOffset2.x = bauScaledWidth / 2.f + 25.f;
    m_itemSpawnOffset2.y = -40.f;
 }

void Bau::addItem(Item* item) {
    item->setPosition(m_sprite->getPosition() + item->getOriginalOffset());
    m_itens.push_back(item);
}

void Bau::abrir() {
    if (m_estado == Estado::Fechado) {
        m_estado = Estado::Abrindo;
        m_currentFrame = 0;
    }
}

void Bau::fechar() {
    if (m_estado == Estado::Aberto) {
        m_estado = Estado::Fechando;
        m_currentFrame = static_cast<int>(m_frames.size() - 1);
    }
}

void Bau::setEstado(Estado estado) {
    m_estado = estado;
}

int Bau::getScore() {
    this->m_currentItem = rand() % 2;
    this->abrir();
    return this->m_itens[m_currentItem]->getValue();
}

void Bau::setNextPosition(sf::Vector2f position) {
    this->m_bauNextPosition = position;
}

void Bau::updateItemPositions() {
    for (auto it : this->m_itens) {
        it->setPosition(m_sprite->getPosition() + it->getOriginalOffset());
    }
}

void Bau::update() {
	updateItemPositions(); // Atualiza as posições dos itens com base na posição do baú

    if (m_estado == Estado::Abrindo) {
        m_frameCounter++;
        if (m_frameCounter == revolutions) {
            m_currentFrame++;
            m_frameCounter = 0;
        }

        if (m_currentFrame >= m_frames.size()) {
            m_currentFrame = static_cast<int>(m_frames.size() - 1);
            m_frameCounter = 0;
            m_estado = Estado::Aberto;
        }
    }
    else if (m_estado == Estado::Fechando) {
        m_frameCounter++;
        if (m_frameCounter == revolutions) {
            m_currentFrame--;
            m_frameCounter = 0;
        }

        if (m_currentFrame < 0) {
            m_currentFrame = 0;
            m_frameCounter = 0;
            m_estado = Estado::Fechado;
        }
    }
    else if (m_estado == Estado::Aberto) {
        this->m_itens[m_currentItem]->setVisible(true);
        fechar();
    }
    else if (m_estado == Estado::Fechado) {
        m_sprite->setPosition(m_bauNextPosition + m_bauOffset);
        for (auto item : m_itens) {
            item->setVisible(false);
        }
    }
    m_sprite->setTextureRect(m_frames[m_currentFrame]);
}

// Visualização do baú e seus itens
void Bau::render(sf::RenderTarget* target) {
    target->draw(*m_sprite);
    for (const auto item : m_itens) {
        item->render(target);
    }
}

sf::FloatRect Bau::getGlobalBounds() {
	return m_sprite->getGlobalBounds(); // Retorna a posição global do baú
}

Bau::Estado Bau::getEstado() {
	return m_estado; // Retorna se o baú está aberto, fechado ou abrindo
}