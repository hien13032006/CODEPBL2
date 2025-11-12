#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "RounderRectangleShape.h"

class Button {
private:
    RoundedRectangleShape box;
    sf::Text text;
    bool isHover = false;
    bool isPressed = false;
    std::function<void()> callback;

    sf::Color normalColor = sf::Color(200, 200, 200);
    sf::Color hoverColor = sf::Color(180, 180, 255);
    sf::Color pressColor = sf::Color(150, 150, 255);
    sf::Color textColor = sf::Color::Black;

public:

    Button() {}

    Button(const std::string &label, sf::Font &f, int size = 20) {
        text.setFont(f);
        text.setString(label);
        text.setCharacterSize(size);
}

    Button(const std::string& label, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
        box = RoundedRectangleShape(size, 12.f); // <--- bo tròn góc 12px
        box.setFillColor(sf::Color(200, 200, 200));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        box.setPosition(pos);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
        text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
    }

    void draw(sf::RenderWindow &w) {
        w.draw(box);
        w.draw(text);
    }

    bool hit(float x, float y) {
        return box.getGlobalBounds().contains({x,y});
    }

    void setSize(float w, float h) {
        box.setSize(sf::Vector2f(w, h));
    }

    void setPosition(float x, float y) {
        box.setPosition(x, y);
        text.setPosition(x + 10, y + 10);
   }

    bool contains(const sf::Vector2f &point) const {
        return box.getGlobalBounds().contains(point);
    }

    void setCallback(std::function<void()> f) {
        callback = f;
    }

    void checkClick(const sf::Event &e, sf::RenderWindow &window) {
        if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
            if(box.getGlobalBounds().contains(mousePos)) {
                if(callback) callback();
            }
        }
    }

    void handleEvent(const sf::Event& e, sf::RenderWindow& w) {
        sf::Vector2f mousePos = w.mapPixelToCoords(sf::Mouse::getPosition(w));
        bool inside = box.getGlobalBounds().contains(mousePos);

        isHover = inside;

        if (inside && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            isPressed = true;
            box.setFillColor(pressColor);
        }
        else if (inside && e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
            if (isPressed && callback) callback();
            isPressed = false;
        }
    }

    void update() {
        if (isPressed)
            box.setFillColor(pressColor);
        else if (isHover)
            box.setFillColor(hoverColor);
        else
            box.setFillColor(normalColor);
    }
};

#endif