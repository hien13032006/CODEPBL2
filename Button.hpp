#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "RoundedRectangle.hpp"

class Button {
private:
    RoundedRectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovered;
    std::function<void()> onClick;
    int id;

public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label,
           sf::Font& font, int buttonId = 0, sf::Color normal = sf::Color(33, 150, 243)) {
        
        shape.setSize(size);
        shape.setCornerRadius(8.0f);
        shape.setPosition(position);
        
        normalColor = normal;
        // Hover: làm sáng hơn
        hoverColor = sf::Color(
            std::min(255, (int)normal.r + 30),
            std::min(255, (int)normal.g + 30),
            std::min(255, (int)normal.b + 30)
        );
        shape.setFillColor(normalColor);
        isHovered = false;
        id = buttonId;

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(position.x + size.x / 2, position.y + size.y / 2 - 3);
    }

    void setOnClick(std::function<void()> callback) { onClick = callback; }
    int getId() const { return id; }

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                shape.setFillColor(hoverColor);
            }
        } else {
            if (isHovered) {
                isHovered = false;
                shape.setFillColor(normalColor);
            }
        }
    }

    bool handleClick(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (onClick) onClick();
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(pos.x + shape.getSize().x / 2, pos.y + shape.getSize().y / 2 - 3);
    }
    const sf::Font& getFont() const { 
    return *text.getFont(); 
}
sf::Text& getText() { 
    return text; 
}
};


#endif