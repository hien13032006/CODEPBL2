#ifndef POPUP_H
#define POPUP_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.h"

class Popup {
private:
    sf::Font &font;
    sf::Text text;
    sf::RectangleShape box;
    bool visible;
    float timer; // thời gian đếm auto close

public:
    
    Popup(sf::Font &f)
        : font(f), visible(false), timer(0)
    {
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        box.setSize({360,130});
        box.setFillColor(sf::Color(0,0,0,180));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::White);
    }

    void show(const std::string &msg, sf::Color color = Theme::BG, float duration = 2.5f){
        text.setString(msg);
        text.setFillColor(sf::Color::White);

        box.setFillColor(color);

        // Center
        float x = 320 - box.getSize().x/2;
        float y = 260 - box.getSize().y/2;
        box.setPosition(x,y);

        text.setPosition(
            x + (box.getSize().x - text.getLocalBounds().width)/2,
            y + (box.getSize().y - text.getLocalBounds().height)/2 - 5
        );

        visible = true;
        timer = duration;
    }

    bool isOpen() const {
        return visible;
    }

    void close(){
        visible = false;
    }

    void update(float dt){
        if(!visible) return;
        timer -= dt;
        if(timer <= 0){
            visible = false;
        }
    }

    void handleEvent(sf::Event &e){
        if(!visible) return;
        if(e.type == sf::Event::MouseButtonPressed){
            close();
        }
        if(e.type == sf::Event::KeyPressed){
            close();
        }
    }

    void draw(sf::RenderWindow &w){
        if(!visible) return;
        w.draw(box);
        w.draw(text);
    }
};

#endif