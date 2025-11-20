#ifndef SCREEN_LIBRARIAN_CARD_H
#define SCREEN_LIBRARIAN_CARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Librarian.h"
#include "Button.hpp"

class ScreenLibrarianCard {
private:
    sf::Font &font;
    Librarian *librarian;

    sf::Text title;

    sf::Text lbMaID, lbHoTen, lbSDT, lbEmail, lbChucVu;

    bool closed;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    Button btnClose;   // nút dấu X

    ScreenLibrarianCard(sf::Font &f, Librarian *l);

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos);
    void render(sf::RenderWindow &window);

    bool wasClosed() const { return closed; }
};

// ------------------ IMPLEMENTATION ------------------
ScreenLibrarianCard::ScreenLibrarianCard(sf::Font &f, Librarian *l)
    : font(f), librarian(l),
      btnClose({960, 300}, {40, 40}, "X", f)
{
    closed = false;

    // ---- Load nền ----
    if (!bgTexture.loadFromFile("picCard.png")) {
        std::cerr << "Khong load duoc anh nen!\n";
    }
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(400, 300); 
    sf::Vector2u texSize = bgTexture.getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        bgSprite.setScale(
            600.f / texSize.x,
            350.f / texSize.y
        );
    }

    // ---- Tiêu đề ----
    title.setFont(font);
    title.setString("THE THU THU");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::Black);
    title.setPosition(560, 350);

    // ---- Thông tin ----
    sf::Text *arr[] = {&lbMaID, &lbHoTen, &lbSDT, &lbEmail, &lbChucVu};
    for (auto t : arr) {
        t->setFont(font);
        t->setCharacterSize(24);
        t->setFillColor(sf::Color::Black);
    }

    lbMaID.setString("Ma ID: " + librarian->getMaID());
    lbHoTen.setString("Ho ten: " + librarian->getHoTen());
    lbSDT.setString("SDT: " + librarian->getSDT());
    lbEmail.setString("Email: " + librarian->getEmail());
    lbChucVu.setString("Chuc vu: " + librarian->getChucVu());

    lbMaID.setPosition(580, 405);
    lbHoTen.setPosition(580, 445);
    lbSDT.setPosition(580, 485);
    lbEmail.setPosition(580, 525);
    lbChucVu.setPosition(580, 565);

    // ---- Nút đóng ----
    btnClose.setColor(sf::Color(230, 90, 90));  
}

void ScreenLibrarianCard::handleEvent(sf::Event &e, const sf::Vector2f &mousePos) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (btnClose.handleClick(mousePos)) {
            closed = true;
        }
    }
}

void ScreenLibrarianCard::render(sf::RenderWindow &window) {
    window.draw(bgSprite);         
    window.draw(title);
    window.draw(lbMaID);
    window.draw(lbHoTen);
    window.draw(lbSDT);
    window.draw(lbEmail);
    window.draw(lbChucVu);

    btnClose.draw(window);     
}

#endif
