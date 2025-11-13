#ifndef SCREEN_RETURN_BOOK_H
#define SCREEN_RETURN_BOOK_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenReturnBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **currentReader;

    TextBox boxID;
    Button btnOK, btnBack;
    sf::Text title;

public:
    ScreenReturnBook(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), currentReader(cur),
          boxID(f,260,40,false),
          btnOK("Tra",f,{0,0},{150,45}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        title.setFont(font);
        title.setString("Tra sach");
        title.setFillColor(Theme::Title);
        title.setCharacterSize(36);
        title.setPosition(200,60);

        boxID.setPosition(200,150);
        boxID.setPlaceholder("Nhap ID...");

        btnOK.setPosition(230,220);
        btnBack.setPosition(230,290);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxID.handleEvent(e);

        btnOK.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnOK.hit(mx,my)){
                // L->ReturnBook(*currentReader, boxID.get());
            }

            if(btnBack.hit(mx,my)) cur = SCREEN_READER_MENU;
        }
    }

    void update() override {
        sf::Vector2i m=sf::Mouse::getPosition();
        btnOK.update(m.x,m.y);
        btnBack.update(m.x,m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        w.draw(title);
        boxID.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif