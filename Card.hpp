#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "RoundedRectangle.hpp"
#include "Theme.hpp"
#include "ResourceManager.hpp"

class Card {
private:
    RoundedRectangleShape cardShape;
    sf::RectangleShape coverImage;
    sf::Sprite coverSprite;
    sf::Text titleText;
    sf::Text authorText;
    sf::Text ratingText;
    sf::ConvexShape starShape;
    sf::Text hotTag;
    
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;
    bool isHotBook;
    std::string truncate(std::string str, size_t width) {
        if (str.length() > width)
            return str.substr(0, width - 3) + "...";
        return str;
    }

    void createStar(float x, float y, float size) {
        starShape.setPointCount(10);
        starShape.setFillColor(sf::Color(255, 193, 7)); 
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
        
        cardShape.setSize(size);
        cardShape.setCornerRadius(10.0f);
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color::White);
        cardShape.setOutlineThickness(1);
        cardShape.setOutlineColor(Theme::Border);
        isHovered = false;

        float coverHeight = size.y * 0.55f; 
        // Dùng coverImage làm nền (placeholder)
        coverImage.setSize(sf::Vector2f(size.x - 20, coverHeight));
        coverImage.setPosition(position.x + 10, position.y + 10);
        coverImage.setFillColor(coverColor); 

        // Lấy texture từ Cache
        sf::Texture& tex = ResourceManager::getInstance()->getTexture(imagePath);
        coverSprite.setTexture(tex);
        
        // Scale ảnh fit vào khung
        float targetW = size.x - 20;
        float targetH = coverHeight;
        if (tex.getSize().x > 0) {
            float scaleX = targetW / tex.getSize().x;
            float scaleY = targetH / tex.getSize().y;
             coverSprite.setScale(scaleX, scaleY);
             coverSprite.setPosition(position.x + 10, position.y + 10);
        }

        // --- TIÊU ĐỀ ---
        float titleY = position.y + coverHeight + 12; 
        titleText.setFont(font);
        std::string displayTitle = truncate(title, 28); 
        if (displayTitle.length() > 14) {
             size_t space = displayTitle.find_last_of(' ', 14); 
             if (space != std::string::npos) displayTitle.insert(space + 1, "\n");
             else displayTitle.insert(14, "\n");
        }
        titleText.setString(displayTitle);
        titleText.setCharacterSize(14); 
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(position.x + 12, titleY);

        // --- CUỐI THẺ ---
        float bottomLineY = position.y + size.y - 25; 
        if (rating > 10.0) rating = 10.0;
        ratingText.setFont(font);
        std::stringstream ss; ss << std::fixed << std::setprecision(1) << rating;
        ratingText.setString(ss.str()); 
        ratingText.setCharacterSize(13);
        ratingText.setStyle(sf::Text::Bold);
        ratingText.setFillColor(sf::Color(255, 160, 0)); 
        
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        float ratingX = position.x + size.x - rateBounds.width - 12;
        ratingText.setPosition(ratingX, bottomLineY - 6); 
        createStar(ratingX - 12, bottomLineY, 6.0f);

        std::string shortAuthor = truncate(author, 12);
        authorText.setFont(font);
        authorText.setString(shortAuthor); 
        authorText.setCharacterSize(12); 
        authorText.setFillColor(sf::Color(120, 120, 120)); 
        authorText.setPosition(position.x + 12, bottomLineY - 6);
        
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
        coverSprite.setPosition(pos.x + 10, pos.y + 10); // Cập nhật vị trí sprite

        titleText.setPosition(pos.x + 12, pos.y + coverHeight + 12);
        float bottomLineY = pos.y + size.y - 25;
        sf::FloatRect rateBounds = ratingText.getLocalBounds();
        float ratingX = pos.x + size.x - rateBounds.width - 12;
        ratingText.setPosition(ratingX, bottomLineY - 6);
        createStar(ratingX - 12, bottomLineY, 6.0f);
        authorText.setPosition(pos.x + 12, bottomLineY - 6);
        if (isHotBook) 
            hotTag.setPosition(pos.x + size.x - 30, pos.y + 5);

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
        if (coverSprite.getTexture())
            window.draw(coverSprite);
        
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
