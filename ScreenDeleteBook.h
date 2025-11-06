#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Node.h"
#include "Book.h"
#include "Button.h"
#include "Popup.h"

class ScreenDeleteBook {
    sf::Font font;
    sf::Text title, hint, col;
    Button btnBack, btnDel;
    Popup popup;

    NodeBook*& headBooks;

    // VIEW LIST
    int scroll = 0;
    int pageSize = 8;
    int rowH = 55;
    int listTop = 140;
    int listLeft = 100;
    int listW = 1000;

    int selectedIndex = -1;

    std::function<void()> onBack;

public:
    ScreenDeleteBook(NodeBook*& head,
                     std::function<void()> backFn)
        : headBooks(head), onBack(backFn),
          btnBack(), btnDel()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("XOA SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(470, 50);

        hint.setFont(font);
        hint.setString("Click vao dong de chon sach. Bam XOA de xoa sach.");
        hint.setCharacterSize(18);
        hint.setFillColor(sf::Color(70,70,70));
        hint.setPosition(100, 100);

        col.setFont(font);
        col.setCharacterSize(20);
        col.setFillColor(sf::Color(10,10,10));
        col.setString("Ma | Ten | Tac gia | The loai | Nam | Nha XB | Tinh trang");
        col.setPosition(listLeft, listTop - 28);

        btnBack = Button(font,"BACK", 100, 620);
        btnDel  = Button(font,"XOA",  300, 620);

        btnBack.setCallback([&](){ if(onBack) onBack(); });
        btnDel.setCallback([&](){ onDelete(); });
    }

    void handle(const sf::Event& evt) {
        btnBack.handle(evt);
        btnDel.handle(evt);

        // CLICK CHON ROW
        if (evt.type == sf::Event::MouseButtonPressed &&
            evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mp = sf::Mouse::getPosition();
            sf::Vector2f p((float)mp.x,(float)mp.y);

            for (int i = 0; i < pageSize; ++i) {
                sf::FloatRect r(listLeft, listTop + i*rowH, (float)listW, (float)rowH);
                if (r.contains(p)) {
                    selectedIndex = scroll + i;
                    break;
                }
            }
        }

        // SCROLL
        if (evt.type == sf::Event::MouseWheelScrolled) {
            if (evt.mouseWheelScroll.delta < 0) scroll += 1;
            else scroll -= 1;
            if (scroll < 0) scroll = 0;
        }
    }

    void update(const sf::RenderWindow& w) {
        btnBack.update(w);
        btnDel.update(w);
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);
        t.draw(hint);
        t.draw(col);

        // BG TABLE
        sf::RectangleShape bg;
        bg.setPosition(listLeft-10, listTop-10);
        bg.setSize({(float)listW+20, (float)rowH*pageSize+20});
        bg.setFillColor(sf::Color(245,245,255));
        bg.setOutlineColor(sf::Color(180,180,220));
        bg.setOutlineThickness(2);
        t.draw(bg);

        drawRows(t);

        btnBack.draw(t);
        btnDel.draw(t);
        popup.draw(t);
    }

private:

    int totalCount() const {
        int c = 0;
        for (NodeBook* p = headBooks; p; p = p->next) ++c;
        return c;
    }

    Sach* bookAtIndex(int idx) {
        int i = 0;
        for (NodeBook* p = headBooks; p; p = p->next) {
            if (i == idx) return p->data;
            i++;
        }
        return nullptr;
    }

    NodeBook* nodeAtIndex(int idx) {
        int i = 0;
        for (NodeBook* p = headBooks; p; p = p->next) {
            if (i == idx) return p;
            i++;
        }
        return nullptr;
    }

    void drawRows(sf::RenderTarget& t) {
        int total = totalCount();
        if (scroll > total) scroll = total;

        for (int i = 0; i < pageSize; ++i) {
            int idx = scroll + i;
            Sach* s = bookAtIndex(idx);

            sf::RectangleShape row;
            row.setPosition(listLeft, listTop + i*rowH);
            row.setSize({(float)listW, (float)rowH - 4});
            row.setFillColor(idx == selectedIndex ?
                sf::Color(255,246,196) :
                ((i%2==0)? sf::Color(250,250,250) : sf::Color(238,238,248))
            );
            t.draw(row);

            if (!s) continue;

            std::string line = s->getMaSach() + " | " + s->getTenSach()
                + " | " + s->getTacGia() + " | " + s->getTheLoai()
                + " | " + std::to_string(s->getNamXuatBan())
                + " | " + s->getNhaXuatBan()
                + " | " + s->getTinhTrang();

            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(18);
            txt.setFillColor(sf::Color::Black);
            txt.setString(line);
            txt.setPosition(listLeft + 8, listTop + i*rowH + 12);
            t.draw(txt);
        }
    }

    void onDelete() {
        if (selectedIndex < 0) {
            popup.show("Chua chon sach!");
            return;
        }

        int idx = selectedIndex;
        NodeBook* node = nodeAtIndex(idx);
        if (!node) {
            popup.show("Sach khong ton tai!");
            return;
        }

        // XOA NODE
        if (node == headBooks) {
            headBooks = headBooks->next;
        } else {
            NodeBook *p = headBooks;
            while (p->next != node) p = p->next;
            p->next = node->next;
        }
        delete node;
        popup.show("Da xoa sach!");
        selectedIndex = -1;
    }
};
