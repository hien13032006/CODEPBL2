#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Node.h"
#include "Book.h"
#include "Popup.h"
#include "Button.h"

class ScreenChooseEditBook {
    sf::Font font;
    sf::Text title;

    NodeBook*& headBooks;

    Button btnBack;
    Popup popup;

    std::function<void(Sach*)> onChoose;
    std::function<void()> onBack;

    float startY = 140;     // vị trí bắt đầu vẽ
    float rowH   = 45;      // chiều cao mỗi hàng
    int   hoverIndex = -1;

public:
    ScreenChooseEditBook(NodeBook*& head,
                         std::function<void(Sach*)> chooseFn,
                         std::function<void()> backFn)
        : headBooks(head), onChoose(chooseFn), onBack(backFn),
          btnBack()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("CHON SACH DE SUA");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(390, 40);

        btnBack = Button(font,"BACK", 500,620);
        btnBack.setCallback([&](){ onBack(); });
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);

        // Vẽ danh sách
        float y = startY;
        int idx = 0;
        for (NodeBook* p = headBooks; p; p = p->next) {
            auto box = sf::RectangleShape({900, rowH});
            box.setPosition(150,y);
            if(idx == hoverIndex)
                box.setFillColor(sf::Color(255,246,196));
            else
                box.setFillColor(sf::Color(240,240,240));

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
            line.setPosition(160,y+10);
            t.draw(line);

            y += rowH;
            idx++;
        }

        btnBack.draw(t);
        popup.draw(t);
    }

    void update(const sf::RenderWindow& w) {
        btnBack.update(w);

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

    void handle(const sf::Event& evt) {
        btnBack.handle(evt);

        if(evt.type == sf::Event::MouseButtonPressed &&
           evt.mouseButton.button == sf::Mouse::Left)
        {
            choose();
        }
    }

private:
    void choose() {
        if(hoverIndex < 0) {
            popup.show("Hay chon 1 sach!");
            return;
        }
        NodeBook* p = headBooks;
        int idx=0;
        while(p && idx<hoverIndex) {
            p=p->next;
            idx++;
        }
        if(!p) {
            popup.show("Khong tim thay sach!");
            return;
        }
        onChoose(p->data);
    }
};
