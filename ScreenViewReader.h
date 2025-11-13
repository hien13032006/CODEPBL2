#ifndef SCREEN_VIEW_READER_H
#define SCREEN_VIEW_READER_H

#include "ScreenBase.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenViewReader : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    Button btnBack;
    sf::Text title;

public:
    ScreenViewReader(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          btnBack("Quay lai",f,{240,400},{200,45})
    {
        title.setFont(font);
        title.setString("Danh sach doc gia");
        title.setFillColor(Theme::Title);
        title.setCharacterSize(36);
        title.setPosition(220,60);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        btnBack.handleEvent(e);
        if(e.type==sf::Event::MouseButtonReleased){
            if(btnBack.hit(e.mouseButton.x,e.mouseButton.y))
                cur = SCREEN_LIB_MENU;
        }
    }

    void update() override {
        sf::Vector2i m=sf::Mouse::getPosition();
        btnBack.update(m.x,m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        w.draw(title);
        btnBack.draw(w);
    }
};

#endif