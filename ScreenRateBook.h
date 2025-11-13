#ifndef SCREEN_RATE_BOOK_H
#define SCREEN_RATE_BOOK_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenRateBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **currentReader;

    TextBox boxID, boxScore;
    Button btnOK, btnBack;
    sf::Text title;

public:
    ScreenRateBook(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), currentReader(cur),
          boxID(f,260,40,false),
          boxScore(f,260,40,false),
          btnOK("Danh gia",f,{0,0},{180,45}),
          btnBack("Quay lai",f,{0,0},{180,45})
    {
        title.setFont(font);
        title.setString("Danh gia sach");
        title.setFillColor(Theme::Title);
        title.setCharacterSize(36);
        title.setPosition(200,50);

        boxID.setPosition(200,150);
        boxScore.setPosition(200,220);

        boxID.setPlaceholder("ID sach...");
        boxScore.setPlaceholder("Diem (1-5)...");

        btnOK.setPosition(230,290);
        btnBack.setPosition(230,350);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxID.handleEvent(e);
        boxScore.handleEvent(e);
        btnOK.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnOK.hit(mx,my)){
                // L->RateBook(*currentReader, boxID.get(), stoi(boxScore.get()));
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
        boxScore.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif