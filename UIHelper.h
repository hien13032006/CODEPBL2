#ifndef UI_HELPER_H
#define UI_HELPER_H
#include <SFML/Graphics.hpp>

inline void drawBackground(sf::RenderWindow &w, sf::Color bgColor = sf::Color(220,235,250)) {
    w.clear(bgColor);
}

inline void drawTitle(sf::RenderWindow &w, sf::Font &font, const std::string &title, float winW, float y = 60) {
    sf::Text t;
    t.setFont(font);
    t.setString(title);
    t.setCharacterSize(42);
    t.setFillColor(sf::Color(30,60,120));
    sf::FloatRect tb = t.getLocalBounds();
    t.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    t.setPosition(winW / 2.f, y);
    w.draw(t);
}

#endif