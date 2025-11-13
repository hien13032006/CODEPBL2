#ifndef SCREEN_READER_LOGIN_H
#define SCREEN_READER_LOGIN_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"
#include "Reader.h"
#include "LibrarySystem.h"
#include "ScreenBase.h"

class ScreenReaderLogin : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *library;
    Reader **currentReader;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Text lbUser;
    sf::Text lbPass;

public:
    TextBox boxUser;
    TextBox boxPass;
    Button btnOK;
    Button btnBack;

    ScreenReaderLogin(sf::Font &f, LibrarySystem *lib = nullptr, Reader **curReader = nullptr)
        : font(f), library(lib), currentReader(curReader)
    {
        // Texts
        lbUser.setFont(font);
        lbUser.setString("Username:");
        lbUser.setCharacterSize(24);
        lbUser.setFillColor(sf::Color::Black);

        lbPass.setFont(font);
        lbPass.setString("Password:");
        lbPass.setCharacterSize(24);
        lbPass.setFillColor(sf::Color::Black);
    }

    void init(sf::RenderWindow &window) {
        // Hình nền full screen
        if(!bgTexture.loadFromFile("picLogin.png")) {
            // xử lý lỗi nếu file không tồn tại
        }

        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgTexture.getSize().x,
            float(winSize.y) / bgTexture.getSize().y
        );

        // --- Vị trí căn giữa các ô ---
        float centerX = winSize.x * 0.5f;
        float boxWidth = 320.f;
        float boxHeight = 45.f;

        // --- Nhãn + ô nhập ---
        float startY = winSize.y * 0.35f;

        lbUser.setPosition(centerX - boxWidth - 20, startY + 10);
        boxUser = TextBox(font, {centerX - boxWidth * 0.3f, startY}, {boxWidth, boxHeight});

        lbPass.setPosition(centerX - boxWidth - 20, startY + 90);
        boxPass = TextBox(font, {centerX - boxWidth * 0.3f, startY + 80}, {boxWidth, boxHeight});
        boxPass.setPassword(true);

        // Khởi tạo TextBox theo tỷ lệ màn hình
        boxUser = TextBox(font, {winSize.x * 0.6f, winSize.y * 0.40f}, {350, 55});
        boxPass = TextBox(font, {winSize.x * 0.6f, winSize.y * 0.50f}, {350, 55});
        boxPass.setPassword(true);

        // Nút Login/Back bên phải
        btnOK   = Button("Login", font, {winSize.x * 0.55f, winSize.y * 0.60f}, {200, 60});
        btnBack = Button("Back",  font, {winSize.x * 0.70f, winSize.y * 0.60f}, {200, 60});
    }

    void handleEvent(sf::Event &e, AppState &current, sf::RenderWindow *window) override {
        if(!window) return;

        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
        btnOK.handleEvent(e, *window);
        btnBack.handleEvent(e, *window);

        // Ví dụ callback để thay đổi state
        btnOK.setCallback([&]() {
            current = SCREEN_READER_LOGIN; // hoặc xử lý login
        });
        btnBack.setCallback([&]() {
            current = SCREEN_WELCOME;
        });
    }

    void update() override {
        btnOK.update();
        btnBack.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        w.draw(lbUser);
        w.draw(lbPass);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif
