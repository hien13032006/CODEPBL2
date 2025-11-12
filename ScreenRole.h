#ifndef SCREEN_ROLE_H
#define SCREEN_ROLE_H
#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"

class ScreenRole : public ScreenBase
{
    sf::Font font;
    Button btnReader;
    Button btnLibrarian;

public:
    ScreenRole(sf::Font &f)
        : font(f)
    {
        auto size = sf::VideoMode::getDesktopMode();
        float W = size.width;
        float H = size.height;

        btnReader    = Button("READER",    font, {W*0.25f, H*0.50f}, {260,80});
        btnLibrarian = Button("LIBRARIAN", font, {W*0.75f, H*0.50f}, {260,80});
    }

    void handleEvent(sf::Event &e, AppState &state) override
    {
        if(e.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);

            if(btnReader.contains(mp))
                state = SCREEN_READER_CHOICE;

            if(btnLibrarian.contains(mp))
                state = SCREEN_LIB_LOGIN;
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override
    {
        btnReader.draw(w);
        btnLibrarian.draw(w);
    }
};

#endif