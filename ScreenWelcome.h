#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"

class ScreenWelcome : public ScreenBase {
private:
    sf::Font &font;
    Button btnVisit;
    sf::Text title;
    sf::Text sub;

public:
    ScreenWelcome(sf::Font &f)
        : font(f),
          btnVisit("VISIT",f,{440,320},{200,50})
    {
        title.setFont(font);
        title.setString("welcome");
        title.setCharacterSize(72);
        title.setFillColor(Theme::Title);
        title.setPosition(280,120);

        sub.setFont(font);
        sub.setString("Library System");
        sub.setCharacterSize(32);
        sub.setFillColor(Theme::Label);
        sub.setPosition(360,210);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        btnVisit.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x, my=e.mouseButton.y;
            if(btnVisit.hit(mx,my)){
                cur = SCREEN_ROLE;
            }
        }
    }

    void update() override {
        sf::Vector2i m = sf::Mouse::getPosition();
        btnVisit.update((float)m.x,(float)m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        w.draw(title);
        w.draw(sub);
        btnVisit.draw(w);
    }
};

#endif