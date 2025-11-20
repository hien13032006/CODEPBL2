#ifndef SCREEN_READER_CARD_H
#define SCREEN_READER_CARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Reader.h"
#include "Button.hpp"
#include <ctime>
#include <string>


class ScreenReaderCard {
private:
    sf::Font &font;
    Reader *reader;

    sf::Text title;
    sf::Text lbMaID, lbHoTen, lbEmail, lbSDT, lbNgayDK;

    bool closed;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

public:
    Button btnClose;   // nút dấu X

    ScreenReaderCard(sf::Font &f, Reader *r);

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos);
    void render(sf::RenderWindow &window);

    bool wasClosed() const { return closed; }
};

// ------------------ IMPLEMENTATION ------------------
ScreenReaderCard::ScreenReaderCard(sf::Font &f, Reader *r)
    : font(f), reader(r),
      btnClose({960, 300}, {40, 40}, "X", f)
{
    closed = false;

    // ---- Load nền ----
    if (!bgTexture.loadFromFile("picCard.png")) {
        std::cerr << "Khong load duoc anh nen!\n";
    }
    bgSprite.setTexture(bgTexture);
    bgSprite.setPosition(400, 300); // cùng vị trí với cardBox
    sf::Vector2u texSize = bgTexture.getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        bgSprite.setScale(
            600.f / texSize.x,
            350.f / texSize.y
        );
    }

    // ---- Tiêu đề ----
    title.setFont(font);
    title.setString("THE BAN DOC");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::Black);
    title.setPosition(550, 350);

    // ---- Thông tin ----
    sf::Text *arr[] = {&lbMaID, &lbHoTen, &lbEmail, &lbSDT, &lbNgayDK};
    for (auto t : arr) {
        t->setFont(font);
        t->setCharacterSize(24);
        t->setFillColor(sf::Color::Black);
    }

    // Lấy thông tin từ Reader đã nạp từ file
    lbMaID.setString("Ma ID: " + reader->getMaID());
    lbHoTen.setString("Ho ten: " + reader->getHoTen());
    lbEmail.setString("Email: " + reader->getEmail());
    lbSDT.setString("SDT: " + reader->getSDT());
    lbNgayDK.setString("Ngay dang ky: " + layNgayDangKy(reader->getNgayDangKy()));

    lbMaID.setPosition(580, 405);
    lbHoTen.setPosition(580, 445);
    lbEmail.setPosition(580, 485);
    lbSDT.setPosition(580, 525);
    lbNgayDK.setPosition(580, 565);

    // ---- Nút đóng ----
    btnClose.setColor(sf::Color(230, 90, 90));  
}

void ScreenReaderCard::handleEvent(sf::Event &e, const sf::Vector2f &mousePos) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (btnClose.handleClick(mousePos)) {
            closed = true;
        }
    }
}

void ScreenReaderCard::render(sf::RenderWindow &window) {
    window.draw(bgSprite);    // nền
    window.draw(title);
    window.draw(lbMaID);
    window.draw(lbHoTen);
    window.draw(lbEmail);
    window.draw(lbSDT);
    window.draw(lbNgayDK);

    btnClose.draw(window);      // nút đóng
}

#endif
