#ifndef SCREEN_READER_LOGIN_H
#define SCREEN_READER_LOGIN_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"

class ScreenReaderLogin {
public:
    TextBox boxUser;
    TextBox boxPass;
    Button btnOK;
    Button btnBack;

    ScreenReaderLogin(sf::Font &f)
    {
        boxUser = TextBox(f, {260,150}, {320,40});
        boxPass = TextBox(f, {260,220}, {320,40});
        boxPass.setPassword(true);

        btnOK   = Button("Đăng nhập", f, {260,290}, {150,50});
        btnBack = Button("Quay lại",   f, {430,290}, {150,50});
    }

    void handleEvent(sf::Event &e) {
        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
    }

    void draw(sf::RenderWindow &w) {
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif