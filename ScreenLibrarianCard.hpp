#ifndef SCREEN_LIBRARIAN_CARD_H
#define SCREEN_LIBRARIAN_CARD_H

#include <SFML/Graphics.hpp>
#include "Librarian.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class ScreenLibrarianCard {
private:
    sf::Font &font; Librarian *librarian;
    sf::Text title; 
    sf::Text lbMaID, lbHoTen, lbEmail, lbSDT, lbChucVu;
    bool closed; 
    RoundedRectangleShape panel; 
    Button btnClose;
    Button* btnUpdate;
    bool updateRequested;

    sf::Texture bgTexture;
    sf::Sprite background;
    bool bgLoaded = false;

public:
    ScreenLibrarianCard(sf::Font &f, Librarian *l) 
        : font(f), librarian(l), 
          btnClose({910, 135}, {40, 40}, "X", f, 0, Theme::Error) 
    {
        closed = false;
        updateRequested = false;

        if (bgTexture.loadFromFile("picCard.png")) {
            bgLoaded = true;

            background.setTexture(bgTexture);

            // Phù hợp với kích thước màn hình 1300x720
            float scaleX = 600.f / bgTexture.getSize().x;
            float scaleY = 450.f  / bgTexture.getSize().y;
            background.setScale(scaleX, scaleY);

            background.setPosition(350, 135);
        }

        panel.setSize({600, 450}); 
        panel.setCornerRadius(20.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(350, 135);

        title.setFont(font); 
        title.setString("THE THU THU");
        title.setCharacterSize(36); 
        title.setFillColor(Theme::TextDark); 
        title.setPosition(550, 205);

        auto setTxt = [&](sf::Text& t, std::string s, float y) {
            t.setFont(font); t.setString(s); 
            t.setCharacterSize(20); 
            t.setFillColor(Theme::TextLight);
            t.setPosition(530, y);
        };

        if (librarian) {
            setTxt(lbMaID,  "ID: " + librarian->getMaID(), 255);
            setTxt(lbHoTen, "Ten: " + librarian->getHoTen(), 295);
            setTxt(lbEmail, "Email: " + librarian->getEmail(), 335);
            setTxt(lbSDT,   "SDT: " + librarian->getSDT(), 375);
            setTxt(lbChucVu,"Chuc vu: " + librarian->getChucVu(), 415);
        } else {
            setTxt(lbMaID, "Loi: Khong co du lieu!", 230);
        }

        btnUpdate = new Button({500, 470}, {300, 50}, "Cap Nhat Thong Tin", font, 1, Theme::Secondary);
    }
    
    ~ScreenLibrarianCard() { delete btnUpdate; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { 
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) closed = true;
            if (btnUpdate->handleClick(mousePos)) {
                updateRequested = true;
                closed = true; 
            }
        }
    }
    
    void update(sf::Vector2f mousePos) {
        btnClose.update(mousePos);
        btnUpdate->update(mousePos);
    }

    void render(sf::RenderWindow &window) { 
        window.draw(panel); 
        if (bgLoaded)
           window.draw(background);
        window.draw(title); 
        window.draw(lbMaID); window.draw(lbHoTen); 
        window.draw(lbEmail); window.draw(lbSDT); window.draw(lbChucVu);
        btnClose.draw(window);
        btnUpdate->draw(window);
    }

    bool wasClosed() const { return closed; }
    bool isUpdateRequested() { return updateRequested; }
};
#endif