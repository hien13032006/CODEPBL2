#ifndef RATING_BOOK_SCREEN_HPP
#define RATING_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
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
    sf::ConvexShape starShapes[5];    
    Button* btnSubmit; 
    Button* btnCancel;    
    int currentRating; 
    std::string maSach;

    // Hàm tạo ngôi sao chuẩn để Scale từ tâm
    sf::ConvexShape createStar(float x, float y, float radius) {
        sf::ConvexShape star;
        star.setPointCount(10); 

        float angle = -3.14159 / 2; 
        float step = 3.14159 / 5;   
        float innerRadius = radius * 0.45f; 

        for (int i = 0; i < 10; i++) {
            float r = (i % 2 == 0) ? radius : innerRadius;
            
            //  Tính toán điểm dựa trên gốc (0,0) thay vì (x,y)
            star.setPoint(i, sf::Vector2f(
                std::cos(angle) * r,
                std::sin(angle) * r
            ));
            angle += step;
        }
        
        // Đặt vị trí sau khi tạo hình
        star.setPosition(x, y);

        star.setOutlineThickness(2);
        star.setOutlineColor(Theme::Primary); 
        return star;
    }

public:
    RatingBookScreen(sf::Font &f, Modal *m) : font(f), modal(m), currentRating(0) {
        panel.setSize({600, 450}); 
        panel.setCornerRadius(20.0f); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(350, 135); 

        title.setFont(font); 
        title.setString("DANH GIA SACH");
        title.setCharacterSize(30); 
        title.setStyle(sf::Text::Bold);
        title.setFillColor(Theme::Primary); 
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.width / 2, 0);
        title.setPosition(650, 165); 

        message.setFont(font); 
        message.setString("Ban cam thay cuon sach nay the nao?");
        message.setCharacterSize(20); 
        message.setFillColor(Theme::TextLight); 
        sf::FloatRect msgBounds = message.getLocalBounds();
        message.setOrigin(msgBounds.width / 2, 0);
        message.setPosition(650, 220);

        float startX = 490.0f; 
        float gap = 80.0f;     
        float radius = 30.0f;  

        for (int i = 0; i < 5; i++) {
            starShapes[i] = createStar(startX + i * gap, 320.0f, radius);
            starShapes[i].setFillColor(sf::Color(220, 220, 220)); 
        }

        btnSubmit = new Button({450, 390}, {200, 50}, "Gui Danh Gia", font, 101, Theme::Success);
        btnCancel = new Button({680, 390}, {150, 50}, "Huy", font, 102, sf::Color(150, 150, 150));
        
        highlightStars();
    }

    ~RatingBookScreen() { 
        delete btnSubmit; 
        delete btnCancel; 
    }

    void setExistingRating(int score) {
        currentRating = (score == 0) ? 0 : (score / 2);
        highlightStars();
        if(currentRating > 0) {
            title.setString("DANH GIA LAI");
            message.setString("Diem cu: " + std::to_string(score) + "/10. Ban muon thay doi?");
        } else {
            title.setString("DANH GIA SACH");
            message.setString("Ban cam thay cuon sach nay the nao?");
        }
        sf::FloatRect tB = title.getLocalBounds();
        title.setOrigin(tB.width/2, 0);
        sf::FloatRect mB = message.getLocalBounds();
        message.setOrigin(mB.width/2, 0);
    }

    void update(sf::Vector2f mousePos) {
        for(int i=0; i<5; i++) {
            if (starShapes[i].getGlobalBounds().contains(mousePos)) {
                starShapes[i].setScale(1.1f, 1.1f); 
            } else {
                starShapes[i].setScale(1.0f, 1.0f);
            }
        }
        btnSubmit->update(mousePos); 
        btnCancel->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        for (int i = 0; i < 5; i++) {
            if (starShapes[i].getGlobalBounds().contains(mousePos)) { 
                currentRating = i + 1; 
                highlightStars(); 
                return 0; 
            }
        }
        if (btnSubmit->handleClick(mousePos))
            return 1; 
        if (btnCancel->handleClick(mousePos))
            return 2; 
        return 0;
    }

    void highlightStars() {
        sf::Color activeColor(255, 215, 0); 
        sf::Color inactiveColor(220, 220, 220); 

        for (int i = 0; i < 5; i++) {
            if (i < currentRating) {
                starShapes[i].setFillColor(activeColor); 
                starShapes[i].setOutlineColor(sf::Color(218, 165, 32)); 
            } else {
                starShapes[i].setFillColor(inactiveColor); 
                starShapes[i].setOutlineColor(sf::Color(150, 150, 150)); 
            }
        }
    }

    int getScore() const { return (currentRating == 0) ? 0 : (currentRating * 2); } 
    
    void setBookID(const std::string& id) { 
        maSach = id; 
        currentRating = 0; 
        title.setString("DANH GIA SACH");
        message.setString("Ban cam thay cuon sach nay the nao?");
        sf::FloatRect tB = title.getLocalBounds();
        title.setOrigin(tB.width/2, 0);
        sf::FloatRect mB = message.getLocalBounds();
        message.setOrigin(mB.width/2, 0);
        highlightStars(); 
    }
    
    std::string getBookID() const { return maSach; }

    void render(sf::RenderWindow &window) {
        if (modal && modal->isShown()) {
            modal->draw(window); 
            window.draw(panel); 
            window.draw(title); 
            window.draw(message);
            for(int i=0; i<5; i++) 
                window.draw(starShapes[i]);
            btnSubmit->draw(window); 
            btnCancel->draw(window);
        }
    }
};

#endif
