#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Book.h"
#include "Node.h"
#include "Button.h"
#include "Popup.h"

class ScreenAddBook {
    sf::Font font;
    sf::Text title;
    sf::Text lbTen, lbTacGia, lbTheLoai, lbNam, lbNxb;

    sf::RectangleShape bTen, bTacGia, bTheLoai, bNam, bNxb;

    sf::String inTen, inTacGia, inTheLoai, inNam, inNxb;

    bool fTen=false, fTacGia=false, fTheLoai=false, fNam=false, fNxb=false;

    Button btnOK, btnBack;
    Popup popup;

    NodeBook*& headBooks;
    std::function<void()> onBack;

public:
    ScreenAddBook(NodeBook*& head,
                  std::function<void()> backFn)
        : headBooks(head), onBack(backFn),
          btnOK(), btnBack()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("THEM SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(450, 50);

        initLabel(lbTen,     "Ten Sach",    250,150);
        initLabel(lbTacGia,  "Tac Gia",     250,220);
        initLabel(lbTheLoai, "The Loai",    250,290);
        initLabel(lbNam,     "Nam XB",      250,360);
        initLabel(lbNxb,     "Nha XB",      250,430);

        initBox(bTen,     250,180);
        initBox(bTacGia,  250,250);
        initBox(bTheLoai, 250,320);
        initBox(bNam,     250,390);
        initBox(bNxb,     250,460);

        btnOK   = Button(font,"OK",   450,540);
        btnBack = Button(font,"BACK", 650,540);

        btnOK.setCallback([&](){ onOK(); });
        btnBack.setCallback([&](){ onBack(); });
    }

    void initLabel(sf::Text& t, const std::string& s, float x, float y) {
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color(30,30,30));
        t.setPosition(x,y);
    }

    void initBox(sf::RectangleShape& r, float x, float y) {
        r.setSize({600,40});
        r.setFillColor(sf::Color::White);
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(1);
        r.setPosition(x,y);
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);
        t.draw(lbTen);     t.draw(lbTacGia);
        t.draw(lbTheLoai); t.draw(lbNam);
        t.draw(lbNxb);

        t.draw(bTen);     drawInput(t, bTen, inTen);
        t.draw(bTacGia);  drawInput(t, bTacGia, inTacGia);
        t.draw(bTheLoai); drawInput(t, bTheLoai, inTheLoai);
        t.draw(bNam);     drawInput(t, bNam, inNam);
        t.draw(bNxb);     drawInput(t, bNxb, inNxb);

        btnOK.draw(t);
        btnBack.draw(t);

        popup.draw(t);
    }

    void drawInput(sf::RenderTarget& t,
                   const sf::RectangleShape& box,
                   const sf::String& s) {
        sf::Text tt(s,font,20);
        tt.setFillColor(sf::Color::Black);
        tt.setPosition(box.getPosition().x+5, box.getPosition().y+7);
        t.draw(tt);
    }

    void handle(const sf::Event& evt) {
        btnOK.handle(evt);
        btnBack.handle(evt);

        if (evt.type == sf::Event::MouseButtonPressed &&
            evt.mouseButton.button == sf::Mouse::Left) 
        {
            auto mp = sf::Mouse::getPosition();
            sf::Vector2f p(mp.x,mp.y);
            fTen     = bTen.getGlobalBounds().contains(p);
            fTacGia  = bTacGia.getGlobalBounds().contains(p);
            fTheLoai = bTheLoai.getGlobalBounds().contains(p);
            fNam     = bNam.getGlobalBounds().contains(p);
            fNxb     = bNxb.getGlobalBounds().contains(p);
        }

        if (evt.type == sf::Event::TextEntered) {
            pushText(evt,fTen,     inTen);
            pushText(evt,fTacGia,  inTacGia);
            pushText(evt,fTheLoai, inTheLoai);
            pushText(evt,fNam,     inNam);
            pushText(evt,fNxb,     inNxb);
        }
    }

    void update(const sf::RenderWindow& w) {
        btnOK.update(w);
        btnBack.update(w);
    }

private:
    void pushText(const sf::Event& evt,
                  bool f,
                  sf::String& s)
    {
        if (!f) return;
        if (evt.text.unicode == 8) {
            if (!s.isEmpty()) s.erase(s.getSize()-1);
        }
        else if (evt.text.unicode >= 32 && evt.text.unicode <= 126) {
            s += (char)evt.text.unicode;
        }
    }

    void onOK() {
        if (inTen.isEmpty() || inTacGia.isEmpty()
        ||  inTheLoai.isEmpty() || inNam.isEmpty()
        ||  inNxb.isEmpty())
        {
            popup.show("Thieu thong tin!");
            return;
        }

        int nam = stoi(inNam.toAnsiString());

        // Tao sach theo the loai (factory)
        Sach* s = Sach::createFromData(
            inTen.toAnsiString(),
            inTacGia.toAnsiString(),
            inTheLoai.toAnsiString(),
            nam,
            inNxb.toAnsiString()
        );
        if (!s) {
            popup.show("Loai sach khong hop le!");
            return;
        }

        // gan ma sach: prefix + số tăng
        static int idx = 1000;
        std::string ma = s->prefix() + std::to_string(++idx);
        s->setMaSach(ma);

        // chen vao NodeBook
        NodeBook* n = new NodeBook(s);
        n->next = headBooks;
        headBooks = n;

        popup.show("Da them thanh cong!");
    }
};

#endif