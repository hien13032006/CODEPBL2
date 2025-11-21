#ifndef DELETE_BOOK_SCREEN_HPP
#define DELETE_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"

class DeleteBookScreen {
private:
    Modal* modal; RoundedRectangleShape panel;
    sf::Text titleText, labelMaSach, labelSoLuong, errText, resultText;
    InputField* maSachField; InputField* soLuongField;
    Button* deleteButton; Button* cancelButton;
    bool shaking = false; float shakeTime = 0.0f;

public:
    DeleteBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        panel.setSize({600, 450}); panel.setCornerRadius(15.0f);
        panel.setFillColor(sf::Color(40, 45, 60)); panel.setPosition(400, 225);

        titleText.setFont(font); titleText.setString("XOA SACH");
        titleText.setCharacterSize(32); titleText.setFillColor(sf::Color::White); titleText.setPosition(620, 255);

        labelMaSach.setFont(font); labelMaSach.setString("Ma sach:");
        labelMaSach.setCharacterSize(18); labelMaSach.setFillColor(sf::Color(200, 200, 200)); labelMaSach.setPosition(440, 310);
        maSachField = new InputField({440, 340}, {520, 45}, "Nhap ma...", font);

        labelSoLuong.setFont(font); labelSoLuong.setString("So luong can xoa:");
        labelSoLuong.setCharacterSize(18); labelSoLuong.setFillColor(sf::Color(200, 200, 200)); labelSoLuong.setPosition(440, 400);
        soLuongField = new InputField({440, 430}, {520, 45}, "Nhap so luong...", font);

        errText.setFont(font); errText.setCharacterSize(16); errText.setFillColor(sf::Color(255, 80, 80)); errText.setPosition(440, 480);
        resultText.setFont(font); resultText.setCharacterSize(18); resultText.setFillColor(sf::Color(100, 255, 100)); resultText.setPosition(440, 520);

        deleteButton = new Button({460, 580}, {180, 50}, "Xoa", font, 0, sf::Color(220, 80, 80));
        cancelButton = new Button({660, 580}, {180, 50}, "Huy", font, 0, sf::Color(100, 100, 120));
    }

    ~DeleteBookScreen() { delete maSachField; delete soLuongField; delete deleteButton; delete cancelButton; }

    void startShake() { shaking = true; shakeTime = 0; }
    void setResult(const std::string& msg) { resultText.setString(msg); }
    std::string getMaSach() const { return maSachField->getText(); }
    int getSoLuong() const { try { return std::stoi(soLuongField->getText()); } catch(...) { return 0; } }
    void clear() { maSachField->clear(); soLuongField->clear(); errText.setString(""); resultText.setString(""); }

    bool validate() {
        errText.setString("");
        if (maSachField->getText().empty()) { errText.setString("Nhap ma sach!"); startShake(); return false; }
        if (soLuongField->getText().empty()) { errText.setString("Nhap so luong!"); startShake(); return false; }
        return true;
    }

    void update(sf::Vector2f mousePos) {
        maSachField->update(); soLuongField->update();
        deleteButton->update(mousePos); cancelButton->update(mousePos);
        if (shaking) {
            shakeTime += 0.02f; float offset = sin(shakeTime * 50) * 10; panel.setPosition(400 + offset, 225);
            if (shakeTime >= 0.4f) { shaking = false; panel.setPosition(400, 225); }
        }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { maSachField->handleEvent(event, mousePos); soLuongField->handleEvent(event, mousePos); }

    int handleClick(sf::Vector2f mousePos) {
        if (deleteButton->handleClick(mousePos)) return 1;
        if (cancelButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(panel); window.draw(titleText);
            window.draw(labelMaSach); maSachField->draw(window);
            window.draw(labelSoLuong); soLuongField->draw(window);
            window.draw(errText); window.draw(resultText);
            deleteButton->draw(window); cancelButton->draw(window);
        }
    }
};

#endif