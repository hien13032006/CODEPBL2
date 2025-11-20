#ifndef LOGIN_READER_SCREEN_HPP
#define LOGIN_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
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

    // TEXT BÁO LỖI
    sf::Text errUsername;
    sf::Text errPassword;
    sf::Text errLogin;

    // ANIMATION RUNG
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

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
        sf::FloatRect b = logoText.getLocalBounds();
        logoText.setOrigin(b.width / 2, b.height / 2);
        logoText.setPosition(440, 400);

        titleText.setFont(font);
        titleText.setString("DANG NHAP DOC GIA");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 220);

        switchText.setFont(font);
        switchText.setString("Chua co tai khoan? Dang ky ngay");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 640);

        usernameField = new InputField({680, 300}, {420, 50}, "Username", font);
        passwordField = new InputField({680, 370}, {420, 50}, "Password", font, true);

        loginButton = new Button({680, 450}, {420, 50}, "Dang Nhap", font);
        backButton = new Button({680, 520}, {420, 45}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        errLogin.setFont(font);
        errLogin.setCharacterSize(14);
        errLogin.setFillColor(sf::Color::White); 
        errLogin.setPosition(680, 430); // dưới ô password

        // KHỞI TẠO TEXT LỖI
        auto initErr = [&](sf::Text& t, float x, float y) {
            t.setFont(font);
            t.setCharacterSize(14);
            t.setFillColor(sf::Color(255, 80, 80));
            t.setPosition(x, y);
        };

        initErr(errUsername, 680, 355);
        initErr(errPassword, 680, 425);
    }

    ~LoginReaderScreen() {
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

        // RUNG PANEL
        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 40) * 8;  // độ rung

            formPanel.setPosition(630 + offset, 150);

            if (shakeTime >= shakeDuration) {
                shaking = false;
                formPanel.setPosition(630, 150);
            }
        }
    }

    // ============ EVENT =============
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

    // ============ RENDER =============
    void render(sf::RenderWindow& window) {

        //BỎ ĐI if(modal->isShown()) – luôn render UI đăng nhập
        window.draw(posterPanel);
        window.draw(formPanel);
        window.draw(logoText);
        window.draw(titleText);

        usernameField->draw(window);
        passwordField->draw(window);

        window.draw(errUsername);
        window.draw(errPassword);

        loginButton->draw(window);
        backButton->draw(window);

        window.draw(switchText);
        window.draw(errLogin);
    }

    // GET/RESET
    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }

    void clearFields() {
        usernameField->clear();
        passwordField->clear();
        errUsername.setString("");
        errPassword.setString("");
    }

    void setLoginError(const std::string& msg) {
        errLogin.setString(msg);
    }
};

#endif
