#ifndef SCREEN_READER_LOGIN_H
#define SCREEN_READER_LOGIN_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include "Reader.h"

class ScreenReaderLogin : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **currentReader;

    TextBox boxUser;
    TextBox boxPass;
    Button btnOK, btnShow, btnBack;

public:
    ScreenReaderLogin(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), currentReader(cur),
          boxUser(f,260,40,false),
          boxPass(f,260,40,true),
          btnOK("Dang nhap",f,{0,0},{200,45}),
          btnShow("Show",f,{0,0},{80,40}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        boxUser.setPosition(190,150);
        boxPass.setPosition(190,220);
        boxUser.setPlaceholder("ten dang nhap...");
        boxPass.setPlaceholder("mat khau...");

        btnOK.setPosition(220,290);
        btnShow.setPosition(470,220);
        btnBack.setPosition(220,360);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxUser.handleEvent(e);
        boxPass.handleEvent(e);

        btnOK.handleEvent(e);
        btnShow.handleEvent(e);
        btnBack.handleEvent(e);

        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x, my=e.mouseButton.y;

            if(btnShow.hit(mx,my)){
                boxPass.toggleShow();
                btnShow.setText(boxPass.isShown() ? "Hide" : "Show");
            }

            if(btnOK.hit(mx,my)){
                Reader *u = L->loginReader(boxUser.get(), boxPass.get());
                if(u){
                    *currentReader = u;
                    cur = SCREEN_READER_MENU;   // FIX OK
                }
            }

                        if(btnBack.hit(mx,my)){
                            cur = SCREEN_ROLE;
                        }
        }
    }

    void update() override {
        sf::Vector2i m = sf::Mouse::getPosition();
        btnOK.update((float)m.x,(float)m.y);
        btnShow.update((float)m.x,(float)m.y);
        btnBack.update((float)m.x,(float)m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnShow.draw(w);
        btnBack.draw(w);
    }
};

#endif