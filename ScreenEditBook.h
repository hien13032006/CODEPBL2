#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include "Node.h"
#include "Book.h"
#include "Button.h"
#include "Popup.h"

class ScreenEditBook {
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

    // cuốn sách đang sửa
    Sach* editing = nullptr;

public:
    ScreenEditBook(NodeBook*& head,
                   std::function<void()> backFn)
        : headBooks(head), onBack(backFn),
          btnOK(), btnBack()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("SUA THONG TIN SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(420, 40);

        initLabel(lbTen,     "Ten",      300,130);
        initLabel(lbTacGia,  "Tac gia",  300,190);
        initLabel(lbTheLoai, "The loai", 300,250);
        initLabel(lbNam,     "Nam XB",   300,310);
        initLabel(lbNxb,     "Nha XB",   300,370);

        initBox(bTen,     300,160);
        initBox(bTacGia,  300,220);
        initBox(bTheLoai, 300,280);
        initBox(bNam,     300,340);
        initBox(bNxb,     300,400);

        btnOK   = Button(font,"LUU",  400,480);
        btnBack = Button(font,"BACK",600,480);

        btnOK.setCallback([&](){ onOK(); });
        btnBack.setCallback([&](){ onBack(); });
    }

    // Nhận sách từ màn chọn
    void setBook(Sach* s) {
        editing = s;
        if(!s) return;

        inTen     = s->getTenSach();
        inTacGia  = s->getTacGia();
        inTheLoai = s->getTheLoai();
        inNam     = std::to_string(s->getNamXuatBan());
        inNxb     = s->getNhaXuatBan();
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);

        t.draw(lbTen);     t.draw(lbTacGia);
        t.draw(lbTheLoai); t.draw(lbNam);
        t.draw(lbNxb);

        drawBox(t, bTen,     inTen);
        drawBox(t, bTacGia,  inTacGia);
        drawBox(t, bTheLoai, inTheLoai);
        drawBox(t, bNam,     inNam);
        drawBox(t, bNxb,     inNxb);

        btnOK.draw(t);
        btnBack.draw(t);

        popup.draw(t);
    }

    void update(const sf::RenderWindow& w) {
        btnOK.update(w);
        btnBack.update(w);
    }

    void handle(const sf::Event& evt) {
        btnOK.handle(evt);
        btnBack.handle(evt);

        // CLICK BOX
        if (evt.type == sf::Event::MouseButtonPressed &&
            evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mp = sf::Mouse::getPosition();
            sf::Vector2f p(mp.x, mp.y);

            fTen     = bTen.getGlobalBounds().contains(p);
            fTacGia  = bTacGia.getGlobalBounds().contains(p);
            fTheLoai = bTheLoai.getGlobalBounds().contains(p);
            fNam     = bNam.getGlobalBounds().contains(p);
            fNxb     = bNxb.getGlobalBounds().contains(p);
        }

        // TEXT INPUT
        if (evt.type == sf::Event::TextEntered) {
            pushText(evt, fTen,     inTen);
            pushText(evt, fTacGia,  inTacGia);
            pushText(evt, fTheLoai, inTheLoai);
            pushText(evt, fNam,     inNam);
            pushText(evt, fNxb,     inNxb);
        }
    }

private:

    void initLabel(sf::Text& t, const std::string& s, float x, float y) {
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Black);
        t.setPosition(x,y);
    }

    void initBox(sf::RectangleShape& r, float x, float y) {
        r.setSize({600,40});
        r.setFillColor(sf::Color::White);
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(1);
        r.setPosition(x,y);
    }

    void drawBox(sf::RenderTarget& t, const sf::RectangleShape& box, const sf::String& s) {
        t.draw(box);
        sf::Text tt(s,font,20);
        tt.setFillColor(sf::Color::Black);
        tt.setPosition(box.getPosition().x+5, box.getPosition().y+7);
        t.draw(tt);
    }

    void pushText(const sf::Event& evt, bool f, sf::String& s) {
        if(!f) return;

        if(evt.text.unicode == 8) {        // backspace
            if(!s.isEmpty()) s.erase(s.getSize()-1,1);
        }
        else if(evt.text.unicode >= 32 && evt.text.unicode <= 126) {
            s += (char)evt.text.unicode;
        }
    }

    void onOK() {
        if(!editing) {
            popup.show("Chua chon sach!");
            return;
        }

        if(inTen.isEmpty() || inTacGia.isEmpty() ||
           inTheLoai.isEmpty() || inNam.isEmpty() || inNxb.isEmpty())
        {
            popup.show("Thieu thong tin!");
            return;
        }

        int nam = 0;
        try {
            nam = stoi(inNam.toAnsiString());
        } catch(...) {
            popup.show("Nam khong hop le!");
            return;
        }

        // Lưu thay đổi vào sách
        editing->setTenSach(inTen.toAnsiString());
        editing->setTacGia(inTacGia.toAnsiString());
        editing->setTheLoai(inTheLoai.toAnsiString());
        editing->setNamXuatBan(nam);
        editing->setNhaXuatBan(inNxb.toAnsiString());

        popup.show("Da luu!");
    }
};

#endif