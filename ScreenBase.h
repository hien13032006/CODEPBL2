#ifndef SCREENBASE_H
#define SCREENBASE_H

#include <SFML/Graphics.hpp>
#include "AppState.h"

class ScreenBase {
public:
    virtual void handleEvent(sf::Event&, AppState&, sf::RenderWindow*) = 0;
    virtual void update() {}
    virtual void draw(sf::RenderWindow&) = 0;
    virtual ~ScreenBase() {}
};

#endif
