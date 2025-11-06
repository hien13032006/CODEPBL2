#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Book.h"
#include "BookSearch.h"
#include "Button.h"

class ScreenSearchBook {
private:
    sf::Font font;
    sf::Text title;

    sf::RectangleShape boxInput;
    sf::String input;

    NodeBook *books;           // danh sách gốc
    NodeBook *result = nullptr;

    Button btnSearch, btnBack;
    std::function<void()> onBack;

public:
    ScreenSearchBook(NodeBook* head, std::function<void()> backFn)
    : books(head), onBack(backFn) {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("TIM KIEM SACH");
        title.setCharacterSize(28);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(450, 40);

        boxInput.setSize({400, 40});
        boxInput.setPosition(350, 120);
        boxInput.setFillColor(sf::Color::White);
        boxInput.setOutlineColor(sf::Color(60,120,200));
        boxInput.setOutlineThickness(2);

        btnSearch = Button(font, "SEARCH", 400, 200);
        btnBack   = Button(font, "BACK",   600, 200);

        btnSearch.setCallback([&](){
            // tìm kiếm
            if (result) {
                // free list cũ
                NodeBook *tmp;
                while (result) {
                    tmp = result;
                    result = result->next;
                    delete tmp->data;
                    delete tmp;
                }
            }
            result = searchBook(books, input.toAnsiString());
        });

        btnBack.setCallback(onBack);
    }

    void handle(const sf::Event& evt) {
        btnSearch.handle(evt);
        btnBack.handle(evt);

        if (evt.type == sf::Event::TextEntered) {
            if (evt.text.unicode == 8) { // backspace
                if (!input.isEmpty())
                    input.erase(input.getSize()-1, 1);
            }
            else if (evt.text.unicode >= 32 && evt.text.unicode <= 126) {
                input += evt.text.unicode;
            }
        }
    }

    void update(const sf::RenderWindow& w) {
        btnSearch.update(w);
        btnBack.update(w);
    }

    void draw(sf::RenderTarget &t) {
        t.draw(title);
        t.draw(boxInput);

        sf::Text ti(input, font, 20);
        ti.setFillColor(sf::Color::Black);
        ti.setPosition(355, 125);
        t.draw(ti);

        btnSearch.draw(t);
        btnBack.draw(t);

        // Vẽ kết quả
        float x = 50.f;
        float y = 280.f;

        for (NodeBook *p = result; p; p = p->next) {
            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(20);
            line.setFillColor(sf::Color::Black);

            string s =
                p->data->getMaSach() + " | " +
                p->data->getTenSach() + " | " +
                p->data->getTacGia();

            line.setString(s);
            line.setPosition(x, y);
            t.draw(line);
            y += 30.f;
        }
    }
};
