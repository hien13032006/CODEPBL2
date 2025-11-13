#ifndef SCREEN_RETURN_BOOK_H
#define SCREEN_RETURN_BOOK_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "LibrarySystem.h"
#include "Button.h"

class ScreenReturnBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **current;

    TextBox boxID;
    Button btnOK;
    Button btnBack;

public:
    ScreenReturnBook(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), current(cur),
          boxID(f,260,40,false),
          btnOK("Tra",f,22),
          btnBack("Quay lai",f,22)
    {
        boxID.setPosition(190,180);
        boxID.setPlaceholder("ma sach...");

        btnOK.setSize(200,45);
        btnOK.setPosition(220,250);

        btnBack.setSize(200,45);
        btnBack.setPosition(220,310);
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        boxID.handleEvent(e);
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnOK.hit(mx,my)){
                if(*current){
                    L->TraSach(*current, boxID.get());
                }
            }
            if(btnBack.hit(mx,my)){
                cur = SCREEN_READER_MENU;
            }
        }
    }

    void update() override {}
    void draw(sf::RenderWindow &w) override {
        boxID.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif