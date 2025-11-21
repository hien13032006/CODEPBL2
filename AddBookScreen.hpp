#ifndef ADD_BOOK_SCREEN_HPP
#define ADD_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "LibrarySystem.h"
#include "Button.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"

class AddBookScreen {
private:
    sf::Font &font; LibrarySystem *L;
    RoundedRectangleShape bg; InputField* inputField;
    sf::Text title, label, message; Button btnConfirm, btnExit;
    bool closed;

public:
    AddBookScreen(sf::Font &f, LibrarySystem *lib) : font(f), L(lib),
          btnConfirm({500, 480}, {180, 50}, "Xac Nhan", f, 0, sf::Color(76, 175, 80)),
          btnExit({700, 480}, {180, 50}, "Thoat", f, 0, sf::Color(180, 80, 80))
    {
        closed = false;
        bg.setSize({800, 400}); bg.setCornerRadius(15.0f); bg.setFillColor(sf::Color(245, 245, 250));
        bg.setOutlineThickness(2); bg.setOutlineColor(sf::Color(180, 180, 200)); bg.setPosition(300, 200);

        title.setFont(font); title.setString("THEM SACH TU FILE");
        title.setCharacterSize(32); title.setFillColor(sf::Color(30, 30, 30)); title.setPosition(530, 230);

        label.setFont(font); label.setString("Nhap ten file (vd: Books.txt):");
        label.setCharacterSize(20); label.setFillColor(sf::Color(60, 60, 60)); label.setPosition(400, 300);

        inputField = new InputField({400, 340}, {600, 50}, "Ten file...", font);
        message.setFont(font); message.setCharacterSize(18); message.setPosition(400, 410);
    }
    ~AddBookScreen() { delete inputField; }

    void reset() { closed = false; inputField->clear(); message.setString(""); }
    bool isClosed() const { return closed; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
        inputField->handleEvent(e, mouse);
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnConfirm.handleClick(mouse)) {
                std::string file = inputField->getText();
                if (!file.empty()) {
                    L->DocFileSach(file); L->GhiFileHeThong("DanhSachSach.txt");
                    message.setString("Da them thanh cong!"); message.setFillColor(sf::Color(0, 150, 0));
                } else { message.setString("Vui long nhap ten file!"); message.setFillColor(sf::Color(200, 0, 0)); }
            }
            if (btnExit.handleClick(mouse)) closed = true;
        }
    }

    void render(sf::RenderWindow &window) {
        window.draw(bg); window.draw(title); window.draw(label);
        inputField->draw(window); window.draw(message);
        btnConfirm.draw(window); btnExit.draw(window);
    }
};

#endif