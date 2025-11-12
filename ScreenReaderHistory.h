#ifndef SCREEN_READER_HISTORY_H
#define SCREEN_READER_HISTORY_H

#include "ScreenBase.h"
#include "UI_Button.h"
#include "Reader.h"
#include "AppState.h"
#include <SFML/Graphics.hpp>

class ScreenReaderHistory: public ScreenBase {

    sf::Font *font;
    UIButton btnLoad;
    UIButton btnBack;
    sf::Text content;

    Reader **currentReader;

public:
    ScreenReaderHistory(sf::Font &f, Reader **r){
        font=&f; currentReader=r;
        btnLoad.init(f,350,120,200,45,"TAI LICH SU");
        btnBack.init(f,20,20,100,40,"BACK");
        content.setFont(*font); content.setCharacterSize(18); 
        content.setFillColor(sf::Color::Black); content.setPosition(80,200);
    }

    void handleEvent(sf::Event &e, AppState &current){
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnLoad.hit(mx,my)){
                if(*currentReader){
                    // xuất trực tiếp ra console
                    (*currentReader)->HienThiLichSuMuonTra();
                    content.setString("Da tai lich su - xem console");
                }
            }
            if(btnBack.hit(mx,my)) current = SCREEN_READER_MENU;
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