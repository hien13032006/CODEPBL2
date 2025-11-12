#ifndef SCREEN_READER_UPDATE_H
#define SCREEN_READER_UPDATE_H

#include "ScreenBase.h"
#include "UI_Input.h"
#include "UI_Button.h"
#include "Reader.h"
#include "AppState.h"
#include <SFML/Graphics.hpp>

class ScreenReaderUpdate : public ScreenBase {
    sf::Font *font;
    UIInput inName, inPhone, inEmail;
    UIButton btnSave;
    UIButton btnBack;

    Reader **currentReader;

public:
    ScreenReaderUpdate(sf::Font &f, Reader **r){
        font=&f; currentReader=r;

        inName .init(f,300,200,300,40,false);
        inPhone.init(f,300,260,300,40,false);
        inEmail.init(f,300,320,300,40,false);

        btnSave.init(f,350,390,200,45,"Luu");
        btnBack.init(f,20,20,100,40,"BACK");
    }

    void handleEvent(sf::Event &e, AppState &current){
        if(e.type==sf::Event::TextEntered||e.type==sf::Event::KeyPressed){
            inName.handleText(e);
            inPhone.handleText(e);
            inEmail.handleText(e);
        }

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;

            if(btnSave.hit(mx,my)){
                if(*currentReader){
                    (*currentReader)->setHoTen(inName.get());
                    (*currentReader)->setSDT(inPhone.get());
                    (*currentReader)->setEmail(inEmail.get());
                    cout<<"Da cap nhat thong tin"<<endl;
                }
            }
            if(btnBack.hit(mx,my)) current = SCREEN_READER_MENU;
        }
    }

    void update(){}
    void draw(sf::RenderWindow &w){
        sf::Text t;
        t.setFont(*font); t.setCharacterSize(22); t.setFillColor(sf::Color::Black);
        t.setString("Cap nhat thong tin");
        t.setPosition(330,150);
        w.draw(t);

        inName.draw(w);
        inPhone.draw(w);
        inEmail.draw(w);

        btnSave.draw(w);
        btnBack.draw(w);
    }
};
#endif