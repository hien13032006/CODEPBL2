#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#ifndef NOMINMAX
#define NOMINMAX
#include <SFML/Graphics.hpp>
#include <string>

class UIButton {
    sf::RectangleShape rect;
    sf::Text label;
    sf::Color baseColor;
    sf::Color hoverColor;
    bool isHover;

public:
    UIButton(){ isHover = false; }

    void init(sf::Font &font, float x, float y, float w, float h, std::string text){
        rect.setSize({w,h});
        rect.setPosition(x,y);

        baseColor  = sf::Color(150,180,255);
        hoverColor = sf::Color(100,140,255);
        rect.setFillColor(baseColor);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::Black);
        label.setPosition(x+10, y+10);
    }

    bool hit(float mx,float my){
        auto p = rect.getPosition();
        auto s = rect.getSize();
        return (mx>=p.x && mx<=p.x+s.x && my>=p.y && my<=p.y+s.y);
    }

    void updateHover(float mx,float my){
        if(hit(mx,my)){
            rect.setFillColor(hoverColor);
            isHover = true;
        } else {
            rect.setFillColor(baseColor);
            isHover = false;
        }
    }

    void draw(sf::RenderWindow &w){
        w.draw(rect);
        w.draw(label);
    }
};

#endif
