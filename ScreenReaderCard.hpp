#ifndef SCREEN_READER_CARD_H
#define SCREEN_READER_CARD_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class ScreenReaderCard {
private:
    sf::Font &font; 
    Reader *reader; 
    sf::Text title; 
    sf::Text lbMaID, lbHoTen, lbEmail, lbSDT, lbNgayDK;
    bool closed; 
    RoundedRectangleShape panel; 
    Button btnClose; 
    Button* btnUpdate; 
    bool updateRequested;

public:
    ScreenReaderCard(sf::Font &f, Reader *r) 
        : font(f), reader(r), 
          btnClose({910, 135}, {40, 40}, "X", f, 0, Theme::Error) 
    {
        closed = false;
        updateRequested = false;

        // Panel 600x450, Căn giữa màn hình 1300x720
        // X = (1300-600)/2 = 350, Y = (720-450)/2 = 135
        panel.setSize({600, 450}); 
        panel.setCornerRadius(20.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(350, 135);

        title.setFont(font); 
        title.setString("THE BAN DOC");
        title.setCharacterSize(36); 
        title.setFillColor(Theme::TextDark); 
        title.setPosition(530, 160);

        auto setTxt = [&](sf::Text& t, std::string s, float y) {
            t.setFont(font); t.setString(s); 
            t.setCharacterSize(20); 
            t.setFillColor(Theme::TextLight);
            t.setPosition(400, y);
        };

        if (reader) {
            char dateBuf[20];
            time_t t = reader->getNgayDangKy();
            tm* ltm = localtime(&t);
            if(ltm) strftime(dateBuf, 20, "%d/%m/%Y", ltm);
            else sprintf(dateBuf, "N/A");

            setTxt(lbMaID,   "ID: " + reader->getMaID(), 230);
            setTxt(lbHoTen,  "Ten: " + reader->getHoTen(), 270);
            setTxt(lbEmail,  "Email: " + reader->getEmail(), 310);
            setTxt(lbSDT,    "SDT: " + reader->getSDT(), 350);
            setTxt(lbNgayDK, "Ngay DK: " + std::string(dateBuf), 390);
        } else {
             setTxt(lbMaID, "Loi: Khong co du lieu doc gia!", 230);
        }

        btnUpdate = new Button({500, 460}, {300, 50}, "Cap Nhat Thong Tin", font, 1, Theme::Secondary);
    }
    
    ~ScreenReaderCard() { delete btnUpdate; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { 
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) closed = true;
            if (btnUpdate->handleClick(mousePos)) updateRequested = true;
        }
    }
    
    void update(sf::Vector2f mousePos) {
        btnClose.update(mousePos);
        btnUpdate->update(mousePos);
    }

    void render(sf::RenderWindow &window) { 
        window.draw(panel); 
        window.draw(title); 
        window.draw(lbMaID); window.draw(lbHoTen); 
        window.draw(lbEmail); window.draw(lbSDT); window.draw(lbNgayDK);
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