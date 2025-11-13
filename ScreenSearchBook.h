#ifndef SCREEN_SEARCH_BOOK_H
#define SCREEN_SEARCH_BOOK_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "LibrarySystem.h"
#include "ListView.h"
#include "Button.h"

class ScreenSearchBook : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;

    TextBox boxKey;
    Button btnFind;
    Button btnBack;
    ListView list;

public:
    ScreenSearchBook(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          boxKey(f,260,40,false),
          btnFind("Tim",f,22),
          btnBack("Quay lai",f,22),
          list(f,500,350)
    {
        boxKey.setPosition(70,70);
        boxKey.setPlaceholder("ten sach...");

        btnFind.setSize(80,40);
        btnFind.setPosition(350,70);

        btnBack.setSize(200,45);
        btnBack.setPosition(220,450);

        list.setPosition(70,130);
    }

    void search(){
        list.clear();

        string key = boxKey.get();
        NodeBook *p = L->getHeadBook();
        while(p){
            if(p->data->getTenSach() == key){
                list.addLine(
                    p->data->getMaSach() + " | " +
                    p->data->getTenSach() + " | " +
                    p->data->getTacGia()
                );
            }
            p = p->next;
        }
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        boxKey.handleEvent(e);
        list.handleScroll(e);

        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;

            if(btnFind.hit(mx,my)){
                search();
            }
            if(btnBack.hit(mx,my)){
                cur = SCREEN_READER_MENU;
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        boxKey.draw(w);
        btnFind.draw(w);
        list.draw(w);
        btnBack.draw(w);
    }
};

#endif