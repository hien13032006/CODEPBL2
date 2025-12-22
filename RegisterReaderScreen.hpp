#ifndef REGISTER_READER_SCREEN_HPP
#define REGISTER_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class RegisterReaderScreen {
private:
    Modal* modal; 
    RoundedRectangleShape posterPanel, formPanel; 
    sf::Text logoText, titleText, switchText;
    sf::Text errName, errPhone, errEmail, errUsername, errPassword, errConfirm;
    InputField *nameField, *phoneField, *emailField, *usernameField, *passwordField, *confirmPasswordField; 
    Button *registerButton, *backButton;
    bool checkSDT(const std::string& s) { 
        return s.length() >= 10 && s.find_first_not_of("0123456789") == std::string::npos; 
    }
    bool checkEmail(const std::string& s) {
        return s.find("@") != std::string::npos && s.find(".com") != std::string::npos; 
    }
    bool checkPass(const std::string& s) { return s.length() >= 6; }

public:
    RegisterReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(300, 700)); 
        posterPanel.setPosition(175, 10); 
        posterPanel.setFillColor(Theme::Primary);
        formPanel.setSize(sf::Vector2f(650, 700)); 
        formPanel.setPosition(475, 10); 
        formPanel.setFillColor(sf::Color::White);
        
        logoText.setFont(font); 
        logoText.setString("JOIN US");
        logoText.setCharacterSize(48); 
        logoText.setFillColor(sf::Color::White);
        logoText.setPosition(225, 350);
        titleText.setFont(font);
        titleText.setString("DANG KY TAI KHOAN"); 
        titleText.setCharacterSize(28);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(525, 30);
        switchText.setFont(font);
        switchText.setString("Da co tai khoan? Dang nhap ngay");
        switchText.setCharacterSize(16); 
        switchText.setFillColor(Theme::Secondary); 
        switchText.setPosition(525, 650);

        float startY = 90; 
        float gap = 80;
        nameField = new InputField({525, startY}, {550, 45}, "Ho ten", font); 
        phoneField = new InputField({525, startY+gap}, {550, 45}, "So dien thoai", font);
        emailField = new InputField({525, startY+gap*2}, {550, 45}, "Email", font); 
        usernameField = new InputField({525, startY+gap*3}, {550, 45}, "Username", font);
        passwordField = new InputField({525, startY+gap*4}, {260, 45}, "Password", font, true); 
        confirmPasswordField = new InputField({815, startY+gap*4}, {260, 45}, "Confirm", font, true);
        
        registerButton = new Button({525, 520}, {550, 55}, "Dang Ky", font, 0, Theme::Primary); 
        backButton = new Button({525, 590}, {550, 45}, "Quay Lai", font, 0, sf::Color(150, 150, 150));
        
        auto initErr = [&](sf::Text& e, float x, float y) {
            e.setFont(font); 
            e.setCharacterSize(14);
            e.setFillColor(Theme::Error);
            e.setPosition(x, y);
            e.setString(""); 
        };
        initErr(errName, 525, startY+50); 
        initErr(errPhone, 525, startY+gap+50);
        initErr(errEmail, 525, startY+gap*2+50);
        initErr(errUsername, 525, startY+gap*3+50);
        initErr(errPassword, 525, startY+gap*4+50); 
        initErr(errConfirm, 815, startY+gap*4+50);
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
        if (registerButton->handleClick(mousePos)) 
            return 1;
        if (switchText.getGlobalBounds().contains(mousePos))
            return 2;
        if (backButton->handleClick(mousePos)) 
            return 3; 
        return 0;
    }
    
    void render(sf::RenderWindow& window) { 
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
    
    string getName() const { return nameField->getText(); } 
    string getPhone() const { return phoneField->getText(); } 
    string getEmail() const { return emailField->getText(); }
    string getUsername() const { return usernameField->getText(); } 
    string getPassword() const { return passwordField->getText(); } 
    string getConfirmPassword() const { return confirmPasswordField->getText(); }
    
    void clearFields() { 
        nameField->clear(); 
        phoneField->clear();
        emailField->clear();
        usernameField->clear();
        passwordField->clear();
        confirmPasswordField->clear(); 
        errName.setString("");
        errPhone.setString("");
        errEmail.setString("");
        errUsername.setString("");
        errPassword.setString("");
        errConfirm.setString(""); 
    }
    
    void setUsernameError(const std::string& msg) { errUsername.setString(msg); }

    bool validate() {
        bool ok = true;
        errName.setString("");
        errPhone.setString("");
        errEmail.setString("");
        errUsername.setString("");
        errPassword.setString("");
        errConfirm.setString("");

        if (getName().empty()) { 
            errName.setString("Vui long nhap ho ten");
            ok = false; 
        }
        if (!checkSDT(getPhone())) {
            errPhone.setString("SDT phai la so (it nhat 10 so)"); 
            ok = false; 
        }
        if (!checkEmail(getEmail())) { 
            errEmail.setString("Email khong hop le (can co @ va .com)"); 
            ok = false;
        }
        if (getUsername().length() < 4) {
            errUsername.setString("Username phai tu 4 ky tu tro len"); 
            ok = false;
        }
        if (!checkPass(getPassword())) {
            errPassword.setString("Mat khau phai tu 6 ky tu");
            ok = false; 
        }
        if (getPassword() != getConfirmPassword()) {
            errConfirm.setString("Mat khau xac nhan khong khop"); 
            ok = false; 
        }  
        return ok;
    }
};
#endif
