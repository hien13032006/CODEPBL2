// ============================================
// ui/Card.hpp - Thẻ sách với bo góc
// ============================================
#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"

class Card {
private:
    RoundedRectangleShape cardShape;
    sf::RectangleShape coverImage;
    sf::Text titleText;
    sf::Text authorText;
    sf::Text yearText;
    sf::Text ratingText;
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;

public:
    Card(sf::Vector2f position, sf::Vector2f size, const std::string& id,
         const std::string& title, const std::string& author, 
         const std::string& year, float rating, sf::Color coverColor, sf::Font& font) {
        
        bookId = id;
        
        cardShape.setSize(size);
        cardShape.setCornerRadius(12.0f); // Bo góc 12px
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color(25, 28, 40));
        
        originalPos = position;
        isHovered = false;

        coverImage.setSize(sf::Vector2f(size.x - 20, 160));
        coverImage.setPosition(position.x + 10, position.y + 10);
        coverImage.setFillColor(coverColor);

        titleText.setFont(font);
        std::string displayTitle = title.length() > 20 ? title.substr(0, 20) + "..." : title;
        titleText.setString(displayTitle);
        titleText.setCharacterSize(14);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(position.x + 15, position.y + 180);

        authorText.setFont(font);
        std::string displayAuthor = author.length() > 18 ? author.substr(0, 18) + "..." : author;
        authorText.setString(displayAuthor);
        authorText.setCharacterSize(12);
        authorText.setFillColor(sf::Color(150, 150, 150));
        authorText.setPosition(position.x + 15, position.y + 202);

        yearText.setFont(font);
        yearText.setString(year);
        yearText.setCharacterSize(11);
        yearText.setFillColor(sf::Color(100, 100, 100));
        yearText.setPosition(position.x + 15, position.y + 222);

        ratingText.setFont(font);
        char ratingStr[20];
        sprintf(ratingStr, "%.1f/10", rating);
        ratingText.setString(ratingStr);
        ratingText.setCharacterSize(12);
        ratingText.setFillColor(sf::Color(255, 193, 94));
        ratingText.setPosition(position.x + size.x - 50, position.y + 222);
    }

    void update(sf::Vector2f mousePos) {
        if (cardShape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                cardShape.setPosition(originalPos.x, originalPos.y - 8);
                updateTextPositions();
                cardShape.setOutlineThickness(2);
                cardShape.setOutlineColor(sf::Color(255, 193, 94, 150));
            }
        } else {
            if (isHovered) {
                isHovered = false;
                cardShape.setPosition(originalPos);
                updateTextPositions();
                cardShape.setOutlineThickness(0);
            }
        }
    }

    void updateTextPositions() {
        sf::Vector2f pos = cardShape.getPosition();
        coverImage.setPosition(pos.x + 10, pos.y + 10);
        titleText.setPosition(pos.x + 15, pos.y + 180);
        authorText.setPosition(pos.x + 15, pos.y + 202);
        yearText.setPosition(pos.x + 15, pos.y + 222);
        ratingText.setPosition(pos.x + cardShape.getSize().x - 50, pos.y + 222);
    }

    bool isClicked(sf::Vector2f mousePos) {
        return cardShape.getGlobalBounds().contains(mousePos);
    }

    std::string getBookId() const { return bookId; }

    void draw(sf::RenderWindow& window) {
        window.draw(cardShape);
        window.draw(coverImage);
        window.draw(titleText);
        window.draw(authorText);
        window.draw(yearText);
        window.draw(ratingText);
    }
};

#endif