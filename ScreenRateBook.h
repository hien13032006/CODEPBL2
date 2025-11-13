#ifndef SCREEN_RATE_BOOK_H
#define SCREEN_RATE_BOOK_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "LibrarySystem.h"
#include "Button.h"

class ScreenRateBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **current;

    TextBox boxID;
    TextBox boxScore;
    Button btnOK;
    Button btnBack;

public:
    ScreenRateBook(sf::Font &f, LibrarySystem *lib, Reader **cur)
        : font(f), L(lib), current(cur),
          boxID(f,260,40,false), boxScore(f,260,40,false),
          btnOK("Danh gia",f,22),
          btnBack("Quay lai",f,22)
    {
        boxID.setPosition(190,150);
        boxScore.setPosition(190,220);

        boxID.setPlaceholder("ma sach...");
        boxScore.setPlaceholder("diem (2-10)");

        btnOK.setSize(200,45);
        btnOK.setPosition(220,290);

        btnBack.setSize(200,45);
        btnBack.setPosition(220,350);
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        boxID.handleEvent(e);
        boxScore.handleEvent(e);

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x,my=e.mouseButton.y;
            if(btnOK.hit(mx,my)){
                if(*current){
                    Sach *s = L->findBookByID(boxID.get());
                    if(s){
                        int d = atoi(boxScore.get().c_str());
                        s->themDanhGia(d);
                        L->GhiFileHeThong("DanhSachSach.txt");
                    }
                }
            }
            if(btnBack.hit(mx,my)){
                cur = SCREEN_READER_MENU;
            }
        }
    }

    void update() override {}
    void draw(sf::RenderWindow &w) override {
        boxID.draw(w);
        boxScore.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif