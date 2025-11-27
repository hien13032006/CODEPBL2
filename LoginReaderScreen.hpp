#ifndef LOGIN_READER_SCREEN_HPP
#define LOGIN_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class LoginReaderScreen {
private:
    Modal* modal; RoundedRectangleShape posterPanel, formPanel; sf::Text logoText, titleText, switchText, errLogin;
    InputField *usernameField, *passwordField; Button *loginButton, *backButton; bool shaking = false; float shakeTime = 0.0f; const float shakeDuration = 0.4f;
    
public:
    LoginReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(400, 600)); posterPanel.setPosition(150, 60); posterPanel.setFillColor(Theme::Primary); 
        formPanel.setSize(sf::Vector2f(600, 600)); formPanel.setPosition(550, 60); formPanel.setFillColor(sf::Color::White); 
        
        logoText.setFont(font); logoText.setString("LIBRARY\nSYSTEM"); logoText.setCharacterSize(48); logoText.setStyle(sf::Text::Bold); logoText.setFillColor(sf::Color::White); logoText.setPosition(230, 260);
        titleText.setFont(font); titleText.setString("DANG NHAP DOC GIA"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(630, 130);
        
        usernameField = new InputField({630, 210}, {440, 55}, "Username", font); 
        passwordField = new InputField({630, 290}, {440, 55}, "Password", font, true);
        
        errLogin.setFont(font); errLogin.setCharacterSize(16); errLogin.setFillColor(Theme::Error); 
        errLogin.setString(""); // Mặc định rỗng
        
        loginButton = new Button({630, 390}, {440, 60}, "Dang Nhap", font, 0, Theme::Primary); 
        backButton = new Button({630, 470}, {440, 50}, "Quay Lai", font, 0, sf::Color(150, 150, 150));
        switchText.setFont(font); switchText.setString("Chua co tai khoan? Dang ky ngay"); switchText.setCharacterSize(16); switchText.setFillColor(Theme::Secondary); switchText.setPosition(630, 560);
    }

    ~LoginReaderScreen() { delete usernameField; delete passwordField; delete loginButton; delete backButton; }

    void startShake() { shaking = true; shakeTime = 0; }

    void setLoginError(const std::string& msg) {
        errLogin.setString(msg);
        // Căn giữa lỗi trong phần Form (X start = 550, Width = 600 -> Center = 850)
        sf::FloatRect b = errLogin.getLocalBounds();
        errLogin.setOrigin(b.width / 2, 0);
        errLogin.setPosition(850, 360); 
        startShake();
    }

    bool validate() {
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
            // Rung cả 2 panel
            posterPanel.setPosition(150 + offset, 60);
            formPanel.setPosition(550 + offset, 60);
            if (shakeTime >= shakeDuration) { 
                shaking = false; 
                posterPanel.setPosition(150, 60); 
                formPanel.setPosition(550, 60);
            }
        }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { usernameField->handleEvent(event, mousePos); passwordField->handleEvent(event, mousePos); }
    
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
        window.draw(errLogin); // Vẽ thông báo lỗi
        loginButton->draw(window); backButton->draw(window);
        window.draw(switchText);
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() { usernameField->clear(); passwordField->clear(); errLogin.setString(""); }
};
#endif