#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#include <SFML/Graphics.hpp>

class Background {
    sf::Texture tex;
    sf::Sprite  spr;
    bool loaded = false;

public:
    bool load(const std::string& path) {
        if (!tex.loadFromFile(path)) return false;
        tex.setSmooth(true);
        spr.setTexture(tex);
        loaded = true;
        return true;
    }

    // cover (giống CSS background-size: cover)
    void resizeToCover(const sf::Vector2u& winSize) {
        if (!loaded) return;
        auto sz = tex.getSize();
        if (sz.x == 0 || sz.y == 0) return;

        float sx = (float)winSize.x / (float)sz.x;
        float sy = (float)winSize.y / (float)sz.y;
        float s  = (sx > sy ? sx : sy);
        spr.setScale(s, s);

        // canh giữa
        sf::FloatRect b = spr.getGlobalBounds();
        spr.setPosition( (winSize.x - b.width)*0.5f, (winSize.y - b.height)*0.5f );
    }

    void draw(sf::RenderTarget& t) {
        if (loaded) t.draw(spr);
    }
};
#endif
