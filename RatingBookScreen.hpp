#ifndef RATING_BOOK_SCREEN_HPP
#define RATING_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class RatingBookScreen {
private:
    sf::Font &font; 
    Modal *modal;
    RoundedRectangleShape panel; 
    sf::Text title; 
    sf::Text message;
    Button* btnStar[5]; 
    Button* btnSubmit; 
    Button* btnCancel;
    int currentRating; // Lưu số sao (1 -> 5)
    std::string maSach;

    // Ký tự ngôi sao
    const std::string STAR_SYMBOL = "\xE2\x98\x85"; 

public:
    RatingBookScreen(sf::Font &f, Modal *m) : font(f), modal(m), currentRating(0) {
        // Panel nền trắng, viền hồng
        panel.setSize({600, 400}); 
        panel.setCornerRadius(20.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(350, 160); // Căn giữa 1300x720

        title.setFont(font); 
        title.setString("DANH GIA SACH");
        title.setCharacterSize(30); 
        title.setFillColor(Theme::Primary); 
        title.setPosition(490, 190);

        message.setFont(font); 
        message.setString("Ban cam thay cuon sach nay the nao?");
        message.setCharacterSize(20); 
        message.setFillColor(Theme::TextLight); 
        message.setPosition(420, 240);

        // Tạo 5 nút ngôi sao
        float startX = 410.0f;
        float gap = 70.0f; 
        
        for (int i = 0; i < 5; i++) {
            // ID từ 1 đến 5
            btnStar[i] = new Button(
                {startX + i * gap, 300.f}, 
                {60, 60}, 
                STAR_SYMBOL, 
                font, 
                i + 1, 
                sf::Color(220, 220, 220) // Màu xám khi chưa chọn
            );
            btnStar[i]->setCornerRadius(30.0f); // Nút tròn
            btnStar[i]->setTextColor(sf::Color::White);
            btnStar[i]->setTextSize(28); 
        }

        btnSubmit = new Button({400, 480}, {200, 50}, "Gui Danh Gia", font, 101, Theme::Success);
        btnCancel = new Button({620, 480}, {120, 50}, "Huy", font, 102, sf::Color(150, 150, 150));
        
        highlightStars();
    }

    ~RatingBookScreen() { 
        for(int i=0; i<5; i++) delete btnStar[i]; 
        delete btnSubmit; 
        delete btnCancel; 
    }

    void update(sf::Vector2f mousePos) {
        for(int i=0; i<5; i++) btnStar[i]->update(mousePos);
        btnSubmit->update(mousePos); 
        btnCancel->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        for (int i = 0; i < 5; i++) {
            if (btnStar[i]->handleClick(mousePos)) { 
                currentRating = i + 1; // Lưu số sao (1-5)
                highlightStars(); 
                return 0; 
            }
        }
        if (btnSubmit->handleClick(mousePos)) return 1; // Gửi
        if (btnCancel->handleClick(mousePos)) return 2; // Hủy
        return 0;
    }

    void highlightStars() {
        sf::Color activeColor(255, 193, 7); // Vàng (Amber)
        sf::Color inactiveColor(220, 220, 220); // Xám

        for (int i = 0; i < 5; i++) {
            if (i < currentRating) btnStar[i]->setColor(activeColor); 
            else btnStar[i]->setColor(inactiveColor); 
        }
    }

    // [QUAN TRỌNG] Quy đổi 5 sao thành thang điểm 10
    int getScore() const { 
        return (currentRating == 0) ? 0 : (currentRating * 2); 
    } 
    
    void setBookID(const std::string& id) { 
        maSach = id; 
        currentRating = 0; 
        highlightStars(); 
    }
    
    std::string getBookID() const { return maSach; }

    void render(sf::RenderWindow &window) {
        if (modal && modal->isShown()) {
            modal->draw(window); 
            window.draw(panel); 
            window.draw(title); 
            window.draw(message);
            for(int i=0; i<5; i++) btnStar[i]->draw(window);
            btnSubmit->draw(window); 
            btnCancel->draw(window);
        }
    }
};

#endif