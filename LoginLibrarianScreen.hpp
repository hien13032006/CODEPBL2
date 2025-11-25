#ifndef LOGIN_LIBRARIAN_SCREEN_HPP
#define LOGIN_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class LoginLibrarianScreen {
private:
    Modal* modal; 
    RoundedRectangleShape panel; 
    sf::Text titleText, errLogin; // Thêm text báo lỗi
    InputField *usernameField, *passwordField; 
    Button *loginButton, *backButton;
    
    // Biến hiệu ứng rung
    bool shaking = false; 
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    LoginLibrarianScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Panel căn giữa (1300x720)
        panel.setSize({500, 600}); 
        panel.setCornerRadius(20.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(400, 60);

        titleText.setFont(font); 
        titleText.setString("QUAN TRI VIEN"); 
        titleText.setCharacterSize(30); 
        titleText.setFillColor(Theme::Primary); 
        sf::FloatRect b = titleText.getLocalBounds(); 
        titleText.setOrigin(b.width/2, 0); 
        titleText.setPosition(650, 130); 

        usernameField = new InputField({450, 230}, {400, 55}, "Username", font); 
        passwordField = new InputField({450, 310}, {400, 55}, "Password", font, true);

        loginButton = new Button({450, 410}, {400, 60}, "Dang Nhap", font, 0, Theme::Primary); 
        backButton = new Button({450, 490}, {400, 50}, "Quay Lai", font, 0, sf::Color(150, 150, 150));

        // Cấu hình dòng báo lỗi
        errLogin.setFont(font); 
        errLogin.setCharacterSize(16); 
        errLogin.setFillColor(Theme::Error); 
        errLogin.setPosition(450, 375);
        errLogin.setString("");
    }

    ~LoginLibrarianScreen() { 
        delete usernameField; delete passwordField; delete loginButton; delete backButton; 
    }

    // Hàm kích hoạt rung
    void startShake() { shaking = true; shakeTime = 0; }

    // Hàm hiển thị lỗi và rung
    void setLoginError(const std::string& msg) {
        errLogin.setString(msg);
        // Căn giữa thông báo lỗi
        sf::FloatRect b = errLogin.getLocalBounds();
        errLogin.setOrigin(b.width/2, 0);
        errLogin.setPosition(650, 380); 
        startShake();
    }

    bool validate() {
        errLogin.setString("");
        if (getUsername().empty() || getPassword().empty()) {
            setLoginError("Vui long nhap day du thong tin!");
            return false;
        }
        return true;
    }

    void update(sf::Vector2f mousePos) {
        usernameField->update(); 
        passwordField->update(); 
        loginButton->update(mousePos); 
        backButton->update(mousePos);

        // Logic rung lắc
        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 50) * 10; 
            panel.setPosition(400 + offset, 60);
            if (shakeTime >= shakeDuration) { 
                shaking = false; 
                panel.setPosition(400, 60); // Trả về vị trí cũ
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
            window.draw(panel); 
            window.draw(titleText);
            usernameField->draw(window); 
            passwordField->draw(window);
            window.draw(errLogin); // Vẽ lỗi
            loginButton->draw(window); 
            backButton->draw(window);
        }
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    
    void clearFields() { 
        usernameField->clear(); 
        passwordField->clear(); 
        errLogin.setString(""); 
    }
};

#endif