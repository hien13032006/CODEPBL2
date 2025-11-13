#ifndef SCREEN_HISTORY_H
#define SCREEN_HISTORY_H

#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "ScreenBase.h"
#include "AppState.h"
#include "LibrarySystem.h"
#include "Reader.h"
#include "ListView.h"
#include "Button.h"

class ScreenHistory : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *L;
    Reader **currentReader;

    ListView listBox;
    Button btnReload, btnBack;
    std::string msg;

public:
    ScreenHistory(sf::Font &f, LibrarySystem *lib, Reader **cr)
        : font(f), L(lib), currentReader(cr),
          listBox(f,520,360),
          btnReload("tai lai",f,20),
          btnBack("quay lai",f,20),
          msg("")
    {
        listBox.setPosition(50,120);
    }

    void reload(){
        listBox.clear();
        if(*currentReader==nullptr){ msg="ban chua dang nhap"; return; }
        std::string user = (*currentReader)->getUsername();

        std::ifstream in("DanhGia.txt");
        if(!in.is_open()){ msg="khong mo duoc DanhGia.txt"; return; }

        std::string line;
        while(std::getline(in,line)){
            // ma|ten|tg|nam|nxb|maDG|user|text|diem
            std::stringstream ss(line);
            std::string ma, ten, tg, nam, nxb, maDG, u, text, diem;
            std::getline(ss, ma, '|');
            std::getline(ss, ten, '|');
            std::getline(ss, tg,  '|');
            std::getline(ss, nam, '|');
            std::getline(ss, nxb, '|');
            std::getline(ss, maDG,'|');
            std::getline(ss, u,   '|');
            std::getline(ss, text,'|');
            std::getline(ss, diem,'|');

            if(u == user){
                std::string row = ma + " | " + ten + " | " + tg + " | " + text + " (" + diem + ")";
                listBox.addLine(row);
            }
        }
        in.close();
        msg = "";
    }

    void handleEvent(sf::Event &e, AppState &current, sf::RenderWindow *w){
        listBox.handleScroll(e);
        if(e.type==sf::Event::MouseButtonPressed){
            float mx=e.mouseButton.x, my=e.mouseButton.y;
            if(btnReload.hit(mx,my)) reload();
            else if(btnBack.hit(mx,my)) current = SCREEN_READER_MENU;
        }
    }

    void update(){}

    void draw(sf::RenderWindow &w){
        sf::Text title("Lich su danh gia cua ban", font, 26);
        title.setFillColor(sf::Color::Black);
        title.setPosition(90,60);
        w.draw(title);

        btnReload.setPosition(50,85); btnReload.draw(w);
        btnBack.setPosition(20,20);   btnBack.draw(w);

        listBox.draw(w);

        sf::Text m(msg,font,18); m.setFillColor(sf::Color::Red); m.setPosition(50,490);
        w.draw(m);
    }
};

#endif