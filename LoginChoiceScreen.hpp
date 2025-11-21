#ifndef LOGIN_CHOICE_SCREEN_HPP
#define LOGIN_CHOICE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Modal.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

class LoginChoiceScreen {
private:
    Modal* modal;
    RoundedRectangleShape panel;
    sf::Text titleText, descText;
    Button* librarianButton;
    Button* readerButton;
    Button btnClose;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    bool closed;
    sf::Font& font;

public:
    LoginChoiceScreen(sf::Font& f, Modal* modalRef)
        : font(f), modal(modalRef),
          btnClose({1030, 220}, {50, 50}, "X", f, 0, sf::Color(200, 80, 80)),
          closed(false)
    {
        // 1. Panel Chính (Size 800x500)
        panel.setSize({800, 500});
        panel.setCornerRadius(20.0f);
        panel.setFillColor(sf::Color(255, 255, 255)); // Nền trắng cho sạch
        panel.setPosition(300, 200);

        // 2. Ảnh minh họa (Bên trái)
        if (bgTexture.loadFromFile("picChoice1.png")) {
            bgSprite.setTexture(bgTexture);
            // Scale ảnh cho vừa một nửa panel
            sf::Vector2u size = bgTexture.getSize();
            float scaleY = 500.0f / size.y;
            bgSprite.setScale(scaleY, scaleY); 
            bgSprite.setPosition(300, 200); // Góc trái panel
        }

        // 3. Title (Bên phải)
        titleText.setFont(font);
        titleText.setString("CHAO MUNG");
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color(40, 45, 60));
        titleText.setPosition(750, 260);

        descText.setFont(font);
        descText.setString("Ban muon dang nhap voi tu cach?");
        descText.setCharacterSize(20);
        descText.setFillColor(sf::Color(100, 100, 100));
        descText.setPosition(730, 320);

        // 4. Buttons (To, Rõ)
        librarianButton = new Button(
            {750, 380}, {250, 60}, "Thu Thu", font, 0, sf::Color(100, 150, 255)
        );
        librarianButton->getText().setCharacterSize(22);

        readerButton = new Button(
            {750, 460}, {250, 60}, "Doc Gia", font, 0, sf::Color(255, 150, 100)
        );
        readerButton->getText().setCharacterSize(22);
    }

    ~LoginChoiceScreen() { delete librarianButton; delete readerButton; }

    void reset() { closed = false; }
    bool isClosed() const { return closed; }

    void handleEvent(sf::Event& e, const sf::Vector2f& mousePos) {
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) closed = true;
        }
    }

    void update(sf::Vector2f mousePos) {
        librarianButton->update(mousePos);
        readerButton->update(mousePos);
        btnClose.update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (librarianButton->handleClick(mousePos)) return 1;
        if (readerButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(panel);
            if (bgTexture.getSize().x > 0) window.draw(bgSprite);
            
            window.draw(titleText);
            window.draw(descText);
            librarianButton->draw(window);
            readerButton->draw(window);
            btnClose.draw(window);
        }
    }
};

#endif