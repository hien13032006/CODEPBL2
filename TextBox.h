#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include "Theme.h"

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Text placeholder;
    bool focused = false;
    bool passwordMode = false;
    std::string value;

public:
    TextBox() {}

    TextBox(sf::Font &font, float w, float h, bool isPassword=false)
    {
        box.setSize({w,h});
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(Theme::Border);

        text.setFont(font);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::Black);

        placeholder.setFont(font);
        placeholder.setCharacterSize(22);
        placeholder.setFillColor(sf::Color(150,150,150));

        passwordMode = isPassword;
    }

    void setPosition(float x,float y){
        box.setPosition(x,y);
        text.setPosition(x+10,y+7);
        placeholder.setPosition(x+10,y+7);
    }

    void setPlaceholder(const std::string &s){
        placeholder.setString(s);
    }

    void toggleShow(){
        passwordMode = !passwordMode;
        refreshText();
    }

    bool isShown() const { return !passwordMode; }

    std::string get() const { return value; }

    void refreshText(){
        if(passwordMode){
            text.setString(std::string(value.size(),'●'));
        }else{
            text.setString(value);
        }
    }

    void handleEvent(sf::Event &e){
        if(e.type == sf::Event::MouseButtonPressed){
            focused = box.getGlobalBounds()
                        .contains(e.mouseButton.x,e.mouseButton.y);
            box.setOutlineColor(focused?Theme::BorderFocus:Theme::Border);
        }
        if(focused && e.type == sf::Event::TextEntered){
            if(e.text.unicode == 8){
                if(!value.empty()) value.pop_back();
            }
            else if(e.text.unicode >= 32 && e.text.unicode <= 126){
                value.push_back((char)e.text.unicode);
            }
            refreshText();
        }
    }

    void draw(sf::RenderWindow &w){
        w.draw(box);
        if(value.empty()) w.draw(placeholder);
        else w.draw(text);
    }
};

#endif