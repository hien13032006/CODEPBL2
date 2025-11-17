#ifndef LOGIN_READER_SCREEN_HPP
#define LOGIN_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class LoginReaderScreen {
private:
    Modal* modal;
    sf::RectangleShape posterPanel;
    sf::RectangleShape formPanel;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text switchText;
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

public:
    LoginReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(380, 600));
        posterPanel.setPosition(250, 150);
        posterPanel.setFillColor(sf::Color(40, 50, 70, 200));

        formPanel.setSize(sf::Vector2f(520, 600));
        formPanel.setPosition(630, 150);
        formPanel.setFillColor(sf::Color(30, 35, 50));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(40);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(440, 400);

        titleText.setFont(font);
        titleText.setString("Dang Nhap Doc Gia");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 220);

        switchText.setFont(font);
        switchText.setString("Chua co tai khoan? Dang ky ngay");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 640);

        usernameField = new InputField(sf::Vector2f(680, 300), sf::Vector2f(420, 50),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(680, 370), sf::Vector2f(420, 50),
                                       "Password", font, true);

        loginButton = new Button(sf::Vector2f(680, 450), sf::Vector2f(420, 50),
                                "Dang Nhap", font);
        
        backButton = new Button(sf::Vector2f(680, 520), sf::Vector2f(420, 45),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~LoginReaderScreen() {
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
        if (switchText.getGlobalBounds().contains(mousePos)) return 2;
        if (backButton->handleClick(mousePos)) return 3;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(posterPanel);
            window.draw(formPanel);
            window.draw(logoText);
            window.draw(titleText);
            usernameField->draw(window);
            passwordField->draw(window);
            loginButton->draw(window);
            backButton->draw(window);
            window.draw(switchText);
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