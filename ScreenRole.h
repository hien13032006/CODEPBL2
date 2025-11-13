#ifndef SCREEN_ROLE_H
#define SCREEN_ROLE_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"
#include <iostream>

class ScreenRole : public ScreenBase {
private:
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
public:
    Button btnReader;
    Button btnLibrarian;

    std::function<void()> onReader;
    std::function<void()> onLibrarian;

    ScreenRole(sf::Font &font, sf::RenderWindow &window)
        : btnReader("READER", font, {window.getSize().x*0.15f, window.getSize().y*0.7f}, {260,80}),
          btnLibrarian("LIBRARIAN", font, {window.getSize().x*0.65f, window.getSize().y*0.7f}, {260,80})
    {
        btnReader.setCallback([this](){
            std::cout << "Reader clicked!\n";
            if(onReader) onReader();
        });
        btnLibrarian.setCallback([this](){
            std::cout << "Librarian clicked!\n";
            if(onLibrarian) onLibrarian();
        });
    }

    void init(sf::RenderWindow &window) {
        if(!bgTexture.loadFromFile("pic2.png")) {
            std::cout << "Cannot load pic2.png\n";
        }
        bgSprite.setTexture(bgTexture);
        sf::Vector2u windowSize = window.getSize();
        bgSprite.setScale(
            float(windowSize.x) / bgTexture.getSize().x,
            float(windowSize.y) / bgTexture.getSize().y
        );
    }

    void handleEvent(sf::Event &e, AppState &state, sf::RenderWindow *w) override {
        if(!w) return;
        btnReader.handleEvent(e, *w);
        btnLibrarian.handleEvent(e, *w);
    }

    void update() override {
        btnReader.update();
        btnLibrarian.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        btnReader.draw(w);
        btnLibrarian.draw(w);
    }
};

#endif
