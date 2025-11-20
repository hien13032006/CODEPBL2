#ifndef RATING_BOOK_SCREEN_HPP
#define RATING_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.hpp"
#include "Modal.hpp"

class RatingBookScreen {
private:
    sf::Font &font;
    Modal *modal;

    sf::RectangleShape panel;
    sf::Text title;
    sf::Text message;

    Button* btnStar[5];
    Button* btnSubmit;
    Button* btnCancel;

    int currentRating;
    std::string maSach;

public:
    RatingBookScreen(sf::Font &f, Modal *m)
        : font(f), modal(m), currentRating(0)
    {
        // Panel nền
        panel.setSize({600, 400});
        panel.setFillColor(sf::Color(40, 50, 70, 230));
        panel.setPosition(380, 200);

        // Tiêu đề
        title.setFont(font);
        title.setString("Danh Gia Sach");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::White);
        title.setPosition(480, 230);

        // Message mô tả
        message.setFont(font);
        message.setString("Ban danh gia sach nay bao nhieu sao?");
        message.setCharacterSize(18);
        message.setFillColor(sf::Color(200, 200, 200));
        message.setPosition(430, 290);

        // Các nút sao
        for (int i = 0; i < 5; i++) {
            btnStar[i] = new Button(
                {420.f + i * 90.f, 340.f},
                {70, 60},
                std::to_string(i + 1),
                font,
                i + 1,
                sf::Color(255, 193, 94)
            );
        }

        // Submit
        btnSubmit = new Button(
            {460, 520},
            {180, 50},
            "Xac nhan",
            font,
            101,
            sf::Color(150, 220, 150)
        );

        // Cancel
        btnCancel = new Button(
            {680, 520},
            {180, 50},
            "Huy",
            font,
            102,
            sf::Color(220, 120, 120)
        );
    }

    ~RatingBookScreen() {
        for (int i = 0; i < 5; i++) delete btnStar[i];
        delete btnSubmit;
        delete btnCancel;
    }

    void update(sf::Vector2f mousePos) {
        for (int i = 0; i < 5; i++)
            btnStar[i]->update(mousePos);

        btnSubmit->update(mousePos);
        btnCancel->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        // Click sao
        for (int i = 0; i < 5; i++) {
            if (btnStar[i]->handleClick(mousePos)) {
                currentRating = i + 1;
                highlightStars();
                return 10 + i;  // mã hành động
            }
        }

        // Submit
        if (btnSubmit->handleClick(mousePos))
            return 1;

        // Cancel
        if (btnCancel->handleClick(mousePos))
            return 2;

        return 0;
    }

    // Hiệu ứng chọn sao (tô màu)
    void highlightStars() {
        for (int i = 0; i < 5; i++) {
            if (i < currentRating)
                btnStar[i]->setColor(sf::Color(255, 215, 0));   // vàng sáng
            else
                btnStar[i]->setColor(sf::Color(255, 193, 94));  // màu gốc
        }
    }

    int getRating() const { return currentRating; }

    void render(sf::RenderWindow &window) {
        if (modal && modal->isShown()) {
            modal->draw(window);
            window.draw(panel);
            window.draw(title);
            window.draw(message);

            for (int i = 0; i < 5; i++)
                btnStar[i]->draw(window);

            btnSubmit->draw(window);
            btnCancel->draw(window);
        }
    }

    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            handleClick(mousePos);
        }
    }

    int getScore() const { return currentRating; }

    std::string getComment() const { return ""; } 


    void setBookID(const std::string& id) { maSach = id; }
    std::string getBookID() const { return maSach; }
};

#endif
