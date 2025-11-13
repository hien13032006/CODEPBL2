#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "ScreenBase.h"
#include "Button.h"
#include "Theme.h"
#include <iostream>

class ScreenWelcome : public ScreenBase {
private:
    sf::Font &font;
    Button btnVisit;
    Button btnExit;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    ScreenWelcome(sf::Font &f)
        : font(f),
          btnVisit("   VISIT", f, 40),
          btnExit("   EXIT", f, 40)
    {
        // --- Kích thước và vị trí các nút ---
        btnVisit.setSize(200, 60);
        btnExit.setSize(200, 60);

        btnVisit.setPosition(800, 800);      // Nút VISIT
        btnExit.setPosition(800, 900);      // Nút EXIT (bên phải VISIT)
    }

    void init(sf::RenderWindow &window) override {
        // Load hình nền
        if(!bgTexture.loadFromFile("pic3.png")) {
            std::cout << "Cannot load pic3.png\n";
            bgTexture.create(1024, 768);
        }
        bgSprite.setTexture(bgTexture);

        // Scale full màn hình
        sf::Vector2u windowSize = window.getSize();
        bgSprite.setScale(
            float(windowSize.x) / bgTexture.getSize().x,
            float(windowSize.y) / bgTexture.getSize().y
        );
    }

    void handleEvent(sf::Event &e, AppState &cur, sf::RenderWindow *w) override {
        if(!w) return;

        btnVisit.handleEvent(e, *w);
        btnExit.handleEvent(e, *w);

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = w->mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

            if (btnVisit.hit(mousePos.x, mousePos.y)) {
                cur = SCREEN_ROLE; // chuyển sang màn hình Role
            } 
            else if (btnExit.hit(mousePos.x, mousePos.y)) {
                w->close(); // đóng cửa sổ
            }
        }
    }

    void update() override {
        btnVisit.update();
        btnExit.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        btnVisit.draw(w);
        btnExit.draw(w);
    }
};

#endif
