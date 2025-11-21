#ifndef LOGIN_READER_SCREEN_HPP
#define LOGIN_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

class LoginReaderScreen {
private:
    Modal* modal;
    RoundedRectangleShape posterPanel; // Trái
    RoundedRectangleShape formPanel;   // Phải
    sf::Text logoText, titleText, switchText;
    
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;
    sf::Text errLogin;

    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    LoginReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Poster Panel (400x600)
        posterPanel.setSize(sf::Vector2f(400, 600));
        posterPanel.setPosition(200, 150);
        posterPanel.setFillColor(sf::Color(60, 100, 180)); // Xanh dương
        // posterPanel.setCornerRadius(20.0f); // Tạm thời không bo góc phải để ghép khít

        // Form Panel (600x600)
        formPanel.setSize(sf::Vector2f(600, 600));
        formPanel.setPosition(600, 150);
        formPanel.setFillColor(sf::Color(40, 45, 60)); // Nền tối

        logoText.setFont(font);
        logoText.setString("LIBRARY\nSYSTEM");
        logoText.setCharacterSize(48);
        logoText.setStyle(sf::Text::Bold);
        logoText.setFillColor(sf::Color::White);
        logoText.setPosition(280, 350);

        titleText.setFont(font);
        titleText.setString("DANG NHAP DOC GIA");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(680, 220);

        switchText.setFont(font);
        switchText.setString("Chua co tai khoan? Dang ky ngay");
        switchText.setCharacterSize(16);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(680, 650);

        usernameField = new InputField({680, 300}, {440, 55}, "Username", font);
        passwordField = new InputField({680, 380}, {440, 55}, "Password", font, true);

        loginButton = new Button({680, 480}, {440, 60}, "Dang Nhap", font, 0, sf::Color(255, 150, 100));
        loginButton->getText().setCharacterSize(22);

        backButton = new Button({680, 560}, {440, 50}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        errLogin.setFont(font);
        errLogin.setCharacterSize(16);
        errLogin.setFillColor(sf::Color(255, 80, 80));
        errLogin.setPosition(680, 445);
    }

    ~LoginReaderScreen() {
        delete usernameField; delete passwordField; delete loginButton; delete backButton;
    }

    void startShake() { shaking = true; shakeTime = 0; }

    bool validate() {
        errLogin.setString("");
        if (getUsername().empty() || getPassword().empty()) {
            setLoginError("Vui long nhap day du thong tin!");
            return false;
        }
        return true;
    }

    void update(sf::Vector2f mousePos) {
        usernameField->update(); passwordField->update();
        loginButton->update(mousePos); backButton->update(mousePos);

        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 50) * 10; 
            posterPanel.setPosition(200 + offset, 150);
            formPanel.setPosition(600 + offset, 150);
            if (shakeTime >= shakeDuration) { 
                shaking = false; 
                posterPanel.setPosition(200, 150); 
                formPanel.setPosition(600, 150);
            }
        }
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
        window.draw(posterPanel); window.draw(formPanel);
        window.draw(logoText); window.draw(titleText);
        usernameField->draw(window); passwordField->draw(window);
        window.draw(errLogin);
        loginButton->draw(window); backButton->draw(window);
        window.draw(switchText);
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() { usernameField->clear(); passwordField->clear(); errLogin.setString(""); }
    void setLoginError(const std::string& msg) { errLogin.setString(msg); startShake(); }
};

#endif