// ============================================
// screens/RegisterReaderScreen.hpp - Đăng ký độc giả
// ============================================
#ifndef REGISTER_READER_SCREEN_HPP
#define REGISTER_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class RegisterReaderScreen {
private:
    Modal* modal;
    sf::RectangleShape posterPanel;
    sf::RectangleShape formPanel;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text switchText;
    InputField* nameField;
    InputField* phoneField;
    InputField* emailField;
    InputField* usernameField;
    InputField* passwordField;
    InputField* confirmPasswordField;
    Button* registerButton;
    Button* backButton;

public:
    RegisterReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
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
        titleText.setString("Dang Ky Doc Gia");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 180);

        switchText.setFont(font);
        switchText.setString("Da co tai khoan? Dang nhap");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 700);

        nameField = new InputField(sf::Vector2f(680, 230), sf::Vector2f(420, 45),
                                   "Ho ten", font);
        
        phoneField = new InputField(sf::Vector2f(680, 290), sf::Vector2f(420, 45),
                                    "So dien thoai", font);
        
        emailField = new InputField(sf::Vector2f(680, 350), sf::Vector2f(420, 45),
                                    "Email", font);
        
        usernameField = new InputField(sf::Vector2f(680, 410), sf::Vector2f(420, 45),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(680, 470), sf::Vector2f(420, 45),
                                       "Password", font, true);
        
        confirmPasswordField = new InputField(sf::Vector2f(680, 530), sf::Vector2f(420, 45),
                                             "Xac nhan password", font, true);

        registerButton = new Button(sf::Vector2f(680, 595), sf::Vector2f(420, 45),
                                   "Dang Ky", font);
        
        backButton = new Button(sf::Vector2f(680, 655), sf::Vector2f(420, 40),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~RegisterReaderScreen() {
        delete nameField;
        delete phoneField;
        delete emailField;
        delete usernameField;
        delete passwordField;
        delete confirmPasswordField;
        delete registerButton;
        delete backButton;
    }

    void update(sf::Vector2f mousePos) {
        nameField->update();
        phoneField->update();
        emailField->update();
        usernameField->update();
        passwordField->update();
        confirmPasswordField->update();
        registerButton->update(mousePos);
        backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        nameField->handleEvent(event, mousePos);
        phoneField->handleEvent(event, mousePos);
        emailField->handleEvent(event, mousePos);
        usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos);
        confirmPasswordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (registerButton->handleClick(mousePos)) return 1;
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
            nameField->draw(window);
            phoneField->draw(window);
            emailField->draw(window);
            usernameField->draw(window);
            passwordField->draw(window);
            confirmPasswordField->draw(window);
            registerButton->draw(window);
            backButton->draw(window);
            window.draw(switchText);
        }
    }

    std::string getName() const { return nameField->getText(); }
    std::string getPhone() const { return phoneField->getText(); }
    std::string getEmail() const { return emailField->getText(); }
    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    std::string getConfirmPassword() const { return confirmPasswordField->getText(); }
    
    void clearFields() {
        nameField->clear();
        phoneField->clear();
        emailField->clear();
        usernameField->clear();
        passwordField->clear();
        confirmPasswordField->clear();
    }
};

#endif