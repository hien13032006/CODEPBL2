#ifndef RETURN_BOOK_SCREEN_HPP
#define RETURN_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"

class ReturnBookScreen {
private:
    Modal* modal;
    RoundedRectangleShape panel, ratingPanel;
    sf::Text titleText, returnMsg, askRatingText;
    InputField* maSachField;
    Button* returnButton; Button* cancelButton; Button* yesButton; Button* noButton;
    bool shaking = false; float shakeTime = 0.0f; bool askRatingVisible = false;

public:
    ReturnBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Panel Tra (700x450)
        panel.setSize(sf::Vector2f(700, 450));
        panel.setCornerRadius(15.0f);
        panel.setFillColor(sf::Color(40, 45, 60));
        panel.setPosition(350, 225);

        titleText.setFont(font); titleText.setString("TRA SACH");
        titleText.setCharacterSize(32); titleText.setFillColor(sf::Color::White);
        titleText.setPosition(620, 260);

        maSachField = new InputField({400, 330}, {600, 55}, "Nhap ma sach can tra...", font);

        returnMsg.setFont(font); returnMsg.setCharacterSize(18); returnMsg.setFillColor(sf::Color(255, 80, 80));
        returnMsg.setPosition(400, 400);

        returnButton = new Button({420, 580}, {200, 55}, "Tra Sach", font);
        cancelButton = new Button({780, 580}, {200, 55}, "Huy", font, 0, sf::Color(180, 80, 80));

        // Panel Rating (500x300)
        ratingPanel.setSize({500, 300});
        ratingPanel.setCornerRadius(15.0f);
        ratingPanel.setFillColor(sf::Color(50, 60, 80));
        ratingPanel.setOutlineThickness(2); ratingPanel.setOutlineColor(sf::Color(100, 200, 100));
        ratingPanel.setPosition(450, 300);

        askRatingText.setFont(font);
        askRatingText.setString("Tra sach thanh cong!\nBan co muon danh gia cuon sach nay?");
        askRatingText.setCharacterSize(22); askRatingText.setFillColor(sf::Color::White);
        askRatingText.setPosition(490, 350);

        yesButton = new Button({500, 500}, {180, 50}, "Co, Luon!", font, 3, sf::Color(76, 175, 80));
        noButton = new Button({720, 500}, {180, 50}, "De sau", font, 4, sf::Color(120, 120, 120));
    }

    ~ReturnBookScreen() { delete returnButton; delete cancelButton; delete maSachField; delete yesButton; delete noButton; }

    std::string getMaSachInput() const { return maSachField->getText(); }
    void setReturnMessage(const std::string& msg) { returnMsg.setString(msg); }
    void startShake() { shaking = true; shakeTime = 0; }
    
    bool validate() {
        if (getMaSachInput().empty()) { setReturnMessage("Vui long nhap ma sach!"); startShake(); return false; }
        return true;
    }

    void showAskRating() { askRatingVisible = true; }
    void hideAskRating() { askRatingVisible = false; }
    void clear() { maSachField->clear(); returnMsg.setString(""); askRatingVisible = false; }

    void update(sf::Vector2f mousePos) {
        if (!askRatingVisible) {
            maSachField->update(); returnButton->update(mousePos); cancelButton->update(mousePos);
            if (shaking) {
                shakeTime += 0.02f; float offset = sin(shakeTime * 50) * 10; panel.setPosition(350 + offset, 225);
                if (shakeTime >= 0.4f) { shaking = false; panel.setPosition(350, 225); }
            }
        } else { yesButton->update(mousePos); noButton->update(mousePos); }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { if (!askRatingVisible) maSachField->handleEvent(event, mousePos); }

    int handleClick(sf::Vector2f mousePos) {
        if (askRatingVisible) {
            if (yesButton->handleClick(mousePos)) return 3;
            if (noButton->handleClick(mousePos)) return 4;
            return 0;
        }
        if (returnButton->handleClick(mousePos)) return 1;
        if (cancelButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (!modal || !modal->isShown()) return;
        if (!askRatingVisible) {
            window.draw(panel); window.draw(titleText); maSachField->draw(window);
            window.draw(returnMsg); returnButton->draw(window); cancelButton->draw(window);
        }
        if (askRatingVisible) {
            window.draw(ratingPanel); window.draw(askRatingText); yesButton->draw(window); noButton->draw(window);
        }
    }
};

#endif 