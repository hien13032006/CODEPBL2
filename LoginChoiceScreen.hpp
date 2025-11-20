// ============================================
// screens/LoginChoiceScreen.hpp - Chọn loại đăng nhập
// ============================================
#ifndef LOGIN_CHOICE_SCREEN_HPP
#define LOGIN_CHOICE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"

class LoginChoiceScreen {
private:
    Modal* modal;
    sf::Text titleText;
    sf::Text descText;
    Button* librarianButton;
    Button* readerButton;

public:
    LoginChoiceScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        titleText.setFont(font);
        titleText.setString("Chon Loai Nguoi Dung");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 300);

        descText.setFont(font);
        descText.setString("Ban muon dang nhap voi tu cach nao?");
        descText.setCharacterSize(16);
        descText.setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect descBounds = descText.getLocalBounds();
        descText.setOrigin(descBounds.width / 2, descBounds.height / 2);
        descText.setPosition(700, 350);

        librarianButton = new Button(sf::Vector2f(450, 420), sf::Vector2f(220, 60),
                                     "Thu Thu", font, 0, sf::Color(100, 150, 255));
        
        readerButton = new Button(sf::Vector2f(730, 420), sf::Vector2f(220, 60),
                                  "Doc Gia", font, 0, sf::Color(255, 150, 100));
    }

    ~LoginChoiceScreen() {
        delete librarianButton;
        delete readerButton;
    }

    void update(sf::Vector2f mousePos) {
        librarianButton->update(mousePos);
        readerButton->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (librarianButton->handleClick(mousePos)) return 1;
        if (readerButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(titleText);
            window.draw(descText);
            librarianButton->draw(window);
            readerButton->draw(window);
        }
    }
};

#endif