#ifndef TOAST_HPP
#define TOAST_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "MessageBox.hpp"

class Toast {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Clock clock;
    bool isVisible;
    float duration;
    float fadeTime;

public:
    Toast(sf::Font& font) {
        box.setSize(sf::Vector2f(400, 60));
        box.setPosition(500, 800);
        box.setFillColor(sf::Color(40, 40, 40, 230));

        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        isVisible = false;
        duration = 3.0f;
        fadeTime = 0.5f;
    }

    void show(const std::string& message, MessageType type = MessageType::INFO) {
        text.setString(message);
        
        // Căn giữa text
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(700, 820);

        // Đặt màu theo loại
        switch(type) {
            case MessageType::SUCCESS:
                box.setFillColor(sf::Color(40, 140, 40, 230));
                break;
            case MessageType::ERROR:
                box.setFillColor(sf::Color(180, 40, 40, 230));
                break;
            case MessageType::WARNING:
                box.setFillColor(sf::Color(200, 150, 40, 230));
                break;
            default:
                box.setFillColor(sf::Color(40, 40, 40, 230));
        }

        isVisible = true;
        clock.restart();
    }

    void update() {
        if (!isVisible) return;

        float elapsed = clock.getElapsedTime().asSeconds();
        
        if (elapsed > duration) {
            // Fade out
            float fadeProgress = (elapsed - duration) / fadeTime;
            if (fadeProgress >= 1.0f) {
                isVisible = false;
            } else {
                sf::Color boxColor = box.getFillColor();
                boxColor.a = 230 * (1.0f - fadeProgress);
                box.setFillColor(boxColor);
                
                sf::Color textColor = text.getFillColor();
                textColor.a = 255 * (1.0f - fadeProgress);
                text.setFillColor(textColor);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (isVisible) {
            window.draw(box);
            window.draw(text);
        }
    }

    bool isShown() const {
        return isVisible;
    }
};

#endif