#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

#include <SFML/Graphics.hpp>
#include "AppState.h"

class ScreenBase {
public:
    virtual ~ScreenBase() {}
    virtual void handleEvent(sf::Event &e, AppState &cur) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow &w) = 0;
};

#endif