#ifndef SCREEN_LIB_DELETE_H
#define SCREEN_LIB_DELETE_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenLibDelete : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxID;
    Button btnOK, btnBack;
    sf::Text title;

public:
    ScreenLibDelete(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxID(f,260,40,false),
          btnOK("Xoa",f,{0,0},{200,45}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        title.setFont(font);
        title.setString("Xoa sach");
        title.setFillColor(Theme::Title);
        title.setCharacterSize(36);
        title.setPosition(230,40);

        boxID.setPosition(200,150);
        boxID.setPlaceholder("Nhap ID...");

        btnOK.setPosition(230,220);
        btnBack.setPosition(230,280);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxID.handleEvent(e);

        btnOK.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnOK.hit(mx,my)){
                // L->DeleteBook(boxID.get());
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
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif