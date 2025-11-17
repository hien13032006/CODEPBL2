#ifndef LOGIN_LIBRARIAN_SCREEN_HPP
#define LOGIN_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class LoginLibrarianScreen {
private:
    Modal* modal;
    sf::Text titleText;
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

public:
    LoginLibrarianScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        titleText.setFont(font);
        titleText.setString("Dang Nhap Thu Thu");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 280);

        usernameField = new InputField(sf::Vector2f(500, 350), sf::Vector2f(400, 50),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(500, 420), sf::Vector2f(400, 50),
                                       "Password", font, true);

        loginButton = new Button(sf::Vector2f(500, 500), sf::Vector2f(400, 50),
                                "Dang Nhap", font);
        
        backButton = new Button(sf::Vector2f(500, 570), sf::Vector2f(400, 45),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~LoginLibrarianScreen() {
        delete usernameField;
        delete passwordField;
        delete loginButton;
        delete backButton;
    }

    void update(sf::Vector2f mousePos) {
        usernameField->update();
        passwordField->update();
        loginButton->update(mousePos);
        backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (loginButton->handleClick(mousePos)) return 1;
        if (backButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(titleText);
            usernameField->draw(window);
            passwordField->draw(window);
            loginButton->draw(window);
            backButton->draw(window);
        }
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() {
        usernameField->clear();
        passwordField->clear();
    }
};

#endif