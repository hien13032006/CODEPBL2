#ifndef SCREEN_LIB_UPDATE_H
#define SCREEN_LIB_UPDATE_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenLibUpdate : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxID, boxTitle, boxAuthor;
    Button btnOK, btnBack;
    sf::Text title;

public:
    ScreenLibUpdate(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxID(f,260,40,false),
          boxTitle(f,260,40,false),
          boxAuthor(f,260,40,false),
          btnOK("Cap nhat",f,{0,0},{200,45}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        title.setFont(font);
        title.setString("Cap nhat sach");
        title.setCharacterSize(36);
        title.setFillColor(Theme::Title);
        title.setPosition(210,40);

        boxID.setPosition(200,120);
        boxTitle.setPosition(200,190);
        boxAuthor.setPosition(200,260);

        boxID.setPlaceholder("Nhap ID...");
        boxTitle.setPlaceholder("Ten moi...");
        boxAuthor.setPlaceholder("Tac gia moi...");

        btnOK.setPosition(230,330);
        btnBack.setPosition(230,390);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxID.handleEvent(e);
        boxTitle.handleEvent(e);
        boxAuthor.handleEvent(e);

        btnOK.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnOK.hit(mx,my)){
                // L->UpdateBook(...)
            }

            if(btnBack.hit(mx,my)) cur = SCREEN_LIB_MENU;
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
        boxTitle.draw(w);
        boxAuthor.draw(w);

        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif