#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"

class ScreenWelcome : public ScreenBase {
private:
    sf::Font &font;
    Button btnVisit;
    sf::Sprite bg;
    bool hasBG;

public:
    ScreenWelcome(sf::Font &f)
        : font(f), btnVisit("VISIT",f,26)
    {
        btnVisit.setSize(180,55);
        btnVisit.setPosition(230,360);

        hasBG = false;
    }

    void setBackground(sf::Texture &t){
        bg.setTexture(t);
        hasBG = true;
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        if(e.type == sf::Event::MouseButtonPressed){
            float mx = e.mouseButton.x;
            float my = e.mouseButton.y;
            if(btnVisit.hit(mx,my)){
                cur = SCREEN_ROLE;
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        if(hasBG) w.draw(bg);
        btnVisit.draw(w);
    }
};

#endif