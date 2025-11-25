#ifndef INPUT_FIELD_HPP
#define INPUT_FIELD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class InputField {
private:
    RoundedRectangleShape box; sf::Text text, placeholder;
    std::string inputString; bool isFocused, isPassword; int limit;

public:
    InputField(sf::Vector2f pos, sf::Vector2f size, const std::string& placeH, sf::Font& font, bool pass = false) {
        box.setSize(size); box.setCornerRadius(8.0f); box.setPosition(pos);
        box.setFillColor(sf::Color::White); box.setOutlineThickness(2); box.setOutlineColor(Theme::Border);

        isPassword = pass; isFocused = false; inputString = ""; limit = 30;

        text.setFont(font); text.setCharacterSize(18); text.setFillColor(Theme::TextDark);
        text.setPosition(pos.x + 15, pos.y + size.y/2 - 12);

        placeholder.setFont(font); placeholder.setString(placeH); placeholder.setCharacterSize(18);
        placeholder.setFillColor(sf::Color(180, 180, 180)); // Màu xám nhạt cho placeholder
        placeholder.setPosition(pos.x + 15, pos.y + size.y/2 - 12);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (box.getGlobalBounds().contains(mousePos)) { isFocused = true; box.setOutlineColor(Theme::Secondary); } 
            else { isFocused = false; box.setOutlineColor(Theme::Border); }
        }
        if (isFocused && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!inputString.empty()) inputString.pop_back();
            } else if (event.text.unicode < 128 && inputString.length() < limit) {
                inputString += static_cast<char>(event.text.unicode);
            }
            
            if (isPassword) text.setString(std::string(inputString.length(), '*')); 
            else text.setString(inputString);
        }
    }

    void update() {} 
    void draw(sf::RenderWindow& window) { window.draw(box); if (inputString.empty()) window.draw(placeholder); else window.draw(text); }
    
    std::string getText() const { return inputString; }
    void setText(const std::string& str) { inputString = str; if (isPassword) text.setString(std::string(inputString.length(), '*')); else text.setString(inputString); }
    void clear() { inputString = ""; text.setString(""); }
    bool hasFocus() const { return isFocused; }
};
#endif