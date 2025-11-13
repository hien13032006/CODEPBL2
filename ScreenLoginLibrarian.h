#ifndef SCREEN_LIBRARIAN_LOGIN_H
#define SCREEN_LIBRARIAN_LOGIN_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include "ScreenBase.h"

class ScreenLoginLibrarian : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem* library;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Text lbUser;
    sf::Text lbPass;
    sf::Text lbError;

public:
    TextBox boxUser;
    TextBox boxPass;
    Button btnOK;
    Button btnBack;

    ScreenLoginLibrarian(sf::Font &f, LibrarySystem* lib)
        : font(f), library(lib)
    {
        // Nhãn
        lbUser.setFont(font);
        lbUser.setString("Username:");
        lbUser.setCharacterSize(24);
        lbUser.setFillColor(sf::Color::Black);

        lbPass.setFont(font);
        lbPass.setString("Password:");
        lbPass.setCharacterSize(24);
        lbPass.setFillColor(sf::Color::Black);

        lbError.setFont(font);
        lbError.setCharacterSize(22);
        lbError.setFillColor(sf::Color::Red);
        lbError.setString("");
    }

    void init(sf::RenderWindow &window) override {
        // Hình nền
        if(!bgTexture.loadFromFile("picLogin.png")) {
            bgTexture.create(1024, 768);
        }
        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgTexture.getSize().x,
            float(winSize.y) / bgTexture.getSize().y
        );

        // Vị trí textbox + nhãn
        float startX = winSize.x * 0.6f;
        float startY = winSize.y * 0.35f;
        float gap = 80.f;

        lbUser.setPosition(startX - 150, startY + 10);
        lbPass.setPosition(startX - 150, startY + gap + 10);
        lbError.setPosition(startX, startY + gap * 2 + 10);

        boxUser = TextBox(font, {startX, startY}, {350, 50});
        boxPass = TextBox(font, {startX, startY + gap}, {350, 50});
        boxPass.setPassword(true);

        // Nút Login/Back
        btnOK   = Button("Login", font, {startX + 10, startY + gap * 2 + 50}, {150, 50});
        btnBack = Button("Back",  font, {startX + 200, startY + gap * 2 + 50}, {150, 50});
    }

    void handleEvent(sf::Event &e, AppState &current, sf::RenderWindow *window) override {
        if(!window) return;

        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
        btnOK.handleEvent(e, *window);
        btnBack.handleEvent(e, *window);

        // Callback Login
        btnOK.setCallback([&]() {
            std::string username = boxUser.get();
            std::string password = boxPass.get();

            if(system){
                USER* tt = library->DangNhapThuThu(username, password);
                if(tt){
                    current = SCREEN_LIB_MENU;
                    lbError.setString("");
                } else {
                    lbError.setString("Sai username hoac password!");
                }
            } else {
                lbError.setString("Khong the truy cap he thong!");
            }
        });

        // Callback Back
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
        w.draw(lbError);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif
