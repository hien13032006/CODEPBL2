#ifndef SCREEN_READER_CARD_H
#define SCREEN_READER_CARD_H
#include <SFML/Graphics.hpp>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class ScreenReaderCard {
private:
    sf::Font &font; Reader *reader; sf::Text title, lbMaID, lbHoTen, lbEmail, lbSDT, lbNgayDK; bool closed; RoundedRectangleShape panel; Button btnClose; Button* btnUpdate; bool updateRequested;
public:
    ScreenReaderCard(sf::Font &f, Reader *r) : font(f), reader(r), btnClose({910, 135}, {40, 40}, "X", f, 0, Theme::Error) {
        closed = false; updateRequested = false;
        panel.setSize({600, 450}); panel.setCornerRadius(20.0f); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(350, 135);
        title.setFont(font); title.setString("THE BAN DOC"); title.setCharacterSize(36); title.setFillColor(Theme::TextDark); title.setPosition(520, 160);
        auto setTxt = [&](sf::Text& t, std::string s, float y) { t.setFont(font); t.setString(s); t.setCharacterSize(22); t.setFillColor(Theme::TextLight); t.setPosition(400, y); };
        char dateBuf[20]; time_t t = reader->getNgayDangKy(); strftime(dateBuf, 20, "%d/%m/%Y", localtime(&t));
        setTxt(lbMaID, "ID: " + reader->getMaID(), 230); setTxt(lbHoTen, "Ten: " + reader->getHoTen(), 270); setTxt(lbEmail, "Email: " + reader->getEmail(), 310); setTxt(lbSDT, "SDT: " + reader->getSDT(), 350); setTxt(lbNgayDK, "Ngay DK: " + std::string(dateBuf), 390);
        btnUpdate = new Button({500, 460}, {300, 50}, "Cap Nhat Thong Tin", font, 1, Theme::Secondary);
    }
    ~ScreenReaderCard() { delete btnUpdate; }
    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { if (e.type == sf::Event::MouseButtonPressed) { if (btnClose.handleClick(mousePos)) closed = true; if (btnUpdate->handleClick(mousePos)) updateRequested = true; } }
    void render(sf::RenderWindow &window) { window.draw(panel); window.draw(title); window.draw(lbMaID); window.draw(lbHoTen); window.draw(lbEmail); window.draw(lbSDT); window.draw(lbNgayDK); btnClose.draw(window); btnUpdate->draw(window); }
    bool wasClosed() const { return closed; } bool isUpdateRequested() { if (updateRequested) { updateRequested = false; return true; } return false; }
};
#endif