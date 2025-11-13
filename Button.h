#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "Theme.h"

class Button {
private:
    sf::RectangleShape box;
    sf::Text text;
    bool isDown = false;

public:
    Button() {}

    Button(const std::string &label, sf::Font &font,
           sf::Vector2f pos, sf::Vector2f size)
    {
        box.setSize(size);
        box.setPosition(pos);
        box.setFillColor(Theme::BtnNormal);
        box.setOutlineColor(Theme::Border);
        box.setOutlineThickness(1);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(22);
        text.setFillColor(Theme::BtnText);

        centerText();
    }

    void setText(const std::string &s) {
        text.setString(s);
        centerText();
    }

    void setPosition(float x, float y) {
        box.setPosition(x,y);
        centerText();
    }

    void setSize(float w, float h) {
        box.setSize({w,h});
        centerText();
    }

    void centerText() {
        auto b = text.getLocalBounds();
        text.setOrigin(b.left + b.width/2, b.top + b.height/2);
        text.setPosition(
            box.getPosition().x + box.getSize().x/2,
            box.getPosition().y + box.getSize().y/2
        );
    }

    bool hit(float mx,float my) const {
        return box.getGlobalBounds().contains(mx,my);
    }

    void handleEvent(const sf::Event &e) {
        if(e.type == sf::Event::MouseButtonPressed){
            if(hit(e.mouseButton.x,e.mouseButton.y)){
                isDown = true;
                box.setFillColor(Theme::BtnClick);
            }
        }
        if(e.type == sf::Event::MouseButtonReleased){
            isDown = false;
            box.setFillColor(Theme::BtnNormal);
        }
    }

    void update(float mx,float my) {
        if(hit(mx,my) && !isDown)
            box.setFillColor(Theme::BtnHover);
        else if(!isDown)
            box.setFillColor(Theme::BtnNormal);
    }

    void draw(sf::RenderWindow &w) {
        w.draw(box);
        w.draw(text);
    }
};

#endif