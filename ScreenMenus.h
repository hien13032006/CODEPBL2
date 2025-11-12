#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "Button.h"
#include "Theme.h"

// ===================== BASE PANEL =====================
class MenuPanel {
protected:
    sf::Font font;
    sf::Text title;
    sf::RectangleShape panel;

public:
    virtual ~MenuPanel() {}

    // ====== virtual để lớp con override ======
    virtual void update(const sf::RenderWindow& w)  {}
    virtual void handle(const sf::Event& e)       {}

    void initBase(const std::string& titleStr) {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString(titleStr);
        title.setCharacterSize(38);
        title.setFillColor(Theme::Title);
        title.setStyle(sf::Text::Bold);
        title.setPosition(0, 70);

        panel.setSize({620.f, 500.f});
        panel.setFillColor(Theme::Panel);
        panel.setOutlineColor(Theme::PanelOutline);
        panel.setOutlineThickness(3.f);
        panel.setPosition(290.f, 130.f);   // center-ish
    }

    void drawBase(sf::RenderTarget& t, float winW) {
        Theme::centerTextX(title, winW);
        t.draw(panel);
        t.draw(title);
    }

    void drawDivider(sf::RenderTarget& t, float x, float y, float w) {
        sf::RectangleShape line({w, 2});
        line.setFillColor(sf::Color(200,205,255));
        line.setPosition(x, y);
        t.draw(line);
    }
};



// ===================== MENU LIBRARIAN =====================
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

        float x1 = 350, x2 = 560;
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

    void update(const sf::RenderWindow& w) override {
        bList.update(w); bSearch.update(w); bAdd.update(w); bDel.update(w);
        bEdit.update(w); bRate.update(w);   bBR.update(w);  bBack.update(w);
    }

    void handle(const sf::Event& e) override {
        bList.handle(e); bSearch.handle(e); bAdd.handle(e); bDel.handle(e);
        bEdit.handle(e); bRate.handle(e);   bBR.handle(e);  bBack.handle(e);
    }

    void draw(sf::RenderTarget& t, float winW) {
        drawBase(t, winW);
        drawDivider(t, 320, 185, 560);

        bList.draw(t); bSearch.draw(t); bAdd.draw(t); bDel.draw(t);
        bEdit.draw(t); bRate.draw(t);   bBR.draw(t);  bBack.draw(t);
    }
};


// ===================== MENU READER =====================
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
        float y  = 200;
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

    void update(const sf::RenderWindow& w) override {
        bList.update(w); bSearch.update(w); bRate.update(w);
        bBR.update(w);   bLogout.update(w);
    }

    void handle(const sf::Event& e) override {
        bList.handle(e); bSearch.handle(e); bRate.handle(e);
        bBR.handle(e);   bLogout.handle(e);
    }

    void draw(sf::RenderTarget& t, float winW) {
        drawBase(t, winW);
        drawDivider(t, 320, 185, 560);

        bList.draw(t); bSearch.draw(t); bRate.draw(t);
        bBR.draw(t);   bLogout.draw(t);
    }
};
