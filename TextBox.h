#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    bool focused = false;
    bool password = false;
    std::string value;

public:
    TextBox() {}

    TextBox(sf::Font &font, sf::Vector2f pos, sf::Vector2f size) {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(20);
        text.setPosition(pos.x + 6, pos.y + 8);
    }

    void setPassword(bool p) { password = p; }
    void setPosition(float x, float y) {
        box.setPosition({x,y});
        text.setPosition(x+6, y+6);
    }

    std::string get() { return value; }

    void handleEvent(sf::Event &e) {
        if (e.type == sf::Event::MouseButtonPressed) {
            auto m = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            focused = box.getGlobalBounds().contains(m);
        }

        if (e.type == sf::Event::TextEntered && focused) {
            if (e.text.unicode == 8) {     // backspace
                if (!value.empty()) value.pop_back();
            } else if (e.text.unicode >= 32 && e.text.unicode < 127) {
                value.push_back((char)e.text.unicode);
            }
            text.setString(password ? std::string(value.size(), '*') : value);
        }
    }

    void draw(sf::RenderWindow &w) {
        w.draw(box);
        w.draw(text);
    }
};

#endif