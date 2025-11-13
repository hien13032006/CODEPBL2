#ifndef THEME_H
#define THEME_H

#include <SFML/Graphics.hpp>

struct Theme {
    // Màu chữ, màu ô input
    static sf::Color Title;
    static sf::Color InputFill;
    static sf::Color InputOutline;

    static sf::Color BG;
    static sf::Color Text;
    static sf::Color Hint;
    static sf::Color Button;
    static sf::Color Hover;
    static sf::Color Danger;
    static sf::Color OK;
    static sf::Color Border;
    static sf::Color BorderFocus;

    static void centerTextX(sf::Text& text, float width) {
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        text.setPosition(width / 2, text.getPosition().y);
    }
};

sf::Color Theme::Title        = sf::Color::White;
sf::Color Theme::InputFill    = sf::Color(200,200,200);
sf::Color Theme::InputOutline = sf::Color::Black;

sf::Color Theme::BG           = sf::Color(236,240,241);
sf::Color Theme::Text         = sf::Color(44,62,80);
sf::Color Theme::Hint         = sf::Color(149,165,166);
sf::Color Theme::Button       = sf::Color(52,152,219);
sf::Color Theme::Hover        = sf::Color(41,128,185);
sf::Color Theme::Danger       = sf::Color(231,76,60);
sf::Color Theme::OK           = sf::Color(46,204,113);
sf::Color Theme::Border       = sf::Color(127,140,141);
sf::Color Theme::BorderFocus  = sf::Color(52,152,219);

#endif
