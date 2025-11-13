#ifndef SCREEN_VISIT_H
#define SCREEN_VISIT_H

#include <SFML/Graphics.hpp>
#include "Button.h"

class ScreenVisit {
private:
    Button btnVisit; // dùng object, không dùng con trỏ
    std::function<void()> onVisit;

public:
    ScreenVisit(std::function<void()> visitCallback)
        : onVisit(visitCallback) {}

    void init(sf::Font& font) {
        // Khởi tạo Button với label, font, vị trí, kích thước
        btnVisit = new Button("VISIT", font, sf::Vector2f(500, 450), sf::Vector2f(200, 50));
        btnVisit->setCallback(onVisit);

    }

    void handle(sf::Event& evt, sf::RenderWindow& window) {
        btnVisit->handleEvent(evt, window);  // dùng handleEvent của Button
    }

    void update() {
        btnVisit->update();
    }

    void draw(sf::RenderWindow& win) {
        btnVisit.draw(win);
    }
};

#endif
