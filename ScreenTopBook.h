#ifndef SCREEN_TOP_BOOK_H
#define SCREEN_TOP_BOOK_H

#include "ScreenBase.h"
#include "LibrarySystem.h"
#include "ListView.h"
#include "Button.h"

class ScreenTopBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    ListView list;
    Button btnBack;

public:
    ScreenTopBook(sf::Font &f, LibrarySystem *lib)
        : font(f),L(lib),list(f,500,380),btnBack("Quay lai",f,22)
    {
        list.setPosition(70,60);
        btnBack.setSize(200,45);
        btnBack.setPosition(220,460);
    }

    void load(){
        list.clear();
        L->XepHangSach();
        // data đã được in console
        // UI chỉ show basic
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        list.handleScroll(e);

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnBack.hit(mx,my)){
                cur = SCREEN_READER_MENU;
            }
        }
    }

    void update() override {}
    void draw(sf::RenderWindow &w) override {
        list.draw(w);
        btnBack.draw(w);
    }
};

#endif