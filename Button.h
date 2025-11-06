#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "RounderRectangleShape.h"
#include "Theme.h"

class Button {
private:
    RoundedRectangleShape box;
    sf::Text text;
    bool isHover = false;
    std::function<void()> callback;

public:
    Button() {}

    Button(const sf::Font& font, const sf::String& label, float x, float y) {
        setup(font,label,x,y);
    }

    void setup(const sf::Font& font, const sf::String& label, float x, float y) {
        const sf::Vector2f size(170.f,50.f);

        box.setSize(size);
        box.setRadius(12.f);
        box.setCornerPointCount(10);
        box.setPosition(x,y);

        box.setFillColor( Theme::BtnFill );
        box.setOutlineColor( Theme::BtnBorder );
        box.setOutlineThickness(2.f);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor( Theme::Text );

        auto b = box.getGlobalBounds();
        auto tb = text.getLocalBounds();

        text.setPosition(
            b.left + (b.width  - tb.width )*0.5f - tb.left,
            b.top  + (b.height - tb.height)*0.5f - tb.top - 2.f
        );
    }

    void setLabel(const sf::String& s) {
        text.setString(s);
        auto b = box.getGlobalBounds();
        auto tb = text.getLocalBounds();
        text.setPosition(
            b.left + (b.width  - tb.width )*0.5f - tb.left,
            b.top  + (b.height - tb.height)*0.5f - tb.top - 2.f
        );
    }

    void setPosition(float x,float y){
        box.setPosition(x,y);

        auto b = box.getGlobalBounds();
        auto tb = text.getLocalBounds();
        text.setPosition(
            b.left + (b.width  - tb.width )*0.5f - tb.left,
            b.top  + (b.height - tb.height)*0.5f - tb.top - 2.f
        );
    }

    void setCallback(std::function<void()> fn){ callback = fn; }

    void update(const sf::RenderWindow& w) {
        auto m = sf::Mouse::getPosition(w);
        isHover = box.getGlobalBounds().contains(
            (float)m.x, (float)m.y
        );
        box.setFillColor(isHover ? Theme::BtnHover : Theme::BtnFill);
    }

    void handle(const sf::Event& e) {
        if(!isHover) return;

        if(e.type == sf::Event::MouseButtonPressed &&
           e.mouseButton.button == sf::Mouse::Left)
        {
            if(callback) callback();
        }
    }

    void draw(sf::RenderTarget& t){
        t.draw(box);
        t.draw(text);
    }
};
