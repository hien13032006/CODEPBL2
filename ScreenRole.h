#ifndef SCREEN_ROLE_H
#define SCREEN_ROLE_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"

class ScreenRole : public ScreenBase {
private:
    sf::Font &font;
    Button btnReader;
    Button btnLibrarian;
    sf::Text title;

public:
    ScreenRole(sf::Font &f)
        : font(f),
          btnReader("READER",f,{220,220},{220,70}),
          btnLibrarian("LIBRARIAN",f,{560,220},{220,70})
    {
        title.setFont(font);
        title.setString("Chon phan quyen");
        title.setCharacterSize(32);
        title.setFillColor(Theme::Title);
        title.setPosition(340,120);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        btnReader.handleEvent(e);
        btnLibrarian.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x, my=e.mouseButton.y;
            if(btnReader.hit(mx,my))  cur = SCREEN_READER_CHOICE;
            if(btnLibrarian.hit(mx,my)) cur = SCREEN_LIB_LOGIN;
        }
    }

    void update() override {
        sf::Vector2i m = sf::Mouse::getPosition();
        btnReader.update((float)m.x,(float)m.y);
        btnLibrarian.update((float)m.x,(float)m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        w.draw(title);
        btnReader.draw(w);
        btnLibrarian.draw(w);
    }
};

#endif