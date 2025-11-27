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
    sf::ConvexShape starShape;
    sf::Text hotTag;
    
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;
    bool isHotBook;
    
    sf::Texture bookTexture; 

    // Helper: Cắt chuỗi an toàn
    std::string truncate(std::string str, size_t width) {
        if (str.length() > width) return str.substr(0, width - 3) + "...";
        return str;
    }

    // Helper: Vẽ ngôi sao
    void createStar(float x, float y, float size) {
        starShape.setPointCount(10);
        starShape.setFillColor(sf::Color(255, 193, 7)); // Màu Vàng
        
        float angle = -3.14159 / 2;
        float step = 3.14159 / 5;
        
        for (int i = 0; i < 10; i++) {
            float r = (i % 2 == 0) ? size : size / 2.2f;
            starShape.setPoint(i, sf::Vector2f(x + cos(angle) * r, y + sin(angle) * r));
            angle += step;
        }
    }

public:
    Card(sf::Vector2f position, sf::Vector2f size, const std::string& id,
         const std::string& imagePath,
         const std::string& title, const std::string& author, 
         double rating, sf::Color coverColor, sf::Font& font, bool isHot = false) {
        
        bookId = id;
        originalPos = position;
        isHotBook = isHot;
        
        // 1. Khung thẻ
        cardShape.setSize(size);
        cardShape.setCornerRadius(10.0f);
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color::White);
        cardShape.setOutlineThickness(1);
        cardShape.setOutlineColor(Theme::Border);
        isHovered = false;

        // 2. Ảnh bìa (Chiếm 55% chiều cao để chừa chỗ cho chữ)
        float coverHeight = size.y * 0.55f; 
        coverImage.setSize(sf::Vector2f(size.x - 20, coverHeight));
        coverImage.setPosition(position.x + 10, position.y + 10);

        bool loadSuccess = false;
        if (!imagePath.empty()) {
            if (bookTexture.loadFromFile(imagePath)) {
                loadSuccess = true;
                bookTexture.setSmooth(true);
                coverImage.setTexture(&bookTexture);
                coverImage.setFillColor(sf::Color::White); 
            }
        }
        if (!loadSuccess) {
            coverImage.setTexture(nullptr);
            coverImage.setFillColor(coverColor);
        }

        // [SỬA LỖI TRÀN CHỮ] 3. Tiêu đề sách
        float titleY = position.y + coverHeight + 12; // Cách ảnh bìa 12px
        
        titleText.setFont(font);
        
        // Cắt chuỗi nghiêm ngặt: Tối đa 28 ký tự
        std::string displayTitle = truncate(title, 28); 
        
        // Tự động xuống dòng nếu dài hơn 14 ký tự
        if (displayTitle.length() > 14) {
             size_t space = displayTitle.find_last_of(' ', 14); // Tìm dấu cách gần giữa
             if (space != std::string::npos) {
                 displayTitle.insert(space + 1, "\n");
             } else {
                 // Nếu không có dấu cách (tên dính liền), ép xuống dòng ở ký tự 14
                 displayTitle.insert(14, "\n");
             }
        }
        
        titleText.setString(displayTitle);
        titleText.setCharacterSize(14); // Giảm size chữ xuống 14 cho an toàn
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(position.x + 12, titleY);

        // [CỐ ĐỊNH VỊ TRÍ] 4. Hàng cuối (Tác giả & Điểm) - Đẩy sát đáy
        // Luôn nằm ở đáy thẻ trừ đi 25px
        float bottomLineY = position.y + size.y - 25; 

        // --- Điểm số (Bên Phải) ---
        if (rating > 10.0) rating = 10.0;
        ratingText.setFont(font);
        std::stringstream ss; 
        ss << std::fixed << std::setprecision(1) << rating;
        ratingText.setString(ss.str()); // Chỉ hiện số (vd: 8.5) cho gọn, bỏ "/10" nếu cần
        
        ratingText.setCharacterSize(13);
        ratingText.setStyle(sf::Text::Bold);
        ratingText.setFillColor(sf::Color(255, 160, 0)); // Màu cam
        
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        // Căn phải: Vị trí X = (X thẻ + Rộng thẻ) - (Rộng chữ) - Padding
        float ratingX = position.x + size.x - rateBounds.width - 12;
        ratingText.setPosition(ratingX, bottomLineY - 6); 

        // Ngôi sao nằm bên trái điểm số
        createStar(ratingX - 12, bottomLineY, 6.0f);

        // --- Tác giả (Bên Trái) ---
        // Giới hạn tên tác giả cực ngắn (12 ký tự) để không đâm vào ngôi sao
        std::string shortAuthor = truncate(author, 12);
        
        authorText.setFont(font);
        authorText.setString(shortAuthor); 
        authorText.setCharacterSize(12); // Chữ tác giả nhỏ và nhạt hơn
        authorText.setFillColor(sf::Color(120, 120, 120)); 
        authorText.setPosition(position.x + 12, bottomLineY - 6);
        
        // 5. Tag HOT
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
        
        // Cập nhật vị trí các thành phần theo khung
        float coverHeight = size.y * 0.55f; 
        coverImage.setPosition(pos.x + 10, pos.y + 10);
        
        titleText.setPosition(pos.x + 12, pos.y + coverHeight + 12);
        
        float bottomLineY = pos.y + size.y - 25;
        
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        float ratingX = pos.x + size.x - rateBounds.width - 12;
        ratingText.setPosition(ratingX, bottomLineY - 6);
        
        createStar(ratingX - 12, bottomLineY, 6.0f);
        authorText.setPosition(pos.x + 12, bottomLineY - 6);
        
        if (isHotBook) hotTag.setPosition(pos.x + size.x - 30, pos.y + 5);

        // Hover Effect
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
        window.draw(starShape);
        window.draw(ratingText);
        if (isHotBook) window.draw(hotTag);
    }

    bool isClicked(sf::Vector2f mousePos) { 
        return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left); 
    }
    std::string getBookId() const { return bookId; }
};

#endif