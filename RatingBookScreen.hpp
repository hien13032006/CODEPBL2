#ifndef RATING_BOOK_SCREEN_HPP
#define RATING_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "RoundedRectangle.hpp"

class RatingBookScreen {
private:
    sf::Font &font; Modal *modal;
    RoundedRectangleShape panel; sf::Text title; sf::Text message;
    Button* btnStar[5]; Button* btnSubmit; Button* btnCancel;
    int currentRating; std::string maSach;

public:
    RatingBookScreen(sf::Font &f, Modal *m) : font(f), modal(m), currentRating(0) {
        panel.setSize({650, 500}); panel.setCornerRadius(20.0f);
        panel.setFillColor(sf::Color(40, 45, 60)); panel.setPosition(375, 200);

        title.setFont(font); title.setString("DANH GIA SACH");
        title.setCharacterSize(32); title.setFillColor(sf::Color::White); title.setPosition(550, 240);

        message.setFont(font); message.setString("Ban thay cuon sach nay the nao?");
        message.setCharacterSize(22); message.setFillColor(sf::Color(200, 200, 200)); message.setPosition(500, 300);

        for (int i = 0; i < 5; i++) {
            btnStar[i] = new Button(
                {450.f + i * 90.f, 380.f}, {70, 70}, std::to_string(i+1), font, i+1, sf::Color(60, 65, 80)
            );
            btnStar[i]->getText().setCharacterSize(28);
        }

        btnSubmit = new Button({450, 580}, {220, 60}, "Gui Danh Gia", font, 101, sf::Color(76, 175, 80));
        btnCancel = new Button({730, 580}, {120, 60}, "Huy", font, 102, sf::Color(180, 80, 80));
    }

    ~RatingBookScreen() { for(int i=0;i<5;i++) delete btnStar[i]; delete btnSubmit; delete btnCancel; }

    void update(sf::Vector2f mousePos) { for(int i=0;i<5;i++) btnStar[i]->update(mousePos); btnSubmit->update(mousePos); btnCancel->update(mousePos); }

    int handleClick(sf::Vector2f mousePos) {
        for (int i = 0; i < 5; i++) {
            if (btnStar[i]->handleClick(mousePos)) { currentRating = i + 1; highlightStars(); return 0; }
        }
        if (btnSubmit->handleClick(mousePos)) return 1;
        if (btnCancel->handleClick(mousePos)) return 2;
        return 0;
    }

    void highlightStars() {
        for (int i = 0; i < 5; i++) {
            if (i < currentRating) btnStar[i]->setColor(sf::Color(255, 193, 94)); 
            else btnStar[i]->setColor(sf::Color(60, 65, 80));
        }
    }

    int getScore() const { return currentRating == 0 ? 5 : currentRating * 2; }
    void setBookID(const std::string& id) { maSach = id; currentRating = 0; highlightStars(); }
    std::string getBookID() const { return maSach; }

    void render(sf::RenderWindow &window) {
        if (modal && modal->isShown()) {
            modal->draw(window); window.draw(panel); window.draw(title); window.draw(message);
            for(int i=0;i<5;i++) btnStar[i]->draw(window);
            btnSubmit->draw(window); btnCancel->draw(window);
        }
    }
};

#endif