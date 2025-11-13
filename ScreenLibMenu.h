#ifndef SCREEN_LIB_MENU_H
#define SCREEN_LIB_MENU_H

#include "ScreenBase.h"
#include "Button.h"

class ScreenLibMenu : public ScreenBase {
private:
    Button btnAdd, btnSear, btnDel, btnUpd, btnView, btnBook, btnExit;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:

    ScreenLibMenu(sf::Font &f, sf::RenderWindow *w)  {
        if(!bgTexture.loadFromFile("picLibrarian.png")) { // tên file ảnh
            bgTexture.create(1024,768); // fallback nếu không có ảnh
        }
        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = w->getSize();
        bgSprite.setScale(
            float(winSize.x)/bgTexture.getSize().x,
            float(winSize.y)/bgTexture.getSize().y
        );

        float x = 800, y = 250, dy = 120;
        btnAdd = Button("THEM SACH MOI", f, {x,y},{300,70});
        btnSear = Button("TIM KIEM SACH", f, {x,y+dy},{300,70});
        btnDel = Button("XOA SACH", f,{x,y+dy*2},{300,70});
        btnUpd = Button("CAP NHAT THONG TIN SACH", f,{x,y+dy*3},{300,70});
        btnView = Button("DANH SACH DOC GIA", f,{x,y+dy*4},{300,70});
        btnBook = Button("DANH SACH SACH", f,{x,y+dy*5},{300,70});
        btnExit = Button("THOAT", f,{x,y+dy*6},{300,70});
    }

    // Sửa đúng chữ ký
    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow* window) override {
        if(e.type == sf::Event::MouseButtonPressed){
            float mx = e.mouseButton.x;
            float my = e.mouseButton.y;

            if(btnAdd.hit(mx,my)) cur = SCREEN_LIB_ADD;
            if(btnSear.hit(mx,my)) cur = SCREEN_SEARCH_BOOK;
            if(btnDel.hit(mx,my)) cur = SCREEN_LIB_DELETE;
            if(btnUpd.hit(mx,my)) cur = SCREEN_LIB_UPDATE;
            if(btnView.hit(mx,my)) cur = SCREEN_VIEW_READER;
            if(btnBook.hit(mx,my)) cur = SCREEN_VIEW_BOOK;
            if(btnExit.hit(mx,my)) cur = SCREEN_WELCOME;
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        btnAdd.draw(w);
        btnSear.draw(w);
        btnDel.draw(w);
        btnUpd.draw(w);
        btnView.draw(w);
        btnBook.draw(w);
        btnExit.draw(w);
    }
};

#endif
