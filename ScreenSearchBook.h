#ifndef SCREEN_SEARCH_BOOK_H
#define SCREEN_SEARCH_BOOK_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenSearchBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxKey;
    Button btnFind, btnBack;
    sf::Text title;

public:
    ScreenSearchBook(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxKey(f,260,40,false),
          btnFind("Tim",f,{0,0},{150,45}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        title.setFont(font);
        title.setString("Tim sach");
        title.setFillColor(Theme::Title);
        title.setCharacterSize(36);
        title.setPosition(200,60);

        boxKey.setPosition(200,150);
        boxKey.setPlaceholder("Nhap tu khoa...");

        btnFind.setPosition(230,220);
        btnBack.setPosition(230,290);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxKey.handleEvent(e);

        btnFind.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnFind.hit(mx,my)){
                // Placeholder logic
                // Bạn có thể đặt kết quả vào một biến hiển thị
            }

            if(btnBack.hit(mx,my)) cur = SCREEN_READER_MENU;
        }
    }

    void update() override {
        sf::Vector2i m = sf::Mouse::getPosition();
        btnFind.update(m.x,m.y);
        btnBack.update(m.x,m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        w.draw(title);
        boxKey.draw(w);
        btnFind.draw(w);
        btnBack.draw(w);
    }
};

#endif