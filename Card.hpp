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
    
    // Biến lưu trữ ảnh (Phải là thành viên class để không bị mất khi ra khỏi hàm)
    sf::Texture bookTexture; 

    // Helper: Cắt chuỗi nếu quá dài
    std::string truncate(std::string str, size_t width) {
        if (str.length() > width) return str.substr(0, width - 3) + "...";
        return str;
    }

    // Helper: Vẽ hình ngôi sao
    void createStar(float x, float y, float size) {
        starShape.setPointCount(10);
        starShape.setFillColor(sf::Color(255, 193, 7)); // Màu vàng hổ phách
        
        float angle = -3.14159 / 2; // Bắt đầu từ đỉnh trên
        float step = 3.14159 / 5;   // Bước nhảy góc
        
        for (int i = 0; i < 10; i++) {
            float r = (i % 2 == 0) ? size : size / 2.2f; // Bán kính trong/ngoài
            starShape.setPoint(i, sf::Vector2f(
                x + cos(angle) * r,
                y + sin(angle) * r
            ));
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
        cardShape.setCornerRadius(12.0f);
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color::White);
        cardShape.setOutlineThickness(1);
        cardShape.setOutlineColor(Theme::Border);
        isHovered = false;

        // 2. Ảnh bìa (Chiếm 58% chiều cao)
        float coverHeight = size.y * 0.58f; 
        coverImage.setSize(sf::Vector2f(size.x - 20, coverHeight));
        coverImage.setPosition(position.x + 10, position.y + 10);

        // [QUAN TRỌNG] Xử lý load ảnh
        bool loadSuccess = false;
        if (!imagePath.empty()) {
            // Thử load ảnh từ đường dẫn
            if (bookTexture.loadFromFile(imagePath)) {
                loadSuccess = true;
                bookTexture.setSmooth(true); // Làm mịn ảnh
                coverImage.setTexture(&bookTexture);
                // Reset màu về Trắng để hiển thị đúng màu gốc của ảnh (không bị ám màu nền)
                coverImage.setFillColor(sf::Color::White); 
            }
        }

        // Nếu không có ảnh hoặc lỗi -> Dùng màu nền mặc định (coverColor)
        if (!loadSuccess) {
            coverImage.setTexture(nullptr);
            coverImage.setFillColor(coverColor);
        }

        // 3. Tiêu đề sách
        titleText.setFont(font);
        // Cắt ngắn tiêu đề (khoảng 2 dòng)
        std::string displayTitle = truncate(title, 35);
        // Xử lý xuống dòng thủ công để đẹp hơn
        if (displayTitle.length() > 17 && displayTitle.find('\n') == std::string::npos) {
             size_t space = displayTitle.find_last_of(' ', 17);
             if (space != std::string::npos) displayTitle.insert(space + 1, "\n");
        }
        titleText.setString(displayTitle);
        titleText.setCharacterSize(15); 
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(position.x + 12, position.y + coverHeight + 15);

        // --- CẤU HÌNH HÀNG CUỐI CÙNG (Tác giả & Điểm) ---
        float bottomLineY = position.y + size.y - 25; 

        // 4. Đánh giá (Rating) - Căn phải
        // Xử lý logic điểm số (Max 10)
        if (rating > 10.0) rating = 10.0;

        ratingText.setFont(font);
        std::stringstream ss; 
        ss << std::fixed << std::setprecision(1) << rating;
        ratingText.setString(ss.str() + "/10"); 
        
        ratingText.setCharacterSize(12);
        ratingText.setStyle(sf::Text::Bold);
        ratingText.setFillColor(sf::Color(255, 160, 0)); // Màu cam đậm
        
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        float ratingX = position.x + size.x - rateBounds.width - 10;
        ratingText.setPosition(ratingX, bottomLineY - 5); 

        // Vẽ ngôi sao bên trái điểm số
        createStar(ratingX - 12, bottomLineY + 2, 6.0f);

        // 5. Tác giả (Author) - Căn trái
        authorText.setFont(font);
        // Cắt ngắn 18 ký tự để không đè vào điểm số
        authorText.setString(truncate(author, 18)); 
        authorText.setCharacterSize(12);
        authorText.setFillColor(sf::Color(120, 120, 120)); // Màu xám
        authorText.setPosition(position.x + 12, bottomLineY - 5);
        
        // 6. Tag HOT (Góc trên phải)
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
        // Tính toán vị trí thực tế khi cuộn
        float actualX = std::floor(originalPos.x - scrollOffsetX);
        float actualY = std::floor(originalPos.y - scrollOffsetY);
        
        cardShape.setPosition(actualX, actualY);
        sf::Vector2f pos = cardShape.getPosition();
        sf::Vector2f size = cardShape.getSize();
        float coverHeight = size.y * 0.58f;
        
        // Cập nhật vị trí các thành phần con theo khung
        coverImage.setPosition(pos.x + 10, pos.y + 10);
        titleText.setPosition(pos.x + 12, pos.y + coverHeight + 15);
        
        float bottomLineY = pos.y + size.y - 25;
        
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        float ratingX = pos.x + size.x - rateBounds.width - 10;
        ratingText.setPosition(ratingX, bottomLineY - 5);
        
        createStar(ratingX - 12, bottomLineY + 2, 6.0f);
        authorText.setPosition(pos.x + 12, bottomLineY - 5);
        
        if (isHotBook) hotTag.setPosition(pos.x + size.x - 30, pos.y + 5);

        // Hiệu ứng Hover
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