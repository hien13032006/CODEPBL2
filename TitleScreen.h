#pragma once
#include "ScreenBase.h"
#include "Button.h"

class TitleScreen : public ScreenBase {
private:
    sf::Font font;
    sf::Text title;
    Button btnStart;
    std::function<void()> goNext;

public:
    TitleScreen(std::function<void()> next) : goNext(next) {}

    void init() override {
        font.loadFromFile("assets/Roboto-Regular.ttf");

        title.setFont(font);
        title.setString("THU VIEN BACH KHOA");
        title.setCharacterSize(42);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(250, 120);

        btnStart = Button(font, "START", 420, 300);
        btnStart.setCallback(goNext);
    }

    void handle(const sf::Event& evt) override {
        btnStart.handle(evt);
    }

    void update(const sf::RenderWindow& win) override {
        btnStart.update(win);
    }

    void draw(sf::RenderTarget& t) override {
        t.draw(title);
        btnStart.draw(t);
    }
};
