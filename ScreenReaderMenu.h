#ifndef SCREEN_READER_MENU_H
#define SCREEN_READER_MENU_H

#include "Button.h"
#include "ScreenBase.h"
#include "AppState.h"
#include <SFML/Graphics.hpp>

class ScreenReaderMenu : public ScreenBase {
private:
    Button btn1, btn2, btn3, btn4, btn5, btn6, btnExit;
    sf::RenderWindow* window; // lưu pointer window
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    AppState* currentState; // pointer để thay đổi màn hình

public:
    ScreenReaderMenu(sf::Font &f, sf::RenderWindow *w, AppState* state) 
        : window(w), currentState(state) 
    {
        // --- Hình nền ---
        if(!bgTexture.loadFromFile("picReader.png")) {
            bgTexture.create(1024,768);
        }
        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = w->getSize();
        bgSprite.setScale(
            float(winSize.x)/bgTexture.getSize().x,
            float(winSize.y)/bgTexture.getSize().y
        );

        float x = 800, y = 250, dy = 100;
        btn1 = Button("DANH SACH SACH", f, {x,y},{350,70});
        btn2 = Button("TIM SACH", f, {x,y+dy},{350,70});
        btn3 = Button("MUON SACH", f,{x,y+dy*2},{350,70});  
        btn4 = Button("TRA SACH", f,{x,y+dy*3},{350,70});
        btn5 = Button("DANH GIA", f,{x,y+dy*4},{350,70});
        btn6 = Button("TOP 10 SACH DUOC YEU THICH", f,{x,y+dy*5},{350,70});
        btnExit = Button("THOAT", f,{x,y+dy*6},{350,70});

        // --- Gán callback ---
        btn1.setCallback([this]() { *currentState = SCREEN_VIEW_BOOK; });
        btn2.setCallback([this]() { *currentState = SCREEN_SEARCH_BOOK; });
        btn3.setCallback([this]() { *currentState = SCREEN_BORROW_BOOK; });
        btn4.setCallback([this]() { *currentState = SCREEN_RETURN_BOOK; });
        btn5.setCallback([this]() { *currentState = SCREEN_VIEW_BOOK; });
        btn6.setCallback([this]() { *currentState = SCREEN_TOP_BOOK; });
        btnExit.setCallback([this]() { *currentState = SCREEN_WELCOME; });
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        btn1.checkClick(e, *window);
        btn2.checkClick(e, *window);
        btn3.checkClick(e, *window);
        btn4.checkClick(e, *window);
        btn5.checkClick(e, *window);
        btn6.checkClick(e, *window);
        btnExit.checkClick(e, *window);
    }

    void update() override {
        btn1.update();
        btn2.update();
        btn3.update();
        btn4.update();
        btn5.update();
        btn6.update();
        btnExit.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        btn1.draw(w);
        btn2.draw(w);
        btn3.draw(w);
        btn4.draw(w);
        btn5.draw(w);
        btn6.draw(w);
        btnExit.draw(w);
    }
};

#endif
