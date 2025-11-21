#ifndef SCREEN_LIBRARIAN_CARD_H
#define SCREEN_LIBRARIAN_CARD_H

#include <SFML/Graphics.hpp>
#include "Librarian.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

class ScreenLibrarianCard {
private:
    sf::Font &font; Librarian *librarian;
    sf::Text title; 
    sf::Text lbMaID, lbHoTen, lbEmail, lbSDT, lbChucVu;
    bool closed; 
    RoundedRectangleShape panel; 
    Button btnClose;
    
    // Nút cập nhật thông tin
    Button* btnUpdate;
    bool updateRequested;

public:
    ScreenLibrarianCard(sf::Font &f, Librarian *l) 
        : font(f), librarian(l), 
          btnClose({960, 200}, {40, 40}, "X", f, 0, sf::Color(230, 90, 90)) 
    {
        closed = false;
        updateRequested = false;

        // Panel (600x450)
        panel.setSize({600, 450}); 
        panel.setCornerRadius(20.0f);
        panel.setFillColor(sf::Color(255, 255, 255)); // Thẻ màu trắng cho nổi bật
        panel.setPosition(400, 200);

        title.setFont(font); 
        title.setString("THE THU THU");
        title.setCharacterSize(36); 
        title.setFillColor(sf::Color(40, 45, 60)); // Chữ tối
        title.setPosition(570, 230);

        auto setTxt = [&](sf::Text& t, std::string s, float y) {
            t.setFont(font); t.setString(s); 
            t.setCharacterSize(22); // Chữ to
            t.setFillColor(sf::Color(60, 60, 60));
            t.setPosition(450, y);
        };

        setTxt(lbMaID,  "ID: " + librarian->getMaID(), 300);
        setTxt(lbHoTen, "Ten: " + librarian->getHoTen(), 340);
        setTxt(lbEmail, "Email: " + librarian->getEmail(), 380);
        setTxt(lbSDT,   "SDT: " + librarian->getSDT(), 420);
        setTxt(lbChucVu,"Chuc vu: " + librarian->getChucVu(), 460);

        // Nút cập nhật
        btnUpdate = new Button(
            {550, 530}, {300, 50}, "Cap Nhat Thong Tin", 
            font, 1, sf::Color(60, 100, 180)
        );
    }
    
    ~ScreenLibrarianCard() { delete btnUpdate; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { 
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) closed = true;
            if (btnUpdate->handleClick(mousePos)) updateRequested = true;
        }
    }

    void render(sf::RenderWindow &window) { 
        window.draw(panel); 
        window.draw(title); 
        window.draw(lbMaID); window.draw(lbHoTen); 
        window.draw(lbEmail); window.draw(lbSDT); window.draw(lbChucVu);
        btnClose.draw(window);
        btnUpdate->draw(window);
    }

    bool wasClosed() const { return closed; }
    bool isUpdateRequested() { 
        if (updateRequested) { updateRequested = false; return true; }
        return false;
    }
};
#endif