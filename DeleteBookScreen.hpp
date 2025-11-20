#ifndef DELETE_BOOK_SCREEN_HPP
#define DELETE_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "InputField.hpp"
#include "Book.h"

class DeleteBookScreen {
private:
    Modal* modal;

    sf::RectangleShape panel;
    sf::Text titleText;

    sf::Text labelMaSach;
    sf::Text labelSoLuong;

    InputField* maSachField;
    InputField* soLuongField;

    sf::Text errText;

    Button* deleteButton;
    Button* cancelButton;

    sf::Text resultText;

    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:

    DeleteBookScreen(sf::Font& font, Modal* modalRef)
        : modal(modalRef)
    {
        // Panel chính
        panel.setSize({600, 430});
        panel.setFillColor(sf::Color(60, 60, 90, 220));
        panel.setPosition(400, 220);

        // Tiêu đề
        titleText.setFont(font);
        titleText.setString("XOA SACH");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(panel.getPosition().x + 200, panel.getPosition().y + 20);

        // Label mã sách
        labelMaSach.setFont(font);
        labelMaSach.setString("Nhap ma sach:");
        labelMaSach.setCharacterSize(20);
        labelMaSach.setFillColor(sf::Color::White);
        labelMaSach.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 90);

        // Field mã sách
        maSachField = new InputField(
            {panel.getPosition().x + 20, panel.getPosition().y + 120},
            {560, 45},
            "Ma sach...",
            font
        );

        // Label số lượng
        labelSoLuong.setFont(font);
        labelSoLuong.setString("Nhap so luong can xoa:");
        labelSoLuong.setCharacterSize(20);
        labelSoLuong.setFillColor(sf::Color::White);
        labelSoLuong.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 180);

        // Field số lượng
        soLuongField = new InputField(
            {panel.getPosition().x + 20, panel.getPosition().y + 210},
            {560, 45},
            "So luong...",
            font
        );

        // Text báo lỗi
        errText.setFont(font);
        errText.setCharacterSize(16);
        errText.setFillColor(sf::Color(255, 80, 80));
        errText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 265);

        // Button xóa
        deleteButton = new Button(
            {panel.getPosition().x + 50, panel.getPosition().y + 360},
            {200, 45},
            "Xoa",
            font
        );

        // Button hủy
        cancelButton = new Button(
            {panel.getPosition().x + 350, panel.getPosition().y + 360},
            {200, 45},
            "Huy",
            font,
            0,
            sf::Color(100, 100, 120)
        );

        // Text kết quả
        resultText.setFont(font);
        resultText.setCharacterSize(18);
        resultText.setFillColor(sf::Color::White);
        resultText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 300);
        resultText.setString("");
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

    bool validate() {
        errText.setString("");

        if (maSachField->getText().empty()) {
            errText.setString("Vui long nhap ma sach!");
            startShake();
            return false;
        }

        if (soLuongField->getText().empty()) {
            errText.setString("Vui long nhap so luong!");
            startShake();
            return false;
        }

        // kiểm tra số
        for (char c : soLuongField->getText()) {
            if (!isdigit(c)) {
                errText.setString("So luong phai la so nguyen!");
                startShake();
                return false;
            }
        }

        return true;
    }

    void update(sf::Vector2f mousePos) {
        maSachField->update();
        soLuongField->update();
        deleteButton->update(mousePos);
        cancelButton->update(mousePos);

        // animation rung
        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 40) * 8;
            panel.setPosition(400 + offset, 220);
            if (shakeTime >= shakeDuration) {
                shaking = false;
                panel.setPosition(400, 220);
            }
        }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        maSachField->handleEvent(event, mousePos);
        soLuongField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (deleteButton->handleClick(mousePos)) return 1;
        if (cancelButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void setResult(const std::string& msg) {
        resultText.setString(msg);
    }

    std::string getMaSach() const { return maSachField->getText(); }
    int getSoLuong() const { 
        if (soLuongField->getText().empty()) return 0;
        return std::stoi(soLuongField->getText());
    }

    void clear() {
        maSachField->clear();
        soLuongField->clear();
        errText.setString("");
        resultText.setString("");
    }

    bool panelContains(sf::Vector2f pos) const {
        return panel.getGlobalBounds().contains(pos);
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(panel);
            window.draw(titleText);
            window.draw(labelMaSach);
            window.draw(labelSoLuong);
            maSachField->draw(window);
            soLuongField->draw(window);
            deleteButton->draw(window);
            cancelButton->draw(window);
            window.draw(errText);
            window.draw(resultText);
        }
    }
};

#endif
