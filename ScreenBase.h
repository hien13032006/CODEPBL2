#pragma once
#include <SFML/Graphics.hpp>

class ScreenBase {
public:
    virtual void init() {}
    virtual void handle(const sf::Event& evt) {}
    virtual void update(const sf::RenderWindow& win) {}
    virtual void draw(sf::RenderTarget& t) {}
};
