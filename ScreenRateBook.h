#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Node.h"
#include "Book.h"
#include "Popup.h"
#include "Button.h"

class ScreenRateBook {
    sf::Font font;
    sf::Text title;
    sf::Text lbScore;
    sf::RectangleShape scoreBox;
    sf::String inScore;
    bool focusScore = false;

    NodeBook*& headBooks;
    Sach* selected = nullptr;

    Button btnBack, btnOK;
    Popup popup;

    std::function<void()> onBack;

    // list
    float startY = 160;
    float rowH = 45;
    int hoverIndex = -1;

public:
    ScreenRateBook(NodeBook*& head,
                   std::function<void()> backFn)
        : headBooks(head), onBack(backFn),
          btnBack(), btnOK()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("DANH GIA SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(420, 40);

        lbScore.setFont(font);
        lbScore.setString("Nhap diem (1-5):");
        lbScore.setCharacterSize(20);
        lbScore.setFillColor(sf::Color::Black);
        lbScore.setPosition(350, 110);

        scoreBox.setSize({200, 40});
        scoreBox.setPosition(550, 105);
        scoreBox.setFillColor(sf::Color::White);
        scoreBox.setOutlineColor(sf::Color::Black);
        scoreBox.setOutlineThickness(1);

        btnOK   = Button(font, "LUU",  450, 600);
        btnBack = Button(font, "BACK", 650, 600);

        btnOK.setCallback([&](){ onOK(); });
        btnBack.setCallback([&](){ onBack(); });
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);
        t.draw(lbScore);

        t.draw(scoreBox);

        sf::Text sc(inScore, font, 20);
        sc.setFillColor(sf::Color::Black);
        sc.setPosition(scoreBox.getPosition().x + 10,
                       scoreBox.getPosition().y + 7);
        t.draw(sc);

        drawList(t);

        btnOK.draw(t);
        btnBack.draw(t);

        popup.draw(t);
    }

    void update(const sf::RenderWindow& w) {
        btnOK.update(w);
        btnBack.update(w);

        hoverCheck(w);
    }

    void handle(const sf::Event& evt) {
        btnOK.handle(evt);
        btnBack.handle(evt);

        if (evt.type == sf::Event::MouseButtonPressed &&
            evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mp = sf::Mouse::getPosition();
            sf::Vector2f p(mp.x, mp.y);

            focusScore = scoreBox.getGlobalBounds().contains(p);

            if (!focusScore) {
                chooseBook();
            }
        }

        if (evt.type == sf::Event::TextEntered) {
            pushText(evt);
        }
    }

private:

    void drawList(sf::RenderTarget& t) {
        float y = startY;
        int i = 0;
        for (NodeBook* p = headBooks; p; p = p->next) {
            sf::RectangleShape box({900, rowH});
            box.setPosition(150, y);
            if (i == hoverIndex)
                box.setFillColor(sf::Color(255, 246, 196));
            else
                box.setFillColor(sf::Color(240, 240, 240));

            box.setOutlineColor(sf::Color::Black);
            box.setOutlineThickness(1);
            t.draw(box);

            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(18);
            line.setFillColor(sf::Color::Black);
            line.setString(
                p->data->getMaSach() + " | " +
                p->data->getTenSach() + " | " +
                p->data->getTacGia()
            );
            line.setPosition(160, y + 10);
            t.draw(line);

            y += rowH;
            i++;
        }

        if (hoverIndex >= 0) {
            // highlight selected
        }
    }

    void hoverCheck(const sf::RenderWindow& w) {
        hoverIndex = -1;
        auto m = sf::Mouse::getPosition(w);
        sf::Vector2f mp(m.x,m.y);

        float y = startY;
        int idx = 0;
        for(NodeBook* p = headBooks; p; p = p->next) {
            sf::FloatRect r(150,y,900,rowH);
            if(r.contains(mp)) {
                hoverIndex = idx;
                break;
            }
            idx++;
            y += rowH;
        }
    }

    void chooseBook() {
        if(hoverIndex < 0) return;

        NodeBook* p = headBooks;
        int idx = 0;
        while (p && idx < hoverIndex) {
            p = p->next;
            idx++;
        }
        if (!p) return;

        selected = p->data;
        popup.show("Da chon: " + selected->getTenSach());
    }

    void pushText(const sf::Event& evt) {
        if (!focusScore) return;
        if (evt.text.unicode == 8) {
            if (!inScore.isEmpty()) inScore.erase(inScore.getSize()-1, 1);
        }
        else if (evt.text.unicode >= '0' && evt.text.unicode <= '9') {
            if (inScore.getSize() < 1)    // chỉ cho nhập 1 ký tự
                inScore += (char)evt.text.unicode;
        }
    }

    void onOK() {
        if (!selected) {
            popup.show("Chua chon sach!");
            return;
        }
        if (inScore.isEmpty()) {
            popup.show("Nhap diem!");
            return;
        }

        int d = std::stoi(inScore.toAnsiString());
        if (d < 1 || d > 5) {
            popup.show("Diem phai tu 1 den 5!");
            return;
        }

        selected->themDanhGia(d);
        popup.show("Da luu danh gia!");

        inScore.clear();
    }
};
