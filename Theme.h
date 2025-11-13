#ifndef THEME_H
#define THEME_H
#include <SFML/Graphics.hpp>

namespace Theme {
    const sf::Color BG         = sf::Color(225,235,250);
    const sf::Color Panel      = sf::Color(255,255,255);
    const sf::Color Border     = sf::Color(180,180,200);
    const sf::Color BorderFocus= sf::Color(70,120,200);

    const sf::Color BtnNormal  = sf::Color(245,245,245);
    const sf::Color BtnHover   = sf::Color(200,220,255);
    const sf::Color BtnClick   = sf::Color(160,200,255);
    const sf::Color BtnText    = sf::Color(40,40,40);

    const sf::Color Title      = sf::Color(40,40,80);
    const sf::Color Label      = sf::Color(60,60,60);
}

#endif