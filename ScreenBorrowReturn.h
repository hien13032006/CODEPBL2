#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>

#include <functional>
#include "Node.h"
#include "Book.h"
#include "Button.h"
#include "Popup.h"

class ScreenBorrowReturn {
    sf::Font font;
    sf::Text title;

    NodeBook*& headBooks;

    Button btnBack;
    Popup popup;

    std::function<void()> onBack;

    float startY = 160;
    float rowH   = 45;
    int hoverIndex = -1;

    // file log (tạm)
    std::string logFile;

public:
    ScreenBorrowReturn(NodeBook*& head,
                       const std::string& logFileName,
                       std::function<void()> backFn)
        : headBooks(head), logFile(logFileName), onBack(backFn),
          btnBack()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("MUON / TRA SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(420, 40);

        btnBack = Button(font, "BACK", 500, 620);
        btnBack.setCallback([&](){ onBack(); });
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);

        drawList(t);
        btnBack.draw(t);
        popup.draw(t);
    }

    void update(const sf::RenderWindow& w) {
        btnBack.update(w);
        hoverCheck(w);
    }

    void handle(const sf::Event& evt) {
        btnBack.handle(evt);

        if (evt.type == sf::Event::MouseButtonPressed &&
            evt.mouseButton.button == sf::Mouse::Left)
        {
            borrowReturn();
        }
    }

private:

    void drawList(sf::RenderTarget& t) {
        float y = startY;
        int idx = 0;

        for (NodeBook* p = headBooks; p; p = p->next) {
            sf::RectangleShape row({900, rowH});
            row.setPosition(150,y);
            if(idx == hoverIndex)
                row.setFillColor(sf::Color(255,246,196));
            else
                row.setFillColor(sf::Color(240,240,240));

            row.setOutlineColor(sf::Color::Black);
            row.setOutlineThickness(1);
            t.draw(row);

            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(18);
            line.setFillColor(sf::Color::Black);

            std::string s =
                p->data->getMaSach() + " | " +
                p->data->getTenSach() + " | " +
                p->data->getTacGia() + " | " +
                p->data->getTinhTrang();

            line.setString(s);
            line.setPosition(160,y+10);
            t.draw(line);

            y += rowH;
            idx++;
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

    void borrowReturn() {
        if (hoverIndex < 0) {
            popup.show("Hay chon 1 sach!");
            return;
        }

        NodeBook* p = headBooks;
        int idx = 0;
        while (p && idx < hoverIndex) {
            p = p->next;
            idx++;
        }
        if (!p) {
            popup.show("Khong tim thay sach!");
            return;
        }

        Sach* s = p->data;
        std::string t = s->getTinhTrang();

        if (t == "Dang con") {
            // Cho muon
            s->muonSach();
            appendLog("Muon", s);
            popup.show("Da muon!");
        }
        else {
            // Dang muon => tra
            s->traSach();
            appendLog("Tra", s);
            popup.show("Da tra!");
        }
    }

    void appendLog(const std::string& act, Sach* s) {
        std::ofstream out(logFile, std::ios::app);
        if (!out.is_open()) return;
        out << act << "|" << s->getMaSach()
            << "|" << s->getTenSach() << "\n";
        out.close();
    }
};