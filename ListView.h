#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <SFML/Graphics.hpp>
#include <string>

class ListView {
private:
    sf::Font &font;
    float x, y;
    float w, h;
    float lineHeight;
    float scroll;
    sf::RectangleShape box;

    std::string lines[200];
    int lineCount;

public:


    ListView(sf::Font &f, float width, float height)
        : font(f), w(width), h(height), lineHeight(26), scroll(0), lineCount(0)
    {
        x = y = 0;
        box.setSize({w,h});
        box.setFillColor(sf::Color(245,245,245));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(1);
    }

    void setPosition(float px, float py){
        x = px;
        y = py;
        box.setPosition(x,y);
    }

    void clear(){
        lineCount = 0;
    }

    void addLine(const std::string &s){
        if(lineCount < 200){
            lines[lineCount++] = s;
        }
    }

    void handleScroll(sf::Event &e){
        if(e.type == sf::Event::MouseWheelScrolled){
            scroll -= e.mouseWheelScroll.delta * 20;
            if(scroll < 0) scroll = 0;
        }
    }

    void draw(sf::RenderWindow &w){
        w.draw(box);

        float yy = y + 4 - scroll;
        for(int i=0; i<lineCount; i++){
            if(yy > y - lineHeight && yy < y + h){
                sf::Text t(lines[i], font, 18);
                t.setFillColor(sf::Color::Black);
                t.setPosition(x + 6, yy);
                w.draw(t);
            }
            yy += lineHeight;
        }
    }
};

#endif