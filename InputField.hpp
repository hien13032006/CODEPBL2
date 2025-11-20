#ifndef INPUT_FIELD_HPP
#define INPUT_FIELD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"

class InputField {
private:
    RoundedRectangleShape box;
    sf::Text text;
    sf::Text placeholder;
    std::string inputText;
    bool isFocused;
    bool isPassword;
    sf::Clock cursorClock;
    bool showCursor;
    sf::Vector2f position;
    sf::Vector2f size;

public:
    InputField(sf::Vector2f pos, sf::Vector2f sz, const std::string& placeholderText,
               sf::Font& font, bool password = false) {
        position = pos;
        size = sz;
        isPassword = password;
        
        box.setSize(sz);
        box.setCornerRadius(8.0f);
        box.setPosition(pos);
        // LIGHT THEME: Background trắng
        box.setFillColor(sf::Color(255, 255, 255));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(220, 220, 220)); // Border xám nhạt

        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color(30, 30, 30)); // Text đen
        text.setPosition(pos.x + 15, pos.y + (sz.y - 20) / 2);

        placeholder.setFont(font);
        placeholder.setString(placeholderText);
        placeholder.setCharacterSize(16);
        placeholder.setFillColor(sf::Color(150, 150, 150)); // Gray placeholder
        placeholder.setPosition(pos.x + 15, pos.y + (sz.y - 20) / 2);

        inputText = "";
        isFocused = false;
        showCursor = false;
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (box.getGlobalBounds().contains(mousePos)) {
                isFocused = true;
                box.setOutlineColor(sf::Color(33, 150, 243)); // Blue khi focus
                box.setOutlineThickness(3);
            } else {
                isFocused = false;
                box.setOutlineColor(sf::Color(220, 220, 220));
                box.setOutlineThickness(2);
            }
        }

        if (isFocused && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && inputText.size() > 0) { 
                inputText.pop_back();
            } 
            else if (event.text.unicode == 13 || event.text.unicode == 9) {
                return;
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                inputText += static_cast<char>(event.text.unicode);
            }
            
            if (isPassword) {
                std::string displayText(inputText.size(), '*');
                text.setString(displayText);
            } else {
                text.setString(inputText);
            }
        }
    }

    void update() {
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        
        if (inputText.empty() && !isFocused) {
            window.draw(placeholder);
        } else {
            window.draw(text);
            
            if (isFocused && showCursor) {
                sf::RectangleShape cursor(sf::Vector2f(2, 20));
                cursor.setFillColor(sf::Color(30, 30, 30));
                sf::FloatRect textBounds = text.getGlobalBounds();
                cursor.setPosition(textBounds.left + textBounds.width + 2, 
                                 text.getPosition().y);
                window.draw(cursor);
            }
        }
    }

    std::string getText() const { return inputText; }
    void clear() { 
        inputText = ""; 
        text.setString("");
    }
    void setFocus(bool focus) { 
        isFocused = focus;
        if (focus) {
            box.setOutlineColor(sf::Color(33, 150, 243));
            box.setOutlineThickness(3);
        } else {
            box.setOutlineColor(sf::Color(220, 220, 220));
            box.setOutlineThickness(2);
        }
    }
    void setPlaceholder(const std::string& newPlaceholder) {
    placeholder.setString(newPlaceholder);
}
    bool hasFocus() const { return isFocused; }
    void setText(const std::string& newText) { 
        inputText = newText; 
        if (isPassword) {
            std::string displayText(inputText.size(), '*');
            text.setString(displayText);
        } else {
            text.setString(inputText);
        }
    }
};

#endif