#ifndef SCREEN_TOP_BOOK_H
#define SCREEN_TOP_BOOK_H

#include "ScreenBase.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenTopBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    Button btnBack;
    sf::Text title;

public:
    ScreenTopBook(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          btnBack("Quay lai",f,{240,400},{200,45})
    {
        title.setFont(font);
        title.setString("Top sach duoc yeu thich");
        title.setCharacterSize(36);
        title.setFillColor(Theme::Title);
        title.setPosition(150,60);
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