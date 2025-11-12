#ifndef SCREEN_READER_RETURN_H
#define SCREEN_READER_RETURN_H

#include "ScreenBase.h"
#include "UI_Input.h"
#include "UI_Button.h"
#include "LibrarySystem.h"
#include "Reader.h"
#include "AppState.h"

class ScreenReaderReturn : public ScreenBase {
    sf::Font *font;
    UIInput input;
    UIButton btnOk;
    UIButton btnBack;

    LibrarySystem *L;
    Reader **currentReader;

public:
    ScreenReaderReturn(sf::Font &f, LibrarySystem *lib, Reader **r){
        font=&f; L=lib; currentReader=r;
        input.init(f,300,240,300,40,false);
        btnOk.init(f,350,300,200,45,"TRA");
        btnBack.init(f,20,20,100,40,"BACK");
    }

    void handleEvent(sf::Event &e, AppState &current){
        if(e.type==sf::Event::TextEntered||e.type==sf::Event::KeyPressed)
            input.handleText(e);

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnOk.hit(mx,my)){
                string ma=input.get();
                if(*currentReader)
                    L->TraSach(*currentReader, ma);
            }
            if(btnBack.hit(mx,my)) current = SCREEN_READER_MENU;
        }
    }

    void update(){}
    void draw(sf::RenderWindow &w){
        sf::Text lb;
        lb.setFont(*font); lb.setCharacterSize(20); lb.setFillColor(sf::Color::Black);
        lb.setString("Nhap ma sach muon tra:");
        lb.setPosition(300,210);
        w.draw(lb);

        input.draw(w);
        btnOk.draw(w);
        btnBack.draw(w);
    }
};
#endif