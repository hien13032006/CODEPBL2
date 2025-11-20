#ifndef REGISTER_READER_SCREEN_HPP
#define REGISTER_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

// Các hàm kiểm tra bên ngoài (bạn đã có sẵn)
extern bool kiemTraSDT(const string&);
extern bool kiemTraEmail(const string&);
extern bool kiemTraMatKhau(const string&);

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

    // ====== TEXT HIỂN THỊ LỖI ======
    sf::Text errName;
    sf::Text errPhone;
    sf::Text errEmail;
    sf::Text errUsername;
    sf::Text errPassword;
    sf::Text errConfirm;

public:
    RegisterReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(380, 600));
        posterPanel.setPosition(250, 150);
        posterPanel.setFillColor(sf::Color(40, 50, 70, 200));

        formPanel.setSize(sf::Vector2f(520, 600));
        formPanel.setPosition(630, 150);
        formPanel.setFillColor(sf::Color(30, 35, 50));

        // Logo
        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(40);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(440, 400);

        // Title
        titleText.setFont(font);
        titleText.setString("Dang Ky Doc Gia");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 180);

        // Switch text
        switchText.setFont(font);
        switchText.setString("Da co tai khoan? Dang nhap");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 700);

        // Fields
        nameField = new InputField({680, 230}, {420, 45}, "Ho ten", font);
        phoneField = new InputField({680, 290}, {420, 45}, "So dien thoai", font);
        emailField = new InputField({680, 350}, {420, 45}, "Email", font);
        usernameField = new InputField({680, 410}, {420, 45}, "Username", font);
        passwordField = new InputField({680, 470}, {420, 45}, "Password", font, true);
        confirmPasswordField = new InputField({680, 530}, {420, 45}, "Xac nhan password", font, true);

        registerButton = new Button({680, 595}, {420, 45}, "Dang Ky", font);
        backButton = new Button({680, 655}, {420, 40}, "Quay Lai", font, 0, sf::Color(100, 100, 120));

        // ====== KHU VỰC HIỂN THỊ LỖI ======
        auto initErr = [&](sf::Text& e, float x, float y) {
            e.setFont(font);
            e.setCharacterSize(14);
            e.setFillColor(sf::Color(255, 80, 80));
            e.setPosition(x, y);
            e.setString("");
        };

        initErr(errName,     680, 275);
        initErr(errPhone,    680, 335);
        initErr(errEmail,    680, 395);
        initErr(errUsername, 680, 455);
        initErr(errPassword, 680, 515);
        initErr(errConfirm,  680, 575);
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

            // Draw errors
            window.draw(errName);
            window.draw(errPhone);
            window.draw(errEmail);
            window.draw(errUsername);
            window.draw(errPassword);
            window.draw(errConfirm);

            registerButton->draw(window);
            backButton->draw(window);
            window.draw(switchText);
        }
    }

    // ===================================
    //   GETTER
    // ===================================
    string getName() const { return nameField->getText(); }
    string getPhone() const { return phoneField->getText(); }
    string getEmail() const { return emailField->getText(); }
    string getUsername() const { return usernameField->getText(); }
    string getPassword() const { return passwordField->getText(); }
    string getConfirmPassword() const { return confirmPasswordField->getText(); }

    // ===================================
    //   CLEAR FIELDS
    // ===================================
    void clearFields() {
        nameField->clear();
        phoneField->clear();
        emailField->clear();
        usernameField->clear();
        passwordField->clear();
        confirmPasswordField->clear();
    }

    // ===================================
    //   VALIDATE FUNCTION
    // ===================================
    bool validate() {
        bool ok = true;

        errName.setString("");
        errPhone.setString("");
        errEmail.setString("");
        errUsername.setString("");
        errPassword.setString("");
        errConfirm.setString("");

        if (getName().empty()) {
            errName.setString("Ho ten khong duoc de trong");
            ok = false;
        }

        if (!kiemTraSDT(getPhone())) {
            errPhone.setString("So dien thoai khong hop le");
            ok = false;
        }

        if (!kiemTraEmail(getEmail())) {
            errEmail.setString("Email khong hop le");
            ok = false;
        }

        if (getUsername().size() < 4) {
            errUsername.setString("Username phai tu 4 ky tu");
            ok = false;
        }

        if (!kiemTraMatKhau(getPassword())) {
            errPassword.setString("Mat khau phai >= 8 ky tu");
            ok = false;
        }

        if (getPassword() != getConfirmPassword()) {
            errConfirm.setString("Mat khau khong khop");
            ok = false;
        }

        return ok;
    }
};

#endif
