#ifndef ADD_BOOK_SCREEN_HPP
#define ADD_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "LibrarySystem.h"
#include "Button.hpp"

class AddBookScreen {
private:
    sf::Font &font;
    LibrarySystem *L;

    // UI
    sf::RectangleShape bg;

    sf::RectangleShape inputBox;
    sf::Text inputText;
    bool typing;

    sf::Text title;
    sf::Text label;

    Button btnConfirm;
    Button btnExit;

    sf::Text message;

    bool closed;

public:
    AddBookScreen(sf::Font &f, LibrarySystem *lib);

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse);
    void render(sf::RenderWindow &window);

    bool isClosed() const { return closed; }
};

AddBookScreen::AddBookScreen(sf::Font &f, LibrarySystem *lib)
    : font(f), L(lib),
      btnConfirm({500, 430}, {150, 45}, "Xac nhan", f),
      btnExit({700, 430}, {150, 45}, "Thoat", f)
{
    closed = false;
    typing = false;

    // Background
    bg.setSize({800, 500});
    bg.setFillColor(sf::Color(245, 245, 245));
    bg.setOutlineThickness(2);
    bg.setOutlineColor(sf::Color(120, 120, 120));
    bg.setPosition(310, 150);

    // Title
    title.setFont(font);
    title.setString("THEM SACH");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::Black);
    title.setPosition(550, 180);

    // Label
    label.setFont(font);
    label.setString("Nhap ten file(vd:Input.txt):");
    label.setCharacterSize(22);
    label.setFillColor(sf::Color::Black);
    label.setPosition(440, 260);

    // Input box
    inputBox.setSize({450, 45});
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color(140, 140, 140));
    inputBox.setPosition(440, 295);

    inputText.setFont(font);
    inputText.setCharacterSize(22);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(445, 300);

    // Message
    message.setFont(font);
    message.setCharacterSize(20);
    message.setFillColor(sf::Color::Green);
    message.setPosition(440, 360);
    message.setString("");
}

void AddBookScreen::handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
    // Nhấp vào input box
    if (e.type == sf::Event::MouseButtonPressed) {
        if (inputBox.getGlobalBounds().contains(mouse)) {
            typing = true;
            inputBox.setOutlineColor(sf::Color(70, 130, 200));
        } else {
            typing = false;
            inputBox.setOutlineColor(sf::Color(140, 140, 140));
        }

        // Nút xác nhận
        if (btnConfirm.handleClick(mouse)) {
            std::string file = inputText.getString();

            if (!file.empty()) {
                L->DocFileSach(file); // chỉ gọi, không gán
                L->GhiFileSach("DanhSachSach.txt");
                message.setString("Them sach thanh cong!");
                message.setFillColor(sf::Color::Green);

                
            } else {
                message.setString("Vui long nhap ten file!");
                message.setFillColor(sf::Color::Red);
            }
        }

        // Nút thoát
        if (btnExit.handleClick(mouse)) {
            closed = true;
        }
    }

    // Nhập chữ
    if (typing && e.type == sf::Event::TextEntered) {
        if (e.text.unicode == 8) {
            // BACKSPACE
            std::string s = inputText.getString();
            if (!s.empty()) {
                s.pop_back();
                inputText.setString(s);
            }
        }
        else if (e.text.unicode >= 32 && e.text.unicode < 128) {
            // Ký tự bình thường
            std::string s = inputText.getString();
            s.push_back((char)e.text.unicode);
            inputText.setString(s);
        }
    }
}

void AddBookScreen::render(sf::RenderWindow &window) {
    window.draw(bg);
    window.draw(title);
    window.draw(label);

    window.draw(inputBox);
    window.draw(inputText);

    btnConfirm.draw(window);
    btnExit.draw(window);

    window.draw(message);
}


#endif
