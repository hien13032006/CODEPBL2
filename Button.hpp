#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"
#include "Theme.hpp" // Thêm Theme để lấy màu mặc định nếu cần

class Button {
private:
    RoundedRectangleShape shape;
    sf::Text text;
    sf::Color normalColor, hoverColor;
    bool isHovered;
    int id;

public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label, sf::Font& font, int btnId = 0, sf::Color color = Theme::Primary) {
        shape.setSize(size); shape.setCornerRadius(10.0f); shape.setPosition(position);
        normalColor = color;
        // Tạo màu hover sáng hơn một chút
        hoverColor = sf::Color(std::min(255, color.r + 30), std::min(255, color.g + 30), std::min(255, color.b + 30));
        shape.setFillColor(normalColor);
        id = btnId; isHovered = false;

        text.setFont(font); 
        text.setString(sf::String::fromUtf8(label.begin(), label.end())); 
        text.setCharacterSize(18); text.setFillColor(sf::Color::White);
        centerText();
    }

    void centerText() {
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width/2.0f, b.top + b.height/2.0f);
        text.setPosition(shape.getPosition().x + shape.getSize().x/2.0f, shape.getPosition().y + shape.getSize().y/2.0f);
    }

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) { isHovered = true; shape.setFillColor(hoverColor); }
        } else {
            if (isHovered) { isHovered = false; shape.setFillColor(normalColor); }
        }
    }

    bool handleClick(sf::Vector2f mousePos) { return shape.getGlobalBounds().contains(mousePos); }
    void draw(sf::RenderWindow& window) { window.draw(shape); window.draw(text); }
    
    // Getters/Setters
    int getId() const { return id; }
    sf::Text& getText() { return text; }
    std::string getTextString() const { return text.getString().toAnsiString(); }
    
    // [QUAN TRỌNG] Hàm này để sửa lỗi "has no member getFont"
    const sf::Font& getFont() const { return *text.getFont(); }

    void setColor(sf::Color color) { normalColor = color; hoverColor = sf::Color(std::min(255, color.r + 30), std::min(255, color.g + 30), std::min(255, color.b + 30)); if(!isHovered) shape.setFillColor(normalColor); }
    void setPosition(sf::Vector2f pos) { shape.setPosition(pos); centerText(); }
    void setCornerRadius(float r) { shape.setCornerRadius(r); }
    void setTextColor(sf::Color c) { text.setFillColor(c); }
    void setTextSize(int size) { text.setCharacterSize(size); centerText(); }
};
#endif