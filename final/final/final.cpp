#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <map>
#include <chrono>
#include <memory> 

class Item {
private:
    sf::Sprite m_sprite;
    bool m_isVisible;
    sf::Vector2f m_originalOffset;

public:
    Item(const sf::Texture& texture, sf::Vector2f position, float scale = 1.0f, sf::Vector2f originalOffset = { 0.f, 0.f })
        : m_sprite(texture),
        m_isVisible(false),
        m_originalOffset(originalOffset)
    {
        m_sprite.setPosition(position);
        m_sprite.setScale(sf::Vector2f(scale, scale));
    }

    void setVisible(bool visible) {
        m_isVisible = visible;
    }

    bool isVisible() const {
        return m_isVisible;
    }

    void draw(sf::RenderWindow& window) const {
        if (m_isVisible) {
            window.draw(m_sprite);
        }
    }

    void setPosition(sf::Vector2f position) {
        m_sprite.setPosition(position);
    }

    sf::Vector2f getOriginalOffset() const {
        return m_originalOffset;
    }
};
class Bau {
public:
    enum class Estado {
        Fechado,
        Abrindo,
        Aberto,
        Fechando
    };

private:
    sf::Sprite m_sprite;
    Estado m_estado;
    int m_currentFrame;
    float m_animationTime;
    float m_frameDuration;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;

    std::vector<sf::IntRect> m_frames;

    std::vector<std::unique_ptr<Item>> m_itens;

public:
    Bau(const sf::Texture& texture, sf::Vector2f position, float scale)
        : m_sprite(texture),
        m_estado(Estado::Fechado),
        m_currentFrame(0),
        m_animationTime(0.5f),
        m_frameDuration(m_animationTime / 4.0f)
    {
        m_sprite.setScale(sf::Vector2f(scale, scale));
        m_sprite.setPosition(position);

        m_frames.push_back(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(48, 36)));
        m_frames.push_back(sf::IntRect(sf::Vector2i(48, 0), sf::Vector2i(48, 36)));
        m_frames.push_back(sf::IntRect(sf::Vector2i(96, 0), sf::Vector2i(48, 36)));
        m_frames.push_back(sf::IntRect(sf::Vector2i(144, 0), sf::Vector2i(48, 36)));

        m_sprite.setTextureRect(m_frames[m_currentFrame]);
        m_lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    Bau(const Bau&) = delete; 
    Bau& operator=(const Bau&) = delete; 
    Bau(Bau&&) = default; 
    Bau& operator=(Bau&&) = default; 


    
    void addItem(std::unique_ptr<Item> item) {
        item->setPosition(m_sprite.getPosition() + item->getOriginalOffset());
        m_itens.push_back(std::move(item)); 
    }

    void abrir() {
        if (m_estado == Estado::Fechado) {
            m_estado = Estado::Abrindo;
            m_currentFrame = 0;
            m_lastFrameTime = std::chrono::high_resolution_clock::now();
        }
    }

    void fechar() {
        if (m_estado == Estado::Aberto) {
            m_estado = Estado::Fechando;
            m_currentFrame = static_cast<int>(m_frames.size() - 1);
            m_lastFrameTime = std::chrono::high_resolution_clock::now();
            for (auto& item : m_itens) {
                item->setVisible(false);
            }
        }
    }

    void update() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - m_lastFrameTime;

        if (elapsed.count() >= m_frameDuration) {
            if (m_estado == Estado::Abrindo) {
                m_currentFrame++;

                if (m_currentFrame >= m_frames.size()) {
                    m_currentFrame = static_cast<int>(m_frames.size() - 1);
                    m_estado = Estado::Aberto;
                    for (auto& item : m_itens) {
                        item->setVisible(true);
                    }
                }
                m_lastFrameTime = currentTime;
                m_sprite.setTextureRect(m_frames[m_currentFrame]);
            }
            else if (m_estado == Estado::Fechando) {
                m_currentFrame--;

                if (m_currentFrame < 0) {
                    m_currentFrame = 0;
                    m_estado = Estado::Fechado;
                }
                m_lastFrameTime = currentTime;
                m_sprite.setTextureRect(m_frames[m_currentFrame]);
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(m_sprite);
        for (const auto& item : m_itens) {
            item->draw(window);
        }
    }

    sf::FloatRect getGlobalBounds() const {
        return m_sprite.getGlobalBounds();
    }

    Estado getEstado() const {
        return m_estado;
    }
};

class Personagem {
public:
    enum class Estado {
        Parado,
        Animando
    };

private:
    sf::Sprite m_sprite;
    Estado m_estado;
    int m_currentFrame;
    float m_animationSpeed;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;

    std::vector<sf::IntRect> m_animationFrames;

    const int FRAME_WIDTH = 32;  
    const int FRAME_HEIGHT = 32;  

public:
    Personagem(const sf::Texture& texture, sf::Vector2f position, float scale = 3.f)
        : m_sprite(texture),
        m_estado(Estado::Parado),
        m_currentFrame(0),
        m_animationSpeed(10.0f)
    {
        m_sprite.setScale(sf::Vector2f(scale, scale));
        m_sprite.setPosition(position);

        const int START_Y_FOR_THIRD_ROW = 2 * FRAME_HEIGHT;

        for (int i = 0; i < 10; ++i) {
            m_animationFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, START_Y_FOR_THIRD_ROW),
                sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        m_sprite.setTextureRect(m_animationFrames[0]);
        m_lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    void iniciarAnimacao() {
        if (m_estado == Estado::Parado) {
            m_estado = Estado::Animando;
            m_currentFrame = 0;
            m_lastFrameTime = std::chrono::high_resolution_clock::now();
        }
    }

    void update() {
        if (m_estado == Estado::Animando) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currentTime - m_lastFrameTime;

            float frameDuration = 1.0f / m_animationSpeed;

            if (elapsed.count() >= frameDuration) {
                m_currentFrame++;
                if (m_currentFrame >= m_animationFrames.size()) {
                    m_currentFrame = 0;
                    m_estado = Estado::Parado;
                }
                m_sprite.setTextureRect(m_animationFrames[m_currentFrame]);
                m_lastFrameTime = currentTime;
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(m_sprite);
    }

    sf::FloatRect getGlobalBounds() const {
        return m_sprite.getGlobalBounds();
    }

    Estado getEstado() const {
        return m_estado;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Visualizar Cenario");
    window.setFramerateLimit(60);

    sf::Texture cenarioTexture;
    if (!cenarioTexture.loadFromFile("cenario.png")) {
        return -1;
    }
    sf::Sprite cenarioSprite(cenarioTexture);
    cenarioSprite.setScale({ 3.f, 3.f });

    sf::Texture bauTexture;
    if (!bauTexture.loadFromFile("bau.png")) {
        return -1;
    }

    sf::Texture trofeuTexture;
    if (!trofeuTexture.loadFromFile("trofeu.png")) {
        return -1;
    }
    sf::Texture tesouroTexture;
    if (!tesouroTexture.loadFromFile("tesouro.png")) {
        return -1;
    }

    const float ORIGINAL_BAU_FRAME_WIDTH = 48.0f;
    const float BAU_SCALE = 1.0f;

    // BAÚ 1: Somente o troféu
    Bau bau1(bauTexture, sf::Vector2f(580.f, 350.f), BAU_SCALE);
    const float TROFEU_SCALE = 0.15f;
    const float TROFEU_OFFSET_Y = -120.f;
    const float TROFEU_OFFSET_X = -90.f;

    bau1.addItem(std::make_unique<Item>(trofeuTexture, sf::Vector2f(0.f, 0.f), TROFEU_SCALE,
        sf::Vector2f(TROFEU_OFFSET_X, TROFEU_OFFSET_Y)));

    // BAÚ 2: Somente o tesouro
    Bau bau2(bauTexture, sf::Vector2f(5.f, 210.f), BAU_SCALE);
    const float TESOURO_SCALE = 0.10f;
    const float TESOURO_OFFSET_Y = -40.f;

    float tesouroOffsetX = ORIGINAL_BAU_FRAME_WIDTH * BAU_SCALE / 2.f; 
    tesouroOffsetX -= 5.f; 
  

    bau2.addItem(std::make_unique<Item>(tesouroTexture, sf::Vector2f(0.f, 0.f), TESOURO_SCALE,
        sf::Vector2f(tesouroOffsetX, TESOURO_OFFSET_Y)));
    std::vector<Bau> baus;
    baus.push_back(std::move(bau1));
    baus.push_back(std::move(bau2));
  

    sf::Texture personagem1Texture;
    if (!personagem1Texture.loadFromFile("personagem1.png")) {
        return -1;
    }

    sf::Texture personagem2Texture;
    if (!personagem2Texture.loadFromFile("personagem2.png")) {
        return -1;
    }

    sf::Texture personagem3Texture;
    if (!personagem3Texture.loadFromFile("personagem3.png")) {
        return -1;
    }

    Personagem personagem1(personagem1Texture, sf::Vector2f(450.f, 550.f), 2.0f);
    Personagem personagem2(personagem2Texture, sf::Vector2f(400.f, 200.f), 2.0f);
    Personagem personagem3(personagem3Texture, sf::Vector2f(40.f, 350.f), 2.0f);

    std::vector<Personagem> personagens;
    personagens.push_back(personagem1);
    personagens.push_back(personagem2);
    personagens.push_back(personagem3);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                for (Bau& bau : baus) {
                    if (bau.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (bau.getEstado() == Bau::Estado::Fechado) {
                            bau.abrir();
                        }
                        else if (bau.getEstado() == Bau::Estado::Aberto) {
                            bau.fechar();
                        }
                    }
                }

                for (Personagem& p : personagens) {
                    if (p.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (p.getEstado() == Personagem::Estado::Parado) {
                            p.iniciarAnimacao();
                        }
                    }
                }
            }
        }

        for (Bau& bau : baus) {
            bau.update();
        }

        for (Personagem& p : personagens) {
            p.update();
        }

        window.clear();
        window.draw(cenarioSprite);

        for (const Bau& bau : baus) {
            bau.draw(window);
        }

        for (const Personagem& p : personagens) {
            p.draw(window);
        }

        window.display();
    }

    return 0;
}