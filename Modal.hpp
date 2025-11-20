#ifndef MODAL_HPP
#define MODAL_HPP

#include <SFML/Graphics.hpp>
#include <functional>

class Modal {
private:
    sf::RectangleShape overlay;
    sf::RectangleShape modalBox;
    sf::RectangleShape closeButton;
    sf::Text closeText;

    sf::Text messageText;

    sf::RectangleShape btnYes, btnNo;
    sf::Text txtYes, txtNo;

    bool isVisible;
    bool isOptionMode; 

public:

    std::function<void(int)> onOptionSelect;

    Modal(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        overlay.setPosition(0, 0);

        modalBox.setSize(sf::Vector2f(900, 600));
        modalBox.setPosition(250, 150);
        modalBox.setFillColor(sf::Color(30, 35, 50));

        closeButton.setSize(sf::Vector2f(35, 35));
        closeButton.setPosition(1115, 155);
        closeButton.setFillColor(sf::Color(60, 63, 75));

        closeText.setFont(font);
        closeText.setString("X");
        closeText.setCharacterSize(20);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition(1128, 160);

        messageText.setFont(font);
        messageText.setCharacterSize(30);
        messageText.setFillColor(sf::Color::White);
        messageText.setPosition(300, 300);

        btnYes.setSize(sf::Vector2f(150, 60));
        btnYes.setFillColor(sf::Color(70, 130, 180));
        btnYes.setPosition(350, 550);

        txtYes.setFont(font);
        txtYes.setCharacterSize(26);
        txtYes.setFillColor(sf::Color::White);
        txtYes.setString("Co");
        txtYes.setPosition(395, 560);

        btnNo.setSize(sf::Vector2f(150, 60));
        btnNo.setFillColor(sf::Color(150, 50, 60));
        btnNo.setPosition(600, 550);

        txtNo.setFont(font);
        txtNo.setCharacterSize(26);
        txtNo.setFillColor(sf::Color::White);
        txtNo.setString("Khong");
        txtNo.setPosition(625, 560);

        isVisible = false;
        isOptionMode = false;
    }

    void show() { isVisible = true; }
    void hide() { isVisible = false; }
    bool isShown() const { return isVisible; }

    void showMessage(const std::string& text) {
        messageText.setString(text);
        isOptionMode = false;
        show();
    }

    void setCustomText(const std::string& text) {
        messageText.setString(text);
    }

    bool handleClose(sf::Vector2f mousePos) {
        if (closeButton.getGlobalBounds().contains(mousePos)) {
            hide();
            return true;
        }
        return false;
    }

    void showOptions(const std::string& yesText, const std::string& noText) {
        txtYes.setString(yesText);
        txtNo.setString(noText);
        isOptionMode = true;
        show();
    }

    void handleClick(sf::Vector2f mousePos) {
        if (!isVisible || !isOptionMode) return;

        if (btnYes.getGlobalBounds().contains(mousePos)) {
            hide();
            if (onOptionSelect) onOptionSelect(1);
        }
        else if (btnNo.getGlobalBounds().contains(mousePos)) {
            hide();
            if (onOptionSelect) onOptionSelect(0);
        }
    }

    void draw(sf::RenderWindow& window) {
        if (isVisible) {
            window.draw(overlay);
            window.draw(modalBox);
            window.draw(closeButton);
            window.draw(closeText);
            window.draw(messageText);
        }

        if (isOptionMode) {
            window.draw(btnYes);
            window.draw(btnNo);
            window.draw(txtYes);
            window.draw(txtNo);
        }
    }

    sf::RectangleShape& getModalBox() { return modalBox; }
};

#endif