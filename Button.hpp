#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"

class Button {
private:
    RoundedRectangleShape shape; 
    sf::Text text;
    sf::Color normalColor, hoverColor;
    bool isHovered;
    int id;

public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label,
           sf::Font& font, int btnId = 0, sf::Color color = sf::Color(70, 130, 180)) {
        
        shape.setSize(size);
        shape.setCornerRadius(10.0f);
        shape.setPosition(position);
        
        normalColor = color;
        hoverColor = sf::Color(std::min(255, color.r + 30), std::min(255, color.g + 30), std::min(255, color.b + 30));
        
        shape.setFillColor(normalColor);
        id = btnId;
        isHovered = false;

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width/2.0f, b.top + b.height/2.0f);
        text.setPosition(position.x + size.x/2.0f, position.y + size.y/2.0f);
    }

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) { isHovered = true; shape.setFillColor(hoverColor); }
        } else {
            if (isHovered) { isHovered = false; shape.setFillColor(normalColor); }
        }
    }

    bool handleClick(sf::Vector2f mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    // Getter/Setter
    int getId() const { return id; }
    sf::Text& getText() { return text; }
    
    // --- CÁC HÀM ĐƯỢC THÊM ĐỂ SỬA LỖI ---
    
    // Lấy nội dung chuỗi của nút (cho CategoryScreen)
    std::string getTextString() const { 
        return text.getString(); 
    }

    // Lấy font chữ của nút (cho BookDetailScreen)
    const sf::Font& getFont() const {
        return *text.getFont();
    }

    void setColor(sf::Color color) { 
        normalColor = color; 
        hoverColor = sf::Color(std::min(255, color.r + 30), std::min(255, color.g + 30), std::min(255, color.b + 30));
        if(!isHovered) shape.setFillColor(normalColor); 
    }
    
    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width/2.0f, b.top + b.height/2.0f);
        text.setPosition(pos.x + shape.getSize().x/2.0f, pos.y + shape.getSize().y/2.0f);
    }
};

#endif