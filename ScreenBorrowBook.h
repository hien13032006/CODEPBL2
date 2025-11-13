#ifndef SCREEN_BORROW_BOOK_H
#define SCREEN_BORROW_BOOK_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include "Popup.h"
class ScreenBorrowBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **current;

    TextBox boxID;
    Button btnOK;
    Button btnBack;

public:
    Popup popup;
    ScreenBorrowBook(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), current(cur),
          boxID(f,260,40,false),
          btnOK("Muon",f,22),
          btnBack("Quay lai",f,22),
          popup(f)
    {
        boxID.setPosition(190,180);
        boxID.setPlaceholder("ma sach..");

        btnOK.setSize(200,45);
        btnOK.setPosition(220,250);

        btnBack.setSize(200,45);
        btnBack.setPosition(220,310);
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        boxID.handleEvent(e);
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;
            if(btnOK.hit(mx,my)){
                if(*current){
                    L->MuonSach(*current, boxID.get());
                    popup.show("Muon sach thanh cong", Theme::OK);
                } else {
                    popup.show("Ban chua dang nhap", Theme::Danger);
                }
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