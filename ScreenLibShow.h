#ifndef SCREEN_LIB_SHOW_H
#define SCREEN_LIB_SHOW_H

#include "ScreenBase.h"
#include "UI_Button.h"
#include "LibrarySystem.h"
#include "AppState.h"
#include <SFML/Graphics.hpp>

class ScreenLibShow: public ScreenBase {

    sf::Font *font;
    UIButton btnLoad;
    UIButton btnBack;
    sf::Text content;
    LibrarySystem *L;

public:
    ScreenLibShow(sf::Font &f, LibrarySystem *lib){
        font=&f; L=lib;
        btnLoad.init(f,350,120,200,45,"HIEN THI");
        btnBack.init(f,20,20,100,40,"BACK");
        content.setFont(*font); content.setCharacterSize(18);
        content.setFillColor(sf::Color::Black);
        content.setPosition(50,200);
    }

    void handleEvent(sf::Event &e, AppState &current){
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnLoad.hit(mx,my)){
                // hiện console
                L->HienThiDanhSachSach();
                content.setString("Da in danh sach ra console");
            }
            if(btnBack.hit(mx,my)) current = SCREEN_LIB_MENU;
        }
    }

    void update(){}
    void draw(sf::RenderWindow &w){
        btnLoad.draw(w);
        btnBack.draw(w);
        w.draw(content);
    }
};
#endif