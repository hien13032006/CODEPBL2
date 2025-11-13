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
    sf::Color hoverColor  = sf::Color(180, 180, 255);
    sf::Color pressColor  = sf::Color(150, 150, 255);
    sf::Color textColor   = sf::Color::Black;

public:
    Button() {}

    // ✅ Giữ lại constructor 3 tham số cũ (dành cho các màn hình cũ)
    Button(const std::string &label, sf::Font &f, int size = 20) {
        text.setFont(f);
        text.setString(label);
        text.setCharacterSize(size);
        text.setFillColor(textColor);

        box = RoundedRectangleShape({200, 50}, 12.f);
        box.setFillColor(normalColor);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
    }

    // ✅ Constructor mới hỗ trợ vị trí và kích thước
    Button(const std::string& label, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
        box = RoundedRectangleShape(size, 12.f); // bo góc 12px
        box.setFillColor(normalColor);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        box.setPosition(pos);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(textColor);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
    }

    // ✅ Các hàm được dùng nhiều nơi
    void setSize(float w, float h) { box.setSize({w, h}); }
    void setPosition(float x, float y) { box.setPosition(x, y); text.setPosition(x + 10, y + 10); }
    bool contains(const sf::Vector2f &point) const { return box.getGlobalBounds().contains(point); }

    // ✅ Giữ lại để các màn hình cũ dùng
    bool hit(float x, float y) { return box.getGlobalBounds().contains({x, y}); }

    void setCallback(std::function<void()> f) { callback = std::move(f); }

    // ✅ Dành cho các màn hình cũ (vẫn dùng checkClick)
    void checkClick(const sf::Event &e, sf::RenderWindow &window) {
        if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
            if(box.getGlobalBounds().contains(mousePos)) {
                if(callback) callback();
            }
        }
    }

    // ✅ Tương tác hover / click
    void handleEvent(const sf::Event &e, sf::RenderWindow &w) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(w); // mouse relative to this window
        sf::Vector2f mousePos = w.mapPixelToCoords(pixelPos);
        bool inside = box.getGlobalBounds().contains(mousePos);

        if (inside) {
            isHover = true;
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                isPressed = true;
                if (callback) callback();
            }
        } else {
            isHover = false;
        }

        if (e.type == sf::Event::MouseButtonReleased)
            isPressed = false;
    }

    void update() {
        if (isPressed)
            box.setFillColor(pressColor);
        else if (isHover)
            box.setFillColor(hoverColor);
        else
            box.setFillColor(normalColor);
    }

    void draw(sf::RenderWindow &w) {
        w.draw(box);
        w.draw(text);
    }
};

#endif
