#ifndef ADD_BOOK_SCREEN_HPP
#define ADD_BOOK_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "LibrarySystem.h"
#include "Button.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class AddBookScreen {
private:
    sf::Font &font; 
    LibrarySystem *L; 
    RoundedRectangleShape bg; 
    InputField* inputField; 
    sf::Text title, label, message; 
    Button btnConfirm, btnExit; 
    bool closed;
public:
    AddBookScreen(sf::Font &f, LibrarySystem *lib) : font(f), L(lib),
          btnConfirm({450, 450}, {180, 50}, "Xac Nhan", f, 0, Theme::Success),
          btnExit({670, 450}, {180, 50}, "Thoat", f, 0, sf::Color(150, 150, 150)) {
        closed = false; 

        bg.setSize({800, 400}); 
        bg.setCornerRadius(15.0f); 
        bg.setFillColor(sf::Color::White); 
        bg.setOutlineThickness(2); 
        bg.setOutlineColor(Theme::Primary); 
        bg.setPosition(250, 160);

        title.setFont(font); 
        title.setString("THEM SACH TU FILE"); 
        title.setCharacterSize(32); 
        title.setFillColor(Theme::Primary); 
        title.setPosition(480, 190);
        label.setFont(font); 

        label.setString("Nhap ten file (vd: Books.txt):"); 
        label.setCharacterSize(20); 
        label.setFillColor(Theme::TextDark); 
        label.setPosition(350, 260);

        inputField = new InputField({350, 300}, {600, 50}, "Ten file...", font); 

        message.setFont(font); 
        message.setCharacterSize(18); 
        message.setPosition(350, 370);
    }
    ~AddBookScreen() { delete inputField; }

    void reset() { 
        closed = false; 
        inputField->clear(); 
        message.setString(""); 
    } 
    bool isClosed() const { return closed; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
        inputField->handleEvent(e, mouse);
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnConfirm.handleClick(mouse)) {
                std::string file = inputField->getText();
                if (!file.empty()) { 
                    L->DocFileSach(file); 
                    L->GhiFileHeThong("DanhSachSach.txt"); 
                    message.setString("Da them thanh cong!"); 
                    message.setFillColor(Theme::Success); } 
                else {
                    message.setString("Vui long nhap ten file!"); 
                    message.setFillColor(Theme::Error); }
            } if (btnExit.handleClick(mouse)) closed = true;
        }
    }
    void render(sf::RenderWindow &window) {
        window.draw(bg); 
        window.draw(title); 
        window.draw(label); 
        inputField->draw(window); 
        window.draw(message); 
        btnConfirm.draw(window); 
        btnExit.draw(window); }
};
#endif