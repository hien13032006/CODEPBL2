#ifndef SCREEN_LIB_MENU_H
#define SCREEN_LIB_MENU_H

#include "ScreenBase.h"
#include "Button.h"

class ScreenLibMenu : public ScreenBase {
private:
    sf::Font &font;
    Button btnAdd, btnDel, btnUpd, btnView, btnExit;

public:
    ScreenLibMenu(sf::Font &f)
        : font(f),
          btnAdd("Them sach",f,22),
          btnDel("Xoa sach",f,22),
          btnUpd("Cap nhat",f,22),
          btnView("Doc gia",f,22),
          btnExit("Thoat",f,22)
    {
        float x = 220,y = 160;
        float dy = 55;

        Button *arr[5] = {&btnAdd,&btnDel,&btnUpd,&btnView,&btnExit};
        for(int i=0;i<5;i++){
            arr[i]->setSize(200,45);
            arr[i]->setPosition(x,y+i*dy);
        }
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;

            if(btnAdd.hit(mx,my)) cur = SCREEN_LIB_ADD;
            if(btnDel.hit(mx,my)) cur = SCREEN_LIB_DELETE;
            if(btnUpd.hit(mx,my)) cur = SCREEN_LIB_UPDATE;
            if(btnView.hit(mx,my)) cur = SCREEN_VIEW_READER;
            if(btnExit.hit(mx,my)) cur = SCREEN_ROLE;
        }
    }

    void update() override {}
    void draw(sf::RenderWindow &w) override {
        btnAdd.draw(w);
        btnDel.draw(w);
        btnUpd.draw(w);
        btnView.draw(w);
        btnExit.draw(w);
    }
};

#endif