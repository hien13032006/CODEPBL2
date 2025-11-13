#ifndef SCREEN_READER_REGISTER_H
#define SCREEN_READER_REGISTER_H

#include "ScreenBase.h"
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"

class ScreenReaderRegister : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxName, boxPhone, boxEmail, boxUser, boxPass;
    Button btnOK, btnShow, btnBack;

public:
    ScreenReaderRegister(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxName(f,260,40,false),
          boxPhone(f,260,40,false),
          boxEmail(f,260,40,false),
          boxUser(f,260,40,false),
          boxPass(f,260,40,true),
          btnOK("Dang ky",f,{0,0},{200,45}),
          btnShow("Show",f,{0,0},{80,40}),
          btnBack("Quay lai",f,{0,0},{200,45})
    {
        float px=190, py=100, dy=60;
        boxName.setPosition(px,py);
        boxPhone.setPosition(px,py+dy);
        boxEmail.setPosition(px,py+2*dy);
        boxUser.setPosition(px,py+3*dy);
        boxPass.setPosition(px,py+4*dy);

        boxName.setPlaceholder("ho ten...");
        boxPhone.setPlaceholder("so dien thoai...");
        boxEmail.setPlaceholder("email...");
        boxUser.setPlaceholder("username...");
        boxPass.setPlaceholder("mat khau...");

        btnOK.setPosition(220,py+5*dy);
        btnShow.setPosition(470,py+4*dy);
        btnBack.setPosition(220,py+5*dy+60);
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        boxName.handleEvent(e);
        boxPhone.handleEvent(e);
        boxEmail.handleEvent(e);
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
                // Bạn có thể dùng hàm DangKyDocGia cũ, ở đây demo đơn giản:
                Reader* r = new Reader();
                r->SignUp(
                    boxName.get(),
                    boxPhone.get(),
                    boxEmail.get(),
                    boxUser.get(),
                    boxPass.get()
                );
                L->addReader(r);
                cur = SCREEN_READER_LOGIN;
            }

            if(btnBack.hit(mx,my)){
                cur = SCREEN_READER_LOGIN;
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
        boxName.draw(w);
        boxPhone.draw(w);
        boxEmail.draw(w);
        boxUser.draw(w);
        boxPass.draw(w);

        btnOK.draw(w);
        btnShow.draw(w);
        btnBack.draw(w);
    }
};

#endif