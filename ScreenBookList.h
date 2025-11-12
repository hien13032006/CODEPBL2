#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Book.h"
#include "Button.h"

class ScreenBookList {
private:
    sf::Font font;
    sf::Text title;

    NodeBook *head;

    Button btnBack;
    std::function<void()> onBack;

public:
    ScreenBookList(NodeBook *h, std::function<void()> backFunc)
    : head(h), onBack(backFunc) {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("DANH SACH SACH");
        title.setCharacterSize(30);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(400, 20);

        btnBack = Button(font, "BACK", 900, 20);
        btnBack.setCallback(onBack);
    }

    void handle(const sf::Event& evt) {
        btnBack.handle(evt);
    }

    void update(const sf::RenderWindow& w) {
        btnBack.update(w);
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);

        float x = 50.f;
        float y = 100.f;

        for (NodeBook *p = head; p; p = p->next) {
            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(20);
            line.setFillColor(sf::Color::Black);

            std::string s =
                p->data->getMaSach() + "  |  " +
                p->data->getTenSach() + "  |  " +
                p->data->getTacGia();

            line.setString(s);
            line.setPosition(x, y);
            t.draw(line);
            y += 30.f;
        }

        btnBack.draw(t);
    }
};

#endif