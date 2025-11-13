#ifndef SCREEN_HISTORY_H
#define SCREEN_HISTORY_H

#include "ScreenBase.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenHistory : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **currentReader;

    Button btnBack;
    sf::Text title;

public:
    ScreenHistory(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), currentReader(cur),
          btnBack("Quay lai",f,{240,350},{200,45})
    {
        title.setFont(font);
        title.setString("Lich su muon - tra");
        title.setCharacterSize(36);
        title.setFillColor(Theme::Title);
        title.setPosition(190,50);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnBack.hit(mx,my)) cur = SCREEN_READER_MENU;
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