#ifndef DELETE_BOOK_SCREEN_HPP
#define DELETE_BOOK_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class DeleteBookScreen {
private:
    Modal* modal; 
    RoundedRectangleShape panel; 
    sf::Text titleText, labelMaSach, labelSoLuong, errText, resultText; 
    InputField* maSachField;
    InputField* soLuongField;
    Button *deleteButton, *cancelButton;
    bool shaking = false; 
    float shakeTime = 0.0f;

public:
    DeleteBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        panel.setSize({600, 400}); 
        panel.setCornerRadius(15.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Error);
        panel.setPosition(350, 160);

        titleText.setFont(font);
        titleText.setString("XOA SACH");
        titleText.setCharacterSize(32);
        titleText.setFillColor(Theme::Error); 
        titleText.setPosition(560, 190);

        labelMaSach.setFont(font); 
        labelMaSach.setString("Ma sach can xoa:"); 
        labelMaSach.setCharacterSize(18); 
        labelMaSach.setFillColor(Theme::TextDark);
        labelMaSach.setPosition(390, 250);
        maSachField = new InputField({390, 280}, {520, 45}, "Nhap ma...", font);

        errText.setFont(font); 
        errText.setCharacterSize(16); 
        errText.setFillColor(Theme::Error); 
        errText.setPosition(390, 415);

        resultText.setFont(font); 
        resultText.setCharacterSize(18); 
        resultText.setFillColor(Theme::Success);
        resultText.setPosition(390, 435);

        labelSoLuong.setFont(font);
        labelSoLuong.setString("So luong can xoa:");
        labelSoLuong.setCharacterSize(18);
        labelSoLuong.setFillColor(Theme::TextDark);
        labelSoLuong.setPosition(390, 330);
        soLuongField = new InputField({390, 360}, {520, 45}, "Nhap so...", font);

        deleteButton = new Button({410, 480}, {180, 50}, "Xoa", font, 0, Theme::Error);
        cancelButton = new Button({610, 480}, {180, 50}, "Huy", font, 0, sf::Color(150, 150, 150));
    }
    ~DeleteBookScreen() { 
        delete maSachField; 
        delete soLuongField;
        delete deleteButton; 
        delete cancelButton; 
    }
    void startShake() { 
        shaking = true; 
        shakeTime = 0; 
    } 
    void setResult(const std::string& msg) { 
        resultText.setString(msg); 
    }

    std::string getMaSach() const { 
        return maSachField->getText(); 
    } 

    void setBookID(const std::string& id) { 
        maSachField->setText(id); 
    }

    int getSoLuong() const {
        return std::stoi(soLuongField->getText());
    }

    void clear() { 
        maSachField->clear();
        soLuongField->clear();
        errText.setString("");
        resultText.setString("");
    }

    bool validate() { 
        errText.setString("");
        if (maSachField->getText().empty()) {
            errText.setString("Nhap ma sach!");
            startShake(); return false;
        } 

        if (soLuongField->getText().empty()) {
            errText.setString("Nhap so luong!");
            startShake();
            return false;
        }

        try {
            int sl = std::stoi(soLuongField->getText());
            if (sl <= 0) {
                errText.setString("So luong phai phai lon 0!");
                startShake();
                return false;
            }
        } catch (...) {
            errText.setString("So luong khong hop le!");
            startShake();
            return false;
        }

        return true; 
    }

    void update(sf::Vector2f mousePos) { 
        maSachField->update();
        soLuongField->update();
        deleteButton->update(mousePos);
        cancelButton->update(mousePos);
        if (shaking) { 
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 50) * 10; 
            panel.setPosition(350 + offset, 160); 
            if (shakeTime >= 0.4f) {
                shaking = false;
                panel.setPosition(350, 160); 
            }
        } 
    }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        maSachField->handleEvent(event, mousePos); 
        soLuongField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) { 
        if (deleteButton->handleClick(mousePos)) 
            return 1; 
        if (cancelButton->handleClick(mousePos)) 
            return 2;
        return 0; 
    }
    void render(sf::RenderWindow& window) { 
        if (modal && modal->isShown()) {
            window.draw(panel); 
            window.draw(titleText); 
            window.draw(labelMaSach);
            maSachField->draw(window);
            window.draw(labelSoLuong);
            soLuongField->draw(window); 
            window.draw(errText);
            window.draw(resultText);
            deleteButton->draw(window);
            cancelButton->draw(window); 
        } 
    }
};
#endif
