// ============================================
// ui/Modal.hpp - Popup overlay với bo góc
// ============================================
#ifndef MODAL_HPP
#define MODAL_HPP

#include <SFML/Graphics.hpp>
#include "RoundedRectangle.hpp"

class Modal {
private:
    sf::RectangleShape overlay;
    RoundedRectangleShape modalBox;
    RoundedRectangleShape closeButton;
    sf::Text closeText;
    bool isVisible;

public:
    Modal(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        overlay.setPosition(0, 0);

        modalBox.setSize(sf::Vector2f(900, 600));
        modalBox.setCornerRadius(16.0f); // Bo góc 16px
        modalBox.setPosition(250, 150);
        modalBox.setFillColor(sf::Color(30, 35, 50));

        closeButton.setSize(sf::Vector2f(35, 35));
        closeButton.setCornerRadius(17.5f); // Bo góc tròn
        closeButton.setPosition(1115, 155);
        closeButton.setFillColor(sf::Color(60, 63, 75));

        closeText.setFont(font);
        closeText.setString("X");
        closeText.setCharacterSize(20);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition(1128, 160);

        isVisible = false;
    }

    void show() { isVisible = true; }
    void hide() { isVisible = false; }
    bool isShown() const { return isVisible; }

    bool handleClose(sf::Vector2f mousePos) {
        if (closeButton.getGlobalBounds().contains(mousePos)) {
            hide();
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        if (isVisible) {
            window.draw(overlay);
            window.draw(modalBox);
            window.draw(closeButton);
            window.draw(closeText);
        }
    }

    RoundedRectangleShape& getModalBox() { return modalBox; }
};

#endif