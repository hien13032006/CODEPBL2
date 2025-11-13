#ifndef SCREEN_LIB_LOGIN_H
#define SCREEN_LIB_LOGIN_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenLibLogin : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    USER **currentUser;

    TextBox boxUser;
    TextBox boxPass;
    Button btnOK;
    Button btnShow;
    Button btnBack;

public:
    ScreenLibLogin(sf::Font &f, LibrarySystem *lib, USER **cur)
        : font(f),
          L(lib),
          currentUser(cur),
          boxUser(f,260,40,false),
          boxPass(f,260,40,true),
          btnOK("Dang nhap",f,22),
          btnShow("show",f,18),
          btnBack("Quay lai",f,22)
    {
        boxUser.setPosition(190,150);
        boxPass.setPosition(190,220);

        boxUser.setPlaceholder("ten dang nhap...");
        boxPass.setPlaceholder("mat khau...");

        btnOK.setSize(200,45);
        btnOK.setPosition(220,290);

        btnShow.setSize(70,40);
        btnShow.setPosition(460,220);

        btnBack.setSize(200,45);
        btnBack.setPosition(220,360);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxUser.handleEvent(e);
        boxPass.handleEvent(e);

        if(e.type == sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;

            if(btnShow.hit(mx,my)){
                boxPass.toggleShow();
                btnShow.setText( boxPass.isShown() ? "hide" : "show" );
            }

            if(btnOK.hit(mx,my)){
                USER *usr=nullptr;
                if(L->DangNhapThuThu(boxUser.get(),boxPass.get(),usr)){
                    *currentUser = usr;
                    cur = SCREEN_LIB_MENU;
                }
            }

            if(btnBack.hit(mx,my)){
                cur = SCREEN_ROLE;
            }
        }
    }
    
    void update() override {}

    void draw(sf::RenderWindow &w) override {
        boxUser.draw(w);
        boxPass.draw(w);

        btnOK.draw(w);
        btnShow.draw(w);
        btnBack.draw(w);
    }
};

#endif