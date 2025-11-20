#ifndef LOGIN_LIBRARIAN_SCREEN_HPP
#define LOGIN_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class LoginLibrarianScreen {
private:
    Modal* modal;
    sf::Text titleText;

    sf::RectangleShape formPanel;

    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

    // TEXT BÁO LỖI
    sf::Text errUsername;
    sf::Text errPassword;
    sf::Text errLogin;

    // ANIMATION RUNG
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    LoginLibrarianScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {

        titleText.setFont(font);
        titleText.setString("DANG NHAP THU THU");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 280);

        usernameField = new InputField({500, 350}, {400, 50}, "Username", font);
        passwordField = new InputField({500, 420}, {400, 50}, "Password", font, true);

        loginButton = new Button({500, 500}, {400, 50}, "Dang Nhap", font);
        backButton = new Button({500, 570}, {400, 45}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        // KHỞI TẠO TEXT LỖI
        auto initErr = [&](sf::Text& t, float x, float y) {
            t.setFont(font);
            t.setCharacterSize(14);
            t.setFillColor(sf::Color(255, 80, 80));
            t.setPosition(x, y);
        };

        initErr(errUsername, 500, 405);
        initErr(errPassword, 500, 475);

        errLogin.setFont(font);
        errLogin.setCharacterSize(14);
        errLogin.setFillColor(sf::Color::White);
        errLogin.setPosition(500, 475); // dưới nút login
    }

    ~LoginLibrarianScreen() {
        delete usernameField;
        delete passwordField;
        delete loginButton;
        delete backButton;
    }
    

    // ============ ANIMATION RUNG =============
    void startShake() {
        shaking = true;
        shakeTime = 0;
    }

    // ============ VALIDATE =============
    bool validate() {
        bool ok = true;
        errUsername.setString("");
        errPassword.setString("");
        errLogin.setString("");

        if (getUsername().empty()) {
            errUsername.setString("Vui long nhap username");
            ok = false;
        }
        if (getPassword().empty()) {
            errPassword.setString("Vui long nhap mat khau");
            ok = false;
        }

        if (!ok) startShake();
        return ok;
    }

    // ============ UPDATE =============
    void update(sf::Vector2f mousePos) {
        usernameField->update();
        passwordField->update();
        loginButton->update(mousePos);
        backButton->update(mousePos);

        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 40) * 8; 
            formPanel.setPosition(630 + offset, 150); // di chuyển panel
            if (shakeTime >= shakeDuration) {
                shaking = false;
                formPanel.setPosition(630, 150);
            }
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
            window.draw(titleText);
            usernameField->draw(window);
            passwordField->draw(window);

            window.draw(errUsername);
            window.draw(errPassword);
            window.draw(errLogin);

            loginButton->draw(window);
            backButton->draw(window);
        }
    }

    // GET/RESET
    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }

    void clearFields() {
        usernameField->clear();
        passwordField->clear();
        errUsername.setString("");
        errPassword.setString("");
        errLogin.setString("");
    }

    void setLoginError(const std::string& msg) {
        errLogin.setString(msg);
        startShake();
    }
};

#endif
