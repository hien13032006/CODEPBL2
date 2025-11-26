#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

enum class MsgType { INFO, SUCCESS, WARNING, ERROR, CONFIRM };

class MessageBox {
private:
    sf::RectangleShape overlay;
    RoundedRectangleShape box;
    sf::Text titleText, msgText;
    Button *btnOK, *btnCancel;
    bool visible, hasCancel;

public:
    MessageBox(sf::Font& font) {
        visible = false; hasCancel = false;
        overlay.setSize(sf::Vector2f(1300, 720));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Làm tối nền

        box.setSize(sf::Vector2f(500, 250));
        box.setCornerRadius(15.0f);
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setPosition(400, 235); 

        titleText.setFont(font); titleText.setCharacterSize(24); titleText.setPosition(430, 255);
        msgText.setFont(font); msgText.setCharacterSize(18); msgText.setFillColor(Theme::TextDark); msgText.setPosition(430, 300);

        btnOK = new Button({430, 420}, {200, 45}, "OK", font, 1, Theme::Success);
        btnCancel = new Button({660, 420}, {200, 45}, "Huy", font, 2, sf::Color(150, 150, 150));
    }

    ~MessageBox() { delete btnOK; delete btnCancel; }

    // Hàm mới để đổi chữ nút bấm
    void setButtonLabels(std::string okLabel, std::string cancelLabel = "Huy") {
        btnOK->setText(okLabel);
        btnCancel->setText(cancelLabel);
    }

    void show(const std::string& title, const std::string& msg, MsgType type, bool showCancel = false) {
        titleText.setString(title);
        msgText.setString(msg);
        hasCancel = showCancel;
        visible = true;

        if (type == MsgType::CONFIRM) {
            titleText.setFillColor(Theme::Primary);
            box.setOutlineColor(Theme::Primary);
            btnOK->setColor(Theme::Primary);
        } else if (type == MsgType::SUCCESS) {
            titleText.setFillColor(Theme::Success);
            box.setOutlineColor(Theme::Success);
            btnOK->setColor(Theme::Success);
        } else {
            titleText.setFillColor(Theme::Error);
            box.setOutlineColor(Theme::Error);
            btnOK->setColor(Theme::Error);
        }

        // Căn chỉnh vị trí nút
        if (!hasCancel) {
            btnOK->setPosition({550, 420}); // Giữa
        } else {
            btnOK->setPosition({430, 420}); // Trái
            btnCancel->setPosition({660, 420}); // Phải
        }
    }

    void hide() { visible = false; }
    bool isShown() const { return visible; }

    void update(sf::Vector2f mousePos) {
        if (!visible) return;
        btnOK->update(mousePos);
        if (hasCancel) btnCancel->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (!visible) return 0;
        if (btnOK->handleClick(mousePos)) return 1;
        if (hasCancel && btnCancel->handleClick(mousePos)) return 2;
        return 0;
    }

    void draw(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(overlay);
        window.draw(box);
        window.draw(titleText);
        window.draw(msgText);
        btnOK->draw(window);
        if (hasCancel) btnCancel->draw(window);
    }
};
#endif