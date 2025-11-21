#ifndef LOGIN_LIBRARIAN_SCREEN_HPP
#define LOGIN_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

class LoginLibrarianScreen {
private:
    Modal* modal;
    RoundedRectangleShape panel;
    sf::Text titleText;
    
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

    sf::Text errLogin;
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    LoginLibrarianScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Panel (500x600)
        panel.setSize({500, 600});
        panel.setCornerRadius(20.0f);
        panel.setFillColor(sf::Color(40, 45, 60)); // Nền tối
        panel.setPosition(450, 150);

        titleText.setFont(font);
        titleText.setString("DANG NHAP THU THU");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect b = titleText.getLocalBounds();
        titleText.setOrigin(b.width/2, 0);
        titleText.setPosition(700, 200); // Giữa panel (450 + 250)

        usernameField = new InputField({500, 300}, {400, 55}, "Username", font);
        passwordField = new InputField({500, 380}, {400, 55}, "Password", font, true);

        loginButton = new Button({500, 480}, {400, 60}, "Dang Nhap", font, 0, sf::Color(76, 175, 80));
        loginButton->getText().setCharacterSize(22);

        backButton = new Button({500, 560}, {400, 50}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        errLogin.setFont(font);
        errLogin.setCharacterSize(16);
        errLogin.setFillColor(sf::Color(255, 80, 80));
        errLogin.setPosition(500, 445);
    }

    ~LoginLibrarianScreen() {
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
            panel.setPosition(450 + offset, 150);
            if (shakeTime >= shakeDuration) { shaking = false; panel.setPosition(450, 150); }
        }
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
            window.draw(panel); window.draw(titleText);
            usernameField->draw(window); passwordField->draw(window);
            window.draw(errLogin);
            loginButton->draw(window); backButton->draw(window);
        }
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() { usernameField->clear(); passwordField->clear(); errLogin.setString(""); }
    void setLoginError(const std::string& msg) { errLogin.setString(msg); startShake(); }
};

#endif