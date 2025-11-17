#ifndef INPUT_FIELD_HPP
#define INPUT_FIELD_HPP

#include <SFML/Graphics.hpp>
#include <string>

class InputField {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Text placeholder;
    std::string inputText;
    bool isFocused;
    bool isPassword;
    sf::Clock cursorClock;
    bool showCursor;

public:
    InputField(sf::Vector2f position, sf::Vector2f size, const std::string& placeholderText,
               sf::Font& font, bool password = false) {
        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(sf::Color(30, 33, 45));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(50, 53, 65));

        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(position.x + 15, position.y + 12);

        placeholder.setFont(font);
        placeholder.setString(placeholderText);
        placeholder.setCharacterSize(16);
        placeholder.setFillColor(sf::Color(100, 100, 100));
        placeholder.setPosition(position.x + 15, position.y + 12);

        inputText = "";
        isFocused = false;
        isPassword = password;
        showCursor = false;
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (box.getGlobalBounds().contains(mousePos)) {
                isFocused = true;
                box.setOutlineColor(sf::Color(255, 193, 94));
            } else {
                isFocused = false;
                box.setOutlineColor(sf::Color(50, 53, 65));
            }
        }

        if (isFocused && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && inputText.size() > 0) {
                inputText.pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode > 31) {
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
                cursor.setFillColor(sf::Color::White);
                sf::FloatRect textBounds = text.getGlobalBounds();
                cursor.setPosition(textBounds.left + textBounds.width + 2, 
                                 text.getPosition().y);
                window.draw(cursor);
            }
        }
    }

    std::string getText() const { return inputText; }
    void clear() { inputText = ""; text.setString(""); }
    void setFocus(bool focus) { isFocused = focus; }
};

#endif