#ifndef SCREEN_READER_CHOICE_H
#define SCREEN_READER_CHOICE_H

#include "ScreenBase.h"
#include "Button.h"
#include "AppState.h"
#include "Theme.h"

class ScreenReaderChoice : public ScreenBase
{
    sf::Font font;
    Button btnLogin;
    Button btnRegister;

public:
    ScreenReaderChoice(sf::Font &f) : font(f)
    {
        auto size = sf::VideoMode::getDesktopMode();
        float W = size.width;
        float H = size.height;

        btnLogin    = Button("DANG NHAP", font, {W*0.30f, H*0.55f}, {300,80});
        btnRegister = Button("DANG KY",   font, {W*0.60f, H*0.55f}, {300,80});
    }

    void handleEvent(sf::Event &e, AppState &state) override
    {
        if(e.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);

            if(btnLogin.contains(mp))
                state = SCREEN_READER_LOGIN;

            if(btnRegister.contains(mp))
                state = SCREEN_READER_REGISTER;
        }
    }
    void update(float dt) override {}
    void draw(sf::RenderWindow &w) override
    {
        btnLogin.draw(w);
        btnRegister.draw(w);
    }
};
#endif