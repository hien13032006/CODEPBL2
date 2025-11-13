#ifndef SCREEN_READER_MENU_H
#define SCREEN_READER_MENU_H

#include "Button.h"
#include "ScreenBase.h"
#include "AppState.h"
#include <SFML/Graphics.hpp>

class ScreenReaderMenu : public ScreenBase {
private:
    Button btn1, btn2, btn3, btn4, btn5, btn6, btnExit;
    sf::RenderWindow* window; // lưu pointer window để dùng trong checkClick

public:
    ScreenReaderMenu(sf::Font &f, sf::RenderWindow *w) : window(w) {
        float x = 260, y = 100, dy = 60;
        btn1 = Button("Xem sách", f, {x,y},{250,50});
        btn2 = Button("Tìm sách", f, {x,y+dy},{250,50});
        btn3 = Button("Mượn sách", f,{x,y+dy*2},{250,50});
        btn4 = Button("Trả sách", f,{x,y+dy*3},{250,50});
        btn5 = Button("Đánh giá", f,{x,y+dy*4},{250,50});
        btn6 = Button("Top sách", f,{x,y+dy*5},{250,50});
        btnExit = Button("Thoát", f,{x,y+dy*6},{250,50});
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        // gọi checkClick với event và window
        btn1.checkClick(e, *window);
        btn2.checkClick(e, *window);
        btn3.checkClick(e, *window);
        btn4.checkClick(e, *window);
        btn5.checkClick(e, *window);
        btn6.checkClick(e, *window);
        btnExit.checkClick(e, *window);
    }

    void draw(sf::RenderWindow &w) {
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
