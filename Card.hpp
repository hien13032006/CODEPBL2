#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class Card {
private:
    RoundedRectangleShape cardShape;
    sf::RectangleShape coverImage;
    sf::Text titleText;
    sf::Text authorText;
    sf::Text ratingText;
    sf::ConvexShape starShape; // Vẽ ngôi sao bằng hình học
    sf::Text hotTag;
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;
    bool isHotBook;

    // Helper: Cắt chuỗi thông minh
    std::string truncate(std::string str, size_t width) {
        if (str.length() > width) return str.substr(0, width - 3) + "...";
        return str;
    }

    // Helper: Tạo hình ngôi sao
    void createStar(float x, float y, float size) {
        starShape.setPointCount(10);
        starShape.setFillColor(sf::Color(255, 193, 7)); // Màu vàng hổ phách
        
        float angle = -3.14159 / 2; // Bắt đầu từ đỉnh trên cùng
        float step = 3.14159 / 5;   // Bước nhảy góc
        
        for (int i = 0; i < 10; i++) {
            // Bán kính: Đỉnh nhọn (size), Đỉnh lõm (size / 2)
            float r = (i % 2 == 0) ? size : size / 2.2f; 
            starShape.setPoint(i, sf::Vector2f(
                x + cos(angle) * r,
                y + sin(angle) * r
            ));
            angle += step;
        }
    }

public:
    Card(sf::Vector2f position, sf::Vector2f size, const std::string& id,
         const std::string& title, const std::string& author, 
         double rating, sf::Color coverColor, sf::Font& font, bool isHot = false) {
        
        bookId = id;
        originalPos = position;
        isHotBook = isHot;
        
        // 1. Khung thẻ
        cardShape.setSize(size);
        cardShape.setCornerRadius(12.0f);
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color::White);
        cardShape.setOutlineThickness(1);
        cardShape.setOutlineColor(Theme::Border);
        isHovered = false;

        // 2. Ảnh bìa (Giảm xuống 55% để dành chỗ cho chữ)
        float coverHeight = size.y * 0.55f; 
        coverImage.setSize(sf::Vector2f(size.x - 20, coverHeight));
        coverImage.setPosition(position.x + 10, position.y + 10);
        coverImage.setFillColor(coverColor);

        // 3. Tiêu đề sách (Title) - Đẩy lên sát ảnh bìa
        titleText.setFont(font);
        // Cho phép dài hơn chút nhưng cắt gọn
        std::string displayTitle = truncate(title, 35);
        
        // Xử lý xuống dòng thủ công để không bị tràn ngang
        if (displayTitle.length() > 17 && displayTitle.find('\n') == std::string::npos) {
             size_t space = displayTitle.find_last_of(' ', 17);
             if (space != std::string::npos) displayTitle.insert(space + 1, "\n");
        }

        titleText.setString(displayTitle);
        titleText.setCharacterSize(15); // Giảm size chữ xuống 15 cho gọn
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(position.x + 12, position.y + coverHeight + 15);

        // 4. Tác giả (Author) - Neo vị trí gần đáy
        authorText.setFont(font);
        authorText.setString(truncate(author, 22));
        authorText.setCharacterSize(13); // Chữ nhỏ hơn tiêu đề
        authorText.setFillColor(sf::Color(100, 100, 100));
        // Đặt vị trí cố định cách đáy 45px (để chừa chỗ cho rating)
        authorText.setPosition(position.x + 12, position.y + size.y - 50);

        // 5. Đánh giá (Rating) - Neo dưới cùng bên phải
        // Tạo ngôi sao
        createStar(position.x + size.x - 65, position.y + size.y - 18, 7.0f);
        
        ratingText.setFont(font);
        std::stringstream ss; 
        ss << std::fixed << std::setprecision(1) << rating;
        ratingText.setString(ss.str()); // Chỉ hiện số
        ratingText.setCharacterSize(13);
        ratingText.setStyle(sf::Text::Bold);
        ratingText.setFillColor(sf::Color(255, 160, 0)); // Màu cam đậm
        // Số nằm bên phải ngôi sao
        ratingText.setPosition(position.x + size.x - 55, position.y + size.y - 26);
        
        // 6. Tag HOT
        if (isHotBook) {
            hotTag.setFont(font);
            hotTag.setString("HOT");
            hotTag.setCharacterSize(10);
            hotTag.setStyle(sf::Text::Bold);
            hotTag.setFillColor(sf::Color::White);
            hotTag.setOutlineColor(Theme::Primary);
            hotTag.setOutlineThickness(3); 
            hotTag.setPosition(position.x + size.x - 30, position.y + 5);
        }
    }

    void update(sf::Vector2f mousePos, float scrollOffsetX = 0, float scrollOffsetY = 0) {
        float actualX = std::floor(originalPos.x - scrollOffsetX);
        float actualY = std::floor(originalPos.y - scrollOffsetY);
        
        cardShape.setPosition(actualX, actualY);
        
        sf::Vector2f pos = cardShape.getPosition();
        sf::Vector2f size = cardShape.getSize();
        float coverHeight = size.y * 0.55f;
        
        coverImage.setPosition(pos.x + 10, pos.y + 10);
        titleText.setPosition(pos.x + 12, pos.y + coverHeight + 15);
        authorText.setPosition(pos.x + 12, pos.y + size.y - 50);
        
        // Cập nhật vị trí ngôi sao và điểm số
        createStar(pos.x + size.x - 65, pos.y + size.y - 18, 7.0f);
        ratingText.setPosition(pos.x + size.x - 55, pos.y + size.y - 26);
        
        if (isHotBook) hotTag.setPosition(pos.x + size.x - 30, pos.y + 5);

        if (cardShape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                cardShape.setOutlineThickness(2.0f);
                cardShape.setOutlineColor(Theme::Secondary);
            }
        } else {
            if (isHovered) {
                isHovered = false;
                cardShape.setOutlineThickness(1);
                cardShape.setOutlineColor(Theme::Border);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(cardShape);
        window.draw(coverImage);
        window.draw(titleText);
        window.draw(authorText);
        window.draw(starShape); // Vẽ hình ngôi sao
        window.draw(ratingText);
        if (isHotBook) window.draw(hotTag);
    }

    bool isClicked(sf::Vector2f mousePos) { 
        return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left); 
    }
    std::string getBookId() const { return bookId; }
};

#endif