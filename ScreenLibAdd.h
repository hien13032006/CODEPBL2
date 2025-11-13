#ifndef SCREEN_LIB_ADD_H
#define SCREEN_LIB_ADD_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenLibAdd : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxTitle, boxAuthor, boxID;
    Button btnOK, btnBack;
    sf::Text title;

public:
    ScreenLibAdd(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxTitle(f,300,40,false),
          boxAuthor(f,300,40,false),
          boxID(f,300,40,false),
          btnOK("Them",f,{0,0},{200,45}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        title.setFont(font);
        title.setString("Them sach");
        title.setCharacterSize(36);
        title.setFillColor(Theme::Title);
        title.setPosition(220,40);

        boxTitle.setPosition(200,120);
        boxAuthor.setPosition(200,190);
        boxID.setPosition(200,260);

        boxTitle.setPlaceholder("Ten sach...");
        boxAuthor.setPlaceholder("Tac gia...");
        boxID.setPlaceholder("ID...");

        btnOK.setPosition(230,330);
        btnBack.setPosition(230,390);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxTitle.handleEvent(e);
        boxAuthor.handleEvent(e);
        boxID.handleEvent(e);

        btnOK.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type == sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnOK.hit(mx,my)){
                // L->AddBook(...)
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
        boxTitle.draw(w);
        boxAuthor.draw(w);
        boxID.draw(w);

        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif