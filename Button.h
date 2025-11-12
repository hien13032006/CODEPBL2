#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    sf::RectangleShape box;
    sf::Text text;

    Button() {}

    Button(const std::string &label, sf::Font &f,
           sf::Vector2f pos, sf::Vector2f size)
    {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color(70,120,200));

        text.setFont(f);
        text.setString(label);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(20);
        text.setPosition(pos.x + 10, pos.y + 8);
    }

    void draw(sf::RenderWindow &w) {
        w.draw(box);
        w.draw(text);
    }

    bool hit(float x, float y) {
        return box.getGlobalBounds().contains({x,y});
    }
};

#endif