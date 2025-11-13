#ifndef SCREEN_READER_MENU_H
#define SCREEN_READER_MENU_H

#include "ScreenBase.h"
#include "Button.h"

class ScreenReaderMenu : public ScreenBase {
private:
    sf::Font &font;
    Button btn1,btn2,btn3,btn4,btn5,btn6,btnExit;

public:
    ScreenReaderMenu(sf::Font &f)
        : font(f),
          btn1("Xem sach",f,{0,0},{220,45}),
          btn2("Tim sach",f,{0,0},{220,45}),
          btn3("Muon sach",f,{0,0},{220,45}),
          btn4("Tra sach",f,{0,0},{220,45}),
          btn5("Danh gia",f,{0,0},{220,45}),
          btn6("Top sach",f,{0,0},{220,45}),
          btnExit("Lich su",f,{0,0},{220,45})
    {
        float x=220,y=120,dy=55;
        Button* arr[]={&btn1,&btn2,&btn3,&btn4,&btn5,&btn6,&btnExit};
        for(int i=0;i<7;i++){
            arr[i]->setPosition(x,y+i*dy);
        }
    }

    void handleEvent(sf::Event &e, AppState &cur) override {
        if(e.type==sf::Event::MouseButtonReleased){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btn1.hit(mx,my)) cur = SCREEN_VIEW_BOOK;
            if(btn2.hit(mx,my)) cur = SCREEN_SEARCH_BOOK;
            if(btn3.hit(mx,my)) cur = SCREEN_BORROW_BOOK;
            if(btn4.hit(mx,my)) cur = SCREEN_RETURN_BOOK;
            if(btn5.hit(mx,my)) cur = SCREEN_RATE_BOOK;
            if(btn6.hit(mx,my)) cur = SCREEN_TOP_BOOK;
            if(btnExit.hit(mx,my)) cur = SCREEN_HISTORY;
        }
    }

    void update() override {
        sf::Vector2i m = sf::Mouse::getPosition();
        Button* arr[]={&btn1,&btn2,&btn3,&btn4,&btn5,&btn6,&btnExit};
        for(int i=0;i<7;i++)
            arr[i]->update((float)m.x,(float)m.y);
    }

    void draw(sf::RenderWindow &w) override {
        w.clear(Theme::BG);
        btn1.draw(w); btn2.draw(w); btn3.draw(w);
        btn4.draw(w); btn5.draw(w); btn6.draw(w);
        btnExit.draw(w);
    }
};

#endif