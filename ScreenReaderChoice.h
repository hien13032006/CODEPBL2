#ifndef SCREEN_READER_CHOICE_H
#define SCREEN_READER_CHOICE_H

#include "ScreenBase.h"
#include "Button.h"
#include "AppState.h"
#include "Theme.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class ScreenReaderChoice : public ScreenBase {
private:
    sf::Font font;
    Button btnLogin;
    Button btnRegister;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    ScreenReaderChoice(sf::Font &f) : font(f) {
        auto size = sf::VideoMode::getDesktopMode();
        float W = size.width;
        float H = size.height;

        // --- Khởi tạo các nút ---
        btnLogin    = Button("LOG IN", font, {W * 0.40f, H * 0.35f}, {300, 80});
        btnRegister = Button("SIGN IN",   font, {W * 0.40f, H * 0.60f}, {300, 80});
    }

    void init(sf::RenderWindow &window) override {
        // --- Load hình nền ---
        if (!bgTexture.loadFromFile("picChoice.png")) {
            std::cout << "Không thể tải ảnh picReaderChoice.png\n";
            bgTexture.create(1024, 768); // fallback nếu thiếu file
        }

        bgSprite.setTexture(bgTexture);

        // --- Scale full màn hình ---
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgTexture.getSize().x,
            float(winSize.y) / bgTexture.getSize().y
        );
    }

    void handleEvent(sf::Event &e, AppState &state, sf::RenderWindow *w) override {
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);

            if (btnLogin.contains(mp))
                state = SCREEN_READER_LOGIN;

            if (btnRegister.contains(mp))
                state = SCREEN_READER_REGISTER;
        }
    }

    void update() override {
        btnLogin.update();
        btnRegister.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);       // vẽ hình nền
        btnLogin.draw(w);       // vẽ các nút
        btnRegister.draw(w);
    }
};

#endif
