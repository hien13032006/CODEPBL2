#ifndef SCREEN_LIBRARIAN_CARD_H
#define SCREEN_LIBRARIAN_CARD_H
#include <SFML/Graphics.hpp>
#include "Librarian.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class ScreenLibrarianCard {
private:
    sf::Font &font; Librarian *librarian; sf::Text title, lbMaID, lbHoTen, lbEmail, lbSDT, lbChucVu; bool closed; RoundedRectangleShape panel; Button btnClose; Button* btnUpdate; bool updateRequested;
public:
    ScreenLibrarianCard(sf::Font &f, Librarian *l) : font(f), librarian(l), btnClose({910, 135}, {40, 40}, "X", f, 0, Theme::Error) {
        closed = false; updateRequested = false;
        panel.setSize({600, 450}); panel.setCornerRadius(20.0f); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(350, 135);
        title.setFont(font); title.setString("THE THU THU"); title.setCharacterSize(36); title.setFillColor(Theme::TextDark); title.setPosition(520, 160);
        auto setTxt = [&](sf::Text& t, std::string s, float y) { t.setFont(font); t.setString(s); t.setCharacterSize(22); t.setFillColor(Theme::TextLight); t.setPosition(400, y); };
        setTxt(lbMaID, "ID: " + librarian->getMaID(), 230); setTxt(lbHoTen, "Ten: " + librarian->getHoTen(), 270); setTxt(lbEmail, "Email: " + librarian->getEmail(), 310); setTxt(lbSDT, "SDT: " + librarian->getSDT(), 350); setTxt(lbChucVu,"Chuc vu: " + librarian->getChucVu(), 390);
        btnUpdate = new Button({500, 460}, {300, 50}, "Cap Nhat Thong Tin", font, 1, Theme::Secondary);
    }
    ~ScreenLibrarianCard() { delete btnUpdate; }
    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { if (e.type == sf::Event::MouseButtonPressed) { if (btnClose.handleClick(mousePos)) closed = true; if (btnUpdate->handleClick(mousePos)) { updateRequested = true; closed = true; } } }
    void render(sf::RenderWindow &window) { window.draw(panel); window.draw(title); window.draw(lbMaID); window.draw(lbHoTen); window.draw(lbEmail); window.draw(lbSDT); window.draw(lbChucVu); btnClose.draw(window); btnUpdate->draw(window); }
    bool wasClosed() const { return closed; } bool isUpdateRequested() { return updateRequested; }
};
#endif