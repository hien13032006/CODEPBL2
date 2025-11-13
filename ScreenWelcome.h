#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"

class ScreenWelcome : public ScreenBase {
private:
    sf::Font &font;
    Button btnVisit;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    ScreenWelcome(sf::Font &f)
        : font(f), btnVisit("VISIT", f, 26)
    {
        btnVisit.setSize(180, 55);
        btnVisit.setPosition(800, 800);
    }

    void init(sf::RenderWindow &window) {
        if(!bgTexture.loadFromFile("pic1.png")) {
            std::cout << "Cannot load pic1.png\n";
        }
        bgSprite.setTexture(bgTexture);
        // Scale full window
        sf::Vector2u windowSize = window.getSize();
        bgSprite.setScale(
            float(windowSize.x) / bgTexture.getSize().x,
            float(windowSize.y) / bgTexture.getSize().y
        );
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        if(e.type == sf::Event::MouseButtonPressed && w){
            sf::Vector2f mousePos = w->mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
            if(btnVisit.hit(mousePos.x, mousePos.y)){
                cur = SCREEN_ROLE;
            }
        }
    }

    void update() override {
        btnVisit.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);    // Vẽ hình nền trước
        btnVisit.draw(w);    // Vẽ nút sau
    }
};

#endif
