#ifndef UPDATE_READER_SCREEN_HPP
#define UPDATE_READER_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "InputField.hpp"
#include "Theme.hpp"

class UpdateReaderScreen {
private:
    sf::Font &font; Reader *reader; bool closed; RoundedRectangleShape bg; sf::Text title, msg;
    InputField *inName, *inPhone, *inEmail, *inUser, *inPass; Button *btnSave, *btnCancel;
public:
    UpdateReaderScreen(sf::Font &f, Reader *r) : font(f), reader(r) {
        closed = false; bg.setSize({600, 650}); bg.setCornerRadius(15.0f); bg.setFillColor(sf::Color::White); bg.setOutlineThickness(2); bg.setOutlineColor(Theme::Primary); bg.setPosition(350, 35);
        title.setFont(font); title.setString("CAP NHAT THONG TIN"); title.setCharacterSize(32); title.setFillColor(Theme::Primary); title.setPosition(470, 65);
        float startY = 130; float gap = 70;
        inName = new InputField({400, startY}, {500, 50}, "Ho ten", font); inName->setText(r->getHoTen());
        inPhone = new InputField({400, startY+gap}, {500, 50}, "So dien thoai", font); inPhone->setText(r->getSDT());
        inEmail = new InputField({400, startY+gap*2}, {500, 50}, "Email", font); inEmail->setText(r->getEmail());
        inUser = new InputField({400, startY+gap*3}, {500, 50}, "Username", font); inUser->setText(r->getUsername());
        inPass = new InputField({400, startY+gap*4}, {500, 50}, "Password", font, true); inPass->setText(r->getPassword());
        btnSave = new Button({400, 580}, {200, 55}, "Luu", font, 0, Theme::Success); btnCancel = new Button({700, 580}, {200, 55}, "Huy", font, 0, sf::Color(150, 150, 150));
        msg.setFont(font); msg.setCharacterSize(18); msg.setPosition(400, 540);
    }
    ~UpdateReaderScreen() { delete inName; delete inPhone; delete inEmail; delete inUser; delete inPass; delete btnSave; delete btnCancel; }
    bool isClosed() const { return closed; }
    void handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
        inName->handleEvent(e, mouse); inPhone->handleEvent(e, mouse); inEmail->handleEvent(e, mouse); inUser->handleEvent(e, mouse); inPass->handleEvent(e, mouse);
        if (e.type == sf::Event::MouseButtonPressed) { if (btnSave->handleClick(mouse)) { reader->setHoTen(inName->getText()); reader->setSDT(inPhone->getText()); reader->setEmail(inEmail->getText()); reader->setUsername(inUser->getText()); reader->setPassword(inPass->getText()); msg.setString("Cap nhat thanh cong!"); msg.setFillColor(Theme::Success); closed = true; } if (btnCancel->handleClick(mouse)) closed = true; }
    }
    void update(sf::Vector2f mouse) { inName->update(); inPhone->update(); inEmail->update(); inUser->update(); inPass->update(); btnSave->update(mouse); btnCancel->update(mouse); }
    void render(sf::RenderWindow &window) { window.draw(bg); window.draw(title); inName->draw(window); inPhone->draw(window); inEmail->draw(window); inUser->draw(window); inPass->draw(window); btnSave->draw(window); btnCancel->draw(window); window.draw(msg); }
};
#endif