#pragma once
#include "MenuPanel.h"
#include "Button.h"

class ScreenMenuReader : public MenuPanel {
    Button bList, bSearch, bRate, bBR, bLogout;

public:
    void init(
        std::function<void()> onList,
        std::function<void()> onSearch,
        std::function<void()> onRate,
        std::function<void()> onBR,
        std::function<void()> onLogout
    ){
        initBase("MENU READER");

        float x = 420;
        float y = 200;
        float dy = 60;

        bList   = Button(font,"DANH SACH", x, y);
        bSearch = Button(font,"TIM KIEM",  x, y+dy);
        bRate   = Button(font,"DANH GIA",  x, y+dy*2);
        bBR     = Button(font,"MUON/TRA",  x, y+dy*3);
        bLogout = Button(font,"DANG XUAT", x, y+dy*4);

        bList.setCallback(onList);
        bSearch.setCallback(onSearch);
        bRate.setCallback(onRate);
        bBR.setCallback(onBR);
        bLogout.setCallback(onLogout);
    }

    void update(const sf::RenderWindow& w)
    {
        bList.update(w); bSearch.update(w); bRate.update(w);
        bBR.update(w);   bLogout.update(w);
    }

    void handle(const sf::Event& e)
    {
        bList.handle(e); bSearch.handle(e); bRate.handle(e);
        bBR.handle(e);   bLogout.handle(e);
    }

    void draw(sf::RenderTarget& t, float winW)
    {
        drawBase(t, winW);
        drawDivider(t, 320, 185, 560);

        bList.draw(t); bSearch.draw(t); bRate.draw(t);
        bBR.draw(t);   bLogout.draw(t);
    }
};
