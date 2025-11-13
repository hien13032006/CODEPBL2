#ifndef SCREEN_READER_REGISTER_H
#define SCREEN_READER_REGISTER_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include "ScreenBase.h"

class ScreenReaderRegister : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *library;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Text lbName, lbPhone, lbEmail, lbUser, lbPass, lbError;

public:
    TextBox boxName, boxPhone, boxEmail, boxUser, boxPass;
    Button btnOK, btnBack;

    ScreenReaderRegister(sf::Font &f, LibrarySystem *lib = nullptr)
        : font(f), library(lib) {}

    void init(sf::RenderWindow &window) override {
        // --- Hình nền ---
        if (!bgTexture.loadFromFile("picSignin.png")) {
            // fallback nếu không có hình
            bgTexture.create(800, 600);
        }

        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgTexture.getSize().x,
            float(winSize.y) / bgTexture.getSize().y
        );

        // --- Nhãn ---
        auto setupLabel = [&](sf::Text &label, const std::string &str, float x, float y) {
            label.setFont(font);
            label.setString(str);
            label.setCharacterSize(24);
            label.setFillColor(sf::Color::Black);
            label.setPosition(x, y);
        };

        float startX = winSize.x * 0.4f;
        float startY = winSize.y * 0.25f;
        float gap = 80.f;

        setupLabel(lbName,  "Full Name:", startX - 150, startY);
        setupLabel(lbPhone, "Phone number:",    startX - 200, startY + gap);
        setupLabel(lbEmail, "Email:",  startX - 150, startY + gap * 2);
        setupLabel(lbUser,  "Username:", startX - 150, startY + gap * 3);
        setupLabel(lbPass,  "Password:", startX - 150, startY + gap * 4);

        // --- Ô nhập liệu ---
        boxName  = TextBox(font, {startX, startY}, {350, 50});
        boxPhone = TextBox(font, {startX, startY + gap}, {350, 50});
        boxEmail = TextBox(font, {startX, startY + gap * 2}, {350, 50});
        boxUser  = TextBox(font, {startX, startY + gap * 3}, {350, 50});
        boxPass  = TextBox(font, {startX, startY + gap * 4}, {350, 50});
        boxPass.setPassword(true);

        // --- Nút ---
        btnOK   = Button("Signin", font, {startX + 10, startY + gap * 5 + 10}, {150, 55});
        btnBack = Button("Back", font, {startX + 240, startY + gap * 5 + 10}, {150, 55});

        // --- Nhãn lỗi ---
        lbError.setFont(font);
        lbError.setCharacterSize(22);
        lbError.setFillColor(sf::Color::Red);
        lbError.setPosition(startX, startY + gap * 6);
        lbError.setString("");
    }

    void handleEvent(sf::Event &e, AppState &current, sf::RenderWindow *w) override {
        boxName.handleEvent(e);
        boxPhone.handleEvent(e);
        boxEmail.handleEvent(e);
        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
        btnOK.handleEvent(e, *w);
        btnBack.handleEvent(e, *w);

        btnOK.setCallback([&]() {
            std::string name = boxName.getText();
            std::string phone = boxPhone.getText();
            std::string email = boxEmail.getText();
            std::string username = boxUser.getText();
            std::string password = boxPass.getText();

            if (library) {
                library->DangKyDocGia(name, phone, email, username, password);
                lbError.setString("\nDANG KI THANH CONG!");
                current = SCREEN_READER_LOGIN;
            } else {
               lbError.setString("Khong the truy cap he thong!");
            }
        });

        btnBack.setCallback([&]() {
            current = SCREEN_READER_CHOICE;
        });
    }

    void update() override {
        btnOK.update();
        btnBack.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        w.draw(lbName);
        w.draw(lbPhone);
        w.draw(lbEmail);
        w.draw(lbUser);
        w.draw(lbPass);
        boxName.draw(w);
        boxPhone.draw(w);
        boxEmail.draw(w);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
        w.draw(lbError);
    }
};

#endif
