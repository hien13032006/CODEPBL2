#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "RoundedRectangle.hpp"

class Card {
private:
    RoundedRectangleShape cardShape;
    sf::RectangleShape coverImage;
    sf::Text titleText;
    sf::Text authorText;
    sf::Text ratingText;
    sf::Text hotTag;
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;
    bool isHotBook;

public:
    Card(sf::Vector2f position, sf::Vector2f size, const std::string& id,
         const std::string& title, const std::string& author, 
         double rating, sf::Color coverColor, sf::Font& font, bool isHot = false) {
        
        bookId = id;
        originalPos = position;
        isHotBook = isHot;
        
        // 1. Khung thẻ
        cardShape.setSize(size);
        cardShape.setCornerRadius(8.0f);
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color(30, 33, 45)); // Màu nền tối
        
        isHovered = false;

        // 2. Ảnh bìa (Chiếm 62% chiều cao)
        coverImage.setSize(sf::Vector2f(size.x - 20, size.y * 0.62));
        coverImage.setPosition(position.x + 10, position.y + 10);
        coverImage.setFillColor(coverColor);

        // --- CẤU HÌNH CHỮ (NORMAL FONT) ---
        
        // Tiêu đề: Size 18, Trắng
        titleText.setFont(font);
        std::string displayTitle = title.length() > 35 ? title.substr(0, 32) + "..." : title;
        // Tự động xuống dòng
        if (displayTitle.length() > 16 && displayTitle.find('\n') == std::string::npos) {
             size_t space = displayTitle.find_last_of(' ', 16);
             if (space != std::string::npos) displayTitle.insert(space + 1, "\n");
        }
        titleText.setString(displayTitle);
        titleText.setCharacterSize(17);
        titleText.setFillColor(sf::Color::White);
        // Ép kiểu int để chữ sắc nét
        titleText.setPosition((int)(position.x + 12), (int)(position.y + size.y * 0.62 + 20));

        // Tác giả: Size 14, Xám
        authorText.setFont(font);
        std::string displayAuthor = author.length() > 22 ? author.substr(0, 19) + "..." : author;
        authorText.setString(displayAuthor);
        authorText.setCharacterSize(14);
        authorText.setFillColor(sf::Color(180, 180, 180));
        authorText.setPosition((int)(position.x + 12), (int)(position.y + size.y - 40));

        // Rating: Size 14, Vàng
        ratingText.setFont(font);
        char ratingStr[20]; sprintf(ratingStr, "%.1f/10", rating);
        ratingText.setString(ratingStr);
        ratingText.setCharacterSize(12);
        ratingText.setFillColor(sf::Color(255, 193, 94));
        ratingText.setPosition((int)(position.x + size.x - 60), (int)(position.y + size.y - 45));
        
        // Tag HOT (Nhỏ gọn)
        hotTag.setFont(font);
        hotTag.setString("HOT");
        hotTag.setCharacterSize(11);
        hotTag.setFillColor(sf::Color::White);
        hotTag.setOutlineColor(sf::Color(220, 50, 50));
        hotTag.setOutlineThickness(1.5f);
        hotTag.setPosition((int)(position.x + size.x - 38), (int)(position.y + 8));
    }

    void update(sf::Vector2f mousePos, float scrollOffsetX = 0, float scrollOffsetY = 0) {
        // Tính toán vị trí thực tế
        float actualX = std::floor(originalPos.x - scrollOffsetX);
        float actualY = std::floor(originalPos.y - scrollOffsetY);
        
        cardShape.setPosition(actualX, actualY);
        sf::Vector2f pos = cardShape.getPosition();
        sf::Vector2f size = cardShape.getSize();
        
        coverImage.setPosition((int)(pos.x + 10), (int)(pos.y + 10));
        titleText.setPosition((int)(pos.x + 12), (int)(pos.y + size.y * 0.62 + 20));
        authorText.setPosition((int)(pos.x + 12), (int)(pos.y + size.y - 45));
        ratingText.setPosition((int)(pos.x + size.x - 60), (int)(pos.y + size.y - 45));
        hotTag.setPosition((int)(pos.x + size.x - 38), (int)(pos.y + 8));

        if (cardShape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                cardShape.setOutlineThickness(1.5f);
                cardShape.setOutlineColor(sf::Color(100, 150, 255));
            }
        } else {
            isHovered = false;
            cardShape.setOutlineThickness(0);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(cardShape);
        window.draw(coverImage);
        window.draw(titleText);
        window.draw(authorText);
        window.draw(ratingText);
        if (isHotBook) window.draw(hotTag);
    }

    bool isClicked(sf::Vector2f mousePos) { 
        return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left); 
    }
    std::string getBookId() const { return bookId; }
};

#endif