#pragma once
#include <SFML/Graphics.hpp>

namespace Theme {

    // ======== COLOR PALETTE ========

    // Color brand (primary)
    const sf::Color Primary      = sf::Color(  9, 87,194);
    const sf::Color PrimaryDark  = sf::Color(  4, 53,132);

    // Panel
    const sf::Color Panel        = sf::Color(255,255,255,240);
    const sf::Color PanelOutline = Primary;

    // Text
    const sf::Color Text         = sf::Color(25,25,25);
    const sf::Color Title        = Primary;

    // Input
    const sf::Color InputFill    = sf::Color(255,255,255);
    const sf::Color InputOutline = Primary;

    // Button
    const sf::Color BtnFill      = sf::Color(239,244,255);
    const sf::Color BtnHover     = sf::Color(194,214,255);
    const sf::Color BtnBorder    = Primary;

    // Background
    const sf::Color BG           = sf::Color(233,240,255);

    // ===== center helper =====
    inline void centerTextX(sf::Text& t, float screenW){
        auto b = t.getLocalBounds();
        t.setPosition(
            (screenW - b.width)/2.f - b.left,
            t.getPosition().y
        );
    }
}
