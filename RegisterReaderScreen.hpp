#ifndef REGISTER_READER_SCREEN_HPP
#define REGISTER_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

// Helper functions extern
extern bool kiemTraSDT(const string&);
extern bool kiemTraEmail(const string&);
extern bool kiemTraMatKhau(const string&);

class RegisterReaderScreen {
private:
    Modal* modal;
    RoundedRectangleShape posterPanel;
    RoundedRectangleShape formPanel;
    sf::Text logoText, titleText, switchText;

    InputField* nameField; InputField* phoneField; InputField* emailField;
    InputField* usernameField; InputField* passwordField; InputField* confirmPasswordField;
    
    Button* registerButton; Button* backButton;
    
    sf::Text errName, errPhone, errEmail, errUsername, errPassword, errConfirm;

public:
    RegisterReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Poster (300x700)
        posterPanel.setSize(sf::Vector2f(300, 700));
        posterPanel.setPosition(200, 100);
        posterPanel.setFillColor(sf::Color(60, 100, 180));

        // Form (650x700)
        formPanel.setSize(sf::Vector2f(650, 700));
        formPanel.setPosition(500, 100);
        formPanel.setFillColor(sf::Color(40, 45, 60));

        logoText.setFont(font);
        logoText.setString("JOIN US");
        logoText.setCharacterSize(48);
        logoText.setFillColor(sf::Color::White);
        logoText.setPosition(250, 400);

        titleText.setFont(font);
        titleText.setString("DANG KY TAI KHOAN");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(550, 130);

        switchText.setFont(font);
        switchText.setString("Da co tai khoan? Dang nhap ngay");
        switchText.setCharacterSize(16);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(550, 750);

        // Fields (Size 45 height)
        float startY = 180; float gap = 80;
        nameField = new InputField({550, startY}, {550, 45}, "Ho ten", font);
        phoneField = new InputField({550, startY + gap}, {550, 45}, "So dien thoai", font);
        emailField = new InputField({550, startY + gap*2}, {550, 45}, "Email", font);
        usernameField = new InputField({550, startY + gap*3}, {550, 45}, "Username", font);
        passwordField = new InputField({550, startY + gap*4}, {260, 45}, "Password", font, true);
        confirmPasswordField = new InputField({840, startY + gap*4}, {260, 45}, "Confirm", font, true);

        registerButton = new Button({550, 600}, {550, 55}, "Dang Ky", font, 0, sf::Color(76, 175, 80));
        backButton = new Button({550, 670}, {550, 45}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        // Errors
        auto initErr = [&](sf::Text& e, float x, float y) {
            e.setFont(font); e.setCharacterSize(14); e.setFillColor(sf::Color(255, 80, 80)); e.setPosition(x, y); e.setString("");
        };
        initErr(errName, 550, startY + 50); initErr(errPhone, 550, startY + gap + 50);
        initErr(errEmail, 550, startY + gap*2 + 50); initErr(errUsername, 550, startY + gap*3 + 50);
        initErr(errPassword, 550, startY + gap*4 + 50); initErr(errConfirm, 840, startY + gap*4 + 50);
    }

    ~RegisterReaderScreen() { /* Delete all pointers */ }

    void update(sf::Vector2f mousePos) {
        nameField->update(); phoneField->update(); emailField->update(); usernameField->update();
        passwordField->update(); confirmPasswordField->update(); registerButton->update(mousePos); backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        nameField->handleEvent(event, mousePos); phoneField->handleEvent(event, mousePos);
        emailField->handleEvent(event, mousePos); usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos); confirmPasswordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (registerButton->handleClick(mousePos)) return 1;
        if (switchText.getGlobalBounds().contains(mousePos)) return 2;
        if (backButton->handleClick(mousePos)) return 3;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        window.draw(posterPanel); window.draw(formPanel); window.draw(logoText); window.draw(titleText);
        nameField->draw(window); phoneField->draw(window); emailField->draw(window);
        usernameField->draw(window); passwordField->draw(window); confirmPasswordField->draw(window);
        window.draw(errName); window.draw(errPhone); window.draw(errEmail);
        window.draw(errUsername); window.draw(errPassword); window.draw(errConfirm);
        registerButton->draw(window); backButton->draw(window); window.draw(switchText);
    }

    // Getters
    string getName() const { return nameField->getText(); }
    string getPhone() const { return phoneField->getText(); }
    string getEmail() const { return emailField->getText(); }
    string getUsername() const { return usernameField->getText(); }
    string getPassword() const { return passwordField->getText(); }
    string getConfirmPassword() const { return confirmPasswordField->getText(); }

    void clearFields() {
        nameField->clear(); phoneField->clear(); emailField->clear();
        usernameField->clear(); passwordField->clear(); confirmPasswordField->clear();
        errName.setString(""); errPhone.setString(""); errEmail.setString("");
        errUsername.setString(""); errPassword.setString(""); errConfirm.setString("");
    }

    void setUsernameError(const std::string& msg) { errUsername.setString(msg); }

    bool validate() {
        bool ok = true;
        errName.setString(""); errPhone.setString(""); errEmail.setString("");
        errUsername.setString(""); errPassword.setString(""); errConfirm.setString("");

        if (getName().empty()) { errName.setString("Bat buoc"); ok = false; }
        if (!kiemTraSDT(getPhone())) { errPhone.setString("SDT khong hop le"); ok = false; }
        if (!kiemTraEmail(getEmail())) { errEmail.setString("Email khong hop le"); ok = false; }
        if (getUsername().size() < 4) { errUsername.setString("Tu 4 ky tu"); ok = false; }
        if (!kiemTraMatKhau(getPassword())) { errPassword.setString(">= 8 ky tu"); ok = false; }
        if (getPassword() != getConfirmPassword()) { errConfirm.setString("Khong khop"); ok = false; }
        return ok;
    }
};

#endif