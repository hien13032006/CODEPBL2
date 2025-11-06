#pragma once
#include "MenuPanel.h"
#include "Button.h"

class ScreenMenuLibrarian : public MenuPanel {
    Button bList, bSearch, bAdd, bDel, bEdit, bRate, bBR, bBack;

public:
    void init(
        std::function<void()> onList,
        std::function<void()> onSearch,
        std::function<void()> onAdd,
        std::function<void()> onDel,
        std::function<void()> onEdit,
        std::function<void()> onRate,
        std::function<void()> onBR,
        std::function<void()> onBack
    ){
        initBase("MENU THU THU");

        float x1 = 360, x2 = 560;
        float y  = 200;
        float dy = 60;

        bList   = Button(font,"DANH SACH", x1, y);
        bSearch = Button(font,"TIM KIEM",  x1, y+dy);
        bAdd    = Button(font,"THEM SACH", x1, y+dy*2);
        bDel    = Button(font,"XOA SACH",  x1, y+dy*3);
        bEdit   = Button(font,"SUA SACH",  x1, y+dy*4);

        bRate   = Button(font,"DANH GIA",  x2, y);
        bBR     = Button(font,"MUON/TRA",  x2, y+dy);
        bBack   = Button(font,"QUAY LAI",  x2, y+dy*4);

        bList.setCallback(onList);
        bSearch.setCallback(onSearch);
        bAdd.setCallback(onAdd);
        bDel.setCallback(onDel);
        bEdit.setCallback(onEdit);
        bRate.setCallback(onRate);
        bBR.setCallback(onBR);
        bBack.setCallback(onBack);
    }

    void update(const sf::RenderWindow& w)
    {
        bList.update(w); bSearch.update(w); bAdd.update(w); bDel.update(w);
        bEdit.update(w); bRate.update(w);   bBR.update(w);  bBack.update(w);
    }

    void handle(const sf::Event& e)
    {
        bList.handle(e); bSearch.handle(e); bAdd.handle(e); bDel.handle(e);
        bEdit.handle(e); bRate.handle(e);   bBR.handle(e);  bBack.handle(e);
    }

    void draw(sf::RenderTarget& t, float winW)
    {
        drawBase(t, winW);
        drawDivider(t, 320, 185, 560);

        bList.draw(t); bSearch.draw(t); bAdd.draw(t); bDel.draw(t);
        bEdit.draw(t); bRate.draw(t);   bBR.draw(t);  bBack.draw(t);
    }
};
