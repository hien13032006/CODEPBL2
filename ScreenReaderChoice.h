#ifndef SCREEN_READER_CHOICE_H
#define SCREEN_READER_CHOICE_H

#include "ScreenBase.h"
#include "Button.h"

class ScreenReaderChoice : public ScreenBase {
private:
    sf::Font &font;
    Button btnLogin;
    Button btnReg;
    Button btnBack;

public:
    ScreenReaderChoice(sf::Font &f)
        : font(f),
          btnLogin("Dang nhap", f, {0,0}, {250,50}),
          btnReg("Dang ky", f, {0,0}, {250,50}),
          btnBack("Quay lai", f, {0,0}, {250,50})
    {
        btnLogin.setPosition(200,180);
        btnReg.setPosition(200,260);
        btnBack.setPosition(200,340);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        if(e.type == sf::Event::MouseButtonPressed) {
            float mx=e.mouseButton.x, my=e.mouseButton.y;

            if(btnLogin.hit(mx,my)) cur = SCREEN_READER_LOGIN;
            if(btnReg.hit(mx,my))   cur = SCREEN_READER_REGISTER;
            if(btnBack.hit(mx,my))  cur = SCREEN_ROLE;
        }
    }

    void draw(sf::RenderWindow &w) override {
        btnLogin.draw(w);
        btnReg.draw(w);
        btnBack.draw(w);
    }
};

#endif