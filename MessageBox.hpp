#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.hpp"

enum class MessageType {
    INFO,
    SUCCESS,
    WARNING,
    ERROR,
    CONFIRM
};

class MessageBox {
private:
    sf::RectangleShape overlay;
    sf::RectangleShape box;
    sf::Text titleText;
    sf::Text messageText;
    Button* okButton;
    Button* cancelButton;
    bool isVisible;
    bool hasCancel;
    MessageType type;
    std::function<void()> onConfirm;
    std::function<void()> onCancel;

public:
    MessageBox(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));

        box.setSize(sf::Vector2f(500, 250));
        box.setPosition(450, 325);
        box.setFillColor(sf::Color(30, 35, 50));
        box.setOutlineThickness(2);

        titleText.setFont(font);
        titleText.setCharacterSize(22);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(480, 350);

        messageText.setFont(font);
        messageText.setCharacterSize(16);
        messageText.setFillColor(sf::Color(200, 200, 200));
        messageText.setPosition(480, 400);

        okButton = new Button(sf::Vector2f(650, 500), sf::Vector2f(120, 45),
                             "OK", font, 0, sf::Color(100, 180, 100));
        
        cancelButton = new Button(sf::Vector2f(500, 500), sf::Vector2f(120, 45),
                                 "Huy", font, 0, sf::Color(120, 120, 120));

        isVisible = false;
        hasCancel = false;
        type = MessageType::INFO;
    }

    ~MessageBox() {
        delete okButton;
        delete cancelButton;
    }

    void show(const std::string& title, const std::string& message, 
              MessageType msgType = MessageType::INFO, bool showCancel = false) {
        titleText.setString(title);
        messageText.setString(message);
        type = msgType;
        hasCancel = showCancel;
        isVisible = true;

        // Đặt màu theo loại
        switch(type) {
            case MessageType::SUCCESS:
                box.setOutlineColor(sf::Color(100, 180, 100));
                titleText.setFillColor(sf::Color(100, 255, 100));
                break;
            case MessageType::ERROR:
                box.setOutlineColor(sf::Color(255, 100, 100));
                titleText.setFillColor(sf::Color(255, 100, 100));
                break;
            case MessageType::WARNING:
                box.setOutlineColor(sf::Color(255, 193, 94));
                titleText.setFillColor(sf::Color(255, 193, 94));
                break;
            case MessageType::CONFIRM:
                box.setOutlineColor(sf::Color(100, 150, 255));
                titleText.setFillColor(sf::Color(100, 150, 255));
                break;
            default:
                box.setOutlineColor(sf::Color(100, 100, 120));
                titleText.setFillColor(sf::Color::White);
        }

        // Điều chỉnh vị trí nút
        if (hasCancel) {
            okButton->setPosition(sf::Vector2f(650, 500));
            cancelButton->setPosition(sf::Vector2f(500, 500));
        } else {
            okButton->setPosition(sf::Vector2f(575, 500));
        }
    }

    void hide() {
        isVisible = false;
    }

    bool isShown() const {
        return isVisible;
    }

    void setOnConfirm(std::function<void()> callback) {
        onConfirm = callback;
    }

    void setOnCancel(std::function<void()> callback) {
        onCancel = callback;
    }

    void update(sf::Vector2f mousePos) {
        if (!isVisible) return;
        
        okButton->update(mousePos);
        if (hasCancel) {
            cancelButton->update(mousePos);
        }
    }

    bool handleClick(sf::Vector2f mousePos) {
        if (!isVisible) return false;

        if (okButton->handleClick(mousePos)) {
            if (onConfirm) onConfirm();
            hide();
            return true;
        }

        if (hasCancel && cancelButton->handleClick(mousePos)) {
            if (onCancel) onCancel();
            hide();
            return true;
        }

        return false;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        window.draw(overlay);
        window.draw(box);
        window.draw(titleText);
        window.draw(messageText);
        okButton->draw(window);
        
        if (hasCancel) {
            cancelButton->draw(window);
        }
    }
};

#endif