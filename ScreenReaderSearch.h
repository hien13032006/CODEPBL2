#ifndef SCREEN_READER_SEARCH_H
#define SCREEN_READER_SEARCH_H

#include "ScreenBase.h"
#include "UI_Input.h"
#include "UI_Button.h"
#include "AppState.h"
#include "LibrarySystem.h"
#include <SFML/Graphics.hpp>

class ScreenReaderSearch : public ScreenBase {

    sf::Font *font;
    UIInput input;
    UIButton btnTim;
    UIButton btnBack;

    LibrarySystem *L;

public:
    ScreenReaderSearch(sf::Font &f, LibrarySystem *lib) {
        font = &f;
        L = lib;

        input.init(f, 280,220,350,40,false);
        btnTim.init (f,350,300,200,45,"TIM");
        btnBack.init(f,20,20,100,40,"BACK");
    }

    void handleEvent(sf::Event &e, AppState &current) {
        if(e.type==sf::Event::TextEntered||e.type==sf::Event::KeyPressed)
            input.handleText(e);

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;
            if(btnTim.hit(mx,my)){
                // gọi tìm
                // L->TimSach(); // có menu để chọn tiêu chí
                // Hoặc tự implement: L->findBookByID, findBookByName
                cout<<"Tim sach voi tu khoa: "<<input.get()<<endl;
            }
            if(btnBack.hit(mx,my)) current = SCREEN_READER_MENU;
        }
    }

    void update(){}

    void draw(sf::RenderWindow &w){
        sf::Text t;
        t.setFont(*font); t.setCharacterSize(20); t.setFillColor(sf::Color::Black);
        t.setString("Nhap tu khoa:");
        t.setPosition(280,190);
        w.draw(t);

        input.draw(w);
        btnTim.draw(w);
        btnBack.draw(w);
    }
};
#endif