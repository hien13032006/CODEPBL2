#ifndef RETURN_BOOK_SCREEN_HPP
#define RETURN_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "Book.h"
#include "InputField.hpp"
#include <string>

class ReturnBookScreen {
private:
    Modal* modal;

    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text bookInfoText;
    sf::Text returnMsg;

    InputField* maSachField;
    Button* returnButton;
    Button* cancelButton;

    sf::Text errMaSach;

    // KHUNG HỎI ĐÁNH GIÁ
    sf::RectangleShape ratingPanel;
    sf::Text askRatingText;
    Button* yesButton;
    Button* noButton;

    // Animation rung
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;
    bool askRatingVisible = false;

    Sach* currentBook = nullptr;

public:

    ReturnBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        // Panel chính
        panel.setSize(sf::Vector2f(600, 380));
        panel.setFillColor(sf::Color(40, 50, 70, 220));
        panel.setPosition(400, 260);

        // Title
        titleText.setFont(font);
        titleText.setString("Tra Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.width / 2, tb.height / 2);
        titleText.setPosition(panel.getPosition().x + 300, panel.getPosition().y + 40);

        // Thông tin sách
        bookInfoText.setFont(font);
        bookInfoText.setCharacterSize(20);
        bookInfoText.setFillColor(sf::Color::White);
        bookInfoText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 100);

        // Message
        returnMsg.setFont(font);
        returnMsg.setCharacterSize(16);
        returnMsg.setFillColor(sf::Color(255, 80, 80));
        returnMsg.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 170);

        // Input maSach
        maSachField = new InputField(
            {panel.getPosition().x + 20, panel.getPosition().y + 100},
            {560, 50},
            "Nhap ma sach...", font
        );

        // Error text
        errMaSach.setFont(font);
        errMaSach.setCharacterSize(14);
        errMaSach.setFillColor(sf::Color(255, 80, 80));
        errMaSach.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 255);

        // Buttons
        returnButton = new Button(
            {panel.getPosition().x + 50, panel.getPosition().y + 310},
            {200, 40},
            "Tra",
            font
        );

        cancelButton = new Button(
            {panel.getPosition().x + 350, panel.getPosition().y + 310},
            {200, 40},
            "Huy",
            font,
            0,
            sf::Color(100, 100, 120)
        );

        // --- PANEL HỎI ĐÁNH GIÁ ---
        ratingPanel.setSize({500, 200});
        ratingPanel.setFillColor(sf::Color(50, 60, 90, 240));
        ratingPanel.setPosition(450, 300);

        askRatingText.setFont(font);
        showAskRating();
        askRatingText.setString("Ban co muon danh gia cuon sach nay?");
        askRatingText.setCharacterSize(20);
        askRatingText.setFillColor(sf::Color::White);
        askRatingText.setPosition(ratingPanel.getPosition().x + 40,
                                  ratingPanel.getPosition().y + 30);

        yesButton = new Button(
            {ratingPanel.getPosition().x + 60, ratingPanel.getPosition().y + 120},
            {150, 40}, "Co", font, 3, sf::Color(150, 220, 150)
        );

        noButton = new Button(
            {ratingPanel.getPosition().x + 280, ratingPanel.getPosition().y + 120},
            {150, 40}, "Khong", font, 4, sf::Color(220,120,120)
        );
    }

    ~ReturnBookScreen() {
        delete returnButton;
        delete cancelButton;
        delete maSachField;
        delete yesButton;
        delete noButton;
    }

    void setCurrentBook(Sach* book) {
        currentBook = book;
        if (book) {
            bookInfoText.setString(
                "Ten sach: " + book->getTenSach() +
                "\nMa sach: " + book->getMaSach()
            );
        }
    }

    std::string getMaSachInput() const { return maSachField->getText(); }

    void setReturnMessage(const std::string& msg) { returnMsg.setString(msg); }

    void startShake() { shaking = true; shakeTime = 0; }

    bool validate() {
        errMaSach.setString("");
        if (getMaSachInput().empty()) {
            errMaSach.setString("Vui long nhap ma sach");
            startShake();
            return false;
        }
        return true;
    }

    void showAskRating() { askRatingVisible = true; }
    void hideAskRating() { askRatingVisible = false; }

    void update(sf::Vector2f mousePos) {
        if (!askRatingVisible) {
            maSachField->update();
            returnButton->update(mousePos);
            cancelButton->update(mousePos);

            // animation rung
            if (shaking) {
                shakeTime += 0.02f;
                float offset = sin(shakeTime * 40) * 8;
                panel.setPosition(400 + offset, 260);
                if (shakeTime >= shakeDuration) {
                    shaking = false;
                    panel.setPosition(400, 260);
                }    
            }
        } else {
            yesButton->update(mousePos);
            noButton->update(mousePos);
        }
    }


    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (!askRatingVisible)
            maSachField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (askRatingVisible) {
            if (yesButton->handleClick(mousePos)) return 3; // sang man hinh rating
            if (noButton->handleClick(mousePos)) return 4;  // thoat
            return 0;
        }

        if (returnButton->handleClick(mousePos)) return 1;
        if (cancelButton->handleClick(mousePos)) return 2;

        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (!modal || !modal->isShown()) return;

        if (!askRatingVisible) {
            // Vẽ màn hình trả sách
            window.draw(panel);
            window.draw(titleText);
            window.draw(bookInfoText);
            maSachField->draw(window);
            window.draw(returnMsg);
            returnButton->draw(window);
            cancelButton->draw(window);
        } 

        if (askRatingVisible) {
            // Vẽ màn hình hỏi đánh giá
            window.draw(ratingPanel);
            window.draw(askRatingText);
            yesButton->draw(window);
            noButton->draw(window);
        }
    }


    void clear() {
        errMaSach.setString("");
        returnMsg.setString("");
        maSachField->clear();
        askRatingVisible = false;
        currentBook = nullptr;
        bookInfoText.setString("");
    }

    bool panelContains(sf::Vector2f pos) const {
        return panel.getGlobalBounds().contains(pos);
    }

    std::string getMaSach() const { return maSachField->getText(); }

    void setMessage(const std::string& msg) { returnMsg.setString(msg); }
};

#endif
