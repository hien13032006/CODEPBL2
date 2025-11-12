#ifndef SCREEN_VIEW_BOOK_H
#define SCREEN_VIEW_BOOK_H

#include "ScreenBase.h"
#include "LibrarySystem.h"
#include "ListView.h"
#include "Button.h"

class ScreenViewBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    ListView list;
    Button btnBack;

public:
    ScreenViewBook(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib), list(f,500,380), btnBack("Quay lai",f,22)
    {
        list.setPosition(70,60);
        btnBack.setSize(200,45);
        btnBack.setPosition(220,460);
    }

    void load(){
        list.clear();
        NodeBook *p = L->getHeadBook();
        while(p){
            list.addLine(
                p->data->getMaSach() + " | " +
                p->data->getTenSach() + " | " +
                p->data->getTacGia()
            );
            p = p->next;
        }
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
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