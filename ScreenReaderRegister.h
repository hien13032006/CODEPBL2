#ifndef SCREEN_READER_REGISTER_H
#define SCREEN_READER_REGISTER_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include "ScreenBase.h"

class ScreenReaderRegister: public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *library;

public:
    TextBox boxName, boxPhone, boxEmail, boxUser, boxPass;
    Button btnOK, btnBack;

    
    ScreenReaderRegister(sf::Font &f)
    : font(f), library(nullptr)
    {
        float x = 260, y = 100, dy = 60;
        boxName  = TextBox(f,{x,y},{320,40});
        boxPhone = TextBox(f,{x,y+dy},{320,40});
        boxEmail = TextBox(f,{x,y+dy*2},{320,40});
        boxUser  = TextBox(f,{x,y+dy*3},{320,40});
        boxPass  = TextBox(f,{x,y+dy*4},{320,40});
        boxPass.setPassword(true);

        btnOK   = Button("Đăng ký", f, {260, y+dy*5+10}, {150,50});
        btnBack = Button("Quay lại",f, {430, y+dy*5+10}, {150,50});
    }

    
    ScreenReaderRegister(sf::Font &f, LibrarySystem *lib)
    : font(f), library(lib)
    {
        float x = 260, y = 100, dy = 60;
        boxName  = TextBox(f,{x,y},{320,40});
        boxPhone = TextBox(f,{x,y+dy},{320,40});
        boxEmail = TextBox(f,{x,y+dy*2},{320,40});
        boxUser  = TextBox(f,{x,y+dy*3},{320,40});
        boxPass  = TextBox(f,{x,y+dy*4},{320,40});
        boxPass.setPassword(true);

        btnOK   = Button("Đăng ký", f, {260, y+dy*5+10}, {150,50});
        btnBack = Button("Quay lại",f, {430, y+dy*5+10}, {150,50});
    }

    
    void handleEvent(sf::Event &e, AppState &state) override {
        boxName.handleEvent(e);
        boxPhone.handleEvent(e);
        boxEmail.handleEvent(e);
        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
    }

    void draw(sf::RenderWindow &w) {
        boxName.draw(w);
        boxPhone.draw(w);
        boxEmail.draw(w);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif
