#ifndef UI_INPUT_H
#define UI_INPUT_H

#include <SFML/Graphics.hpp>
#include <string>

class UIInput {
    std::string value;
    sf::RectangleShape box;
    sf::Text text;
    bool isPassword;
    bool showPassword;
    bool focus;

public:
    UIInput(){
        isPassword=false;
        showPassword=false;
        focus=false;
    }

    void init(sf::Font &font,float x,float y,float w,float h,bool isPass){
        box.setPosition(x,y);
        box.setSize({w,h});
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x+8,y+6);

        isPassword = isPass;
    }

    void setFocus(bool f){
        focus = f;
        if(focus) box.setOutlineColor(sf::Color::Blue);
        else      box.setOutlineColor(sf::Color::Black);
    }

    void handleClick(float mx,float my){
        auto p = box.getPosition();
        auto s = box.getSize();
        setFocus(mx>=p.x && mx<=p.x+s.x && my>=p.y && my<=p.y+s.y);
    }

    void handleText(sf::Event &e){
        if(!focus) return;

        if(e.type==sf::Event::TextEntered){
            char c = (char)e.text.unicode;
            if(c>=32 && c<=126) value.push_back(c);
        }
        if(e.type==sf::Event::KeyPressed){
            if(e.key.code==sf::Keyboard::Backspace && !value.empty())
                value.pop_back();
        }
    }

    void toggleShow(){
        if(isPassword) showPassword = !showPassword;
    }

    std::string get(){ return value; }

    void draw(sf::RenderWindow &w){
        w.draw(box);

        std::string toShow = value;
        if(isPassword && !showPassword) toShow = std::string(value.size(), '*');
        
        text.setString(toShow);
        w.draw(text);
    }
    void drawPanel(sf::RenderWindow &w, float x, float y, float wdt, float hgt){
    sf::RectangleShape panel;
    panel.setSize({wdt,hgt});
    panel.setFillColor(sf::Color(255,255,255,200)); // trắng mờ
    panel.setPosition(x,y);
    panel.setOutlineThickness(3);
    panel.setOutlineColor(sf::Color::Black);
    w.draw(panel);
}

};

#endif