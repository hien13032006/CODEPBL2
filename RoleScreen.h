#pragma once
#include "ScreenBase.h"
#include "Button.h"

class RoleScreen : public ScreenBase {
private:
    sf::Font font;
    sf::Text title;

    Button btnReader;
    Button btnLibrarian;
    Button btnBack;

    std::function<void()> onReader;
    std::function<void()> onLibrarian;
    std::function<void()> onBack;

public:
    RoleScreen(
        std::function<void()> reader,
        std::function<void()> librarian,
        std::function<void()> back
    )
    : onReader(reader), onLibrarian(librarian), onBack(back) {}

    void init() override {
        font.loadFromFile("assets/Roboto-Regular.ttf");

        title.setFont(font);
        title.setString("CHON PHAN QUYEN");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(330, 100);

        btnReader    = Button(font, "READER",    280, 260);
        btnLibrarian = Button(font, "LIBRARIAN", 480, 260);
        btnBack      = Button(font, "BACK",      680, 260);

        btnReader.setCallback(onReader);
        btnLibrarian.setCallback(onLibrarian);
        btnBack.setCallback(onBack);
    }

    void handle(const sf::Event& evt) override {
        btnReader.handle(evt);
        btnLibrarian.handle(evt);
        btnBack.handle(evt);
    }

    void update(const sf::RenderWindow& win) override {
        btnReader.update(win);
        btnLibrarian.update(win);
        btnBack.update(win);
    }

    void draw(sf::RenderTarget& t) override {
        t.draw(title);
        btnReader.draw(t);
        btnLibrarian.draw(t);
        btnBack.draw(t);
    }
};
