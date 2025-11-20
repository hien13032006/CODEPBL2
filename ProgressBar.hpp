#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <SFML/Graphics.hpp>

class ProgressBar {
private:
    sf::RectangleShape background;
    sf::RectangleShape fill;
    sf::Text percentText;
    float progress;

public:
    ProgressBar(sf::Font& font, sf::Vector2f position, sf::Vector2f size) {
        background.setSize(size);
        background.setPosition(position);
        background.setFillColor(sf::Color(40, 40, 40));
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color(80, 80, 80));

        fill.setSize(sf::Vector2f(0, size.y - 4));
        fill.setPosition(position.x + 2, position.y + 2);
        fill.setFillColor(sf::Color(100, 180, 100));

        percentText.setFont(font);
        percentText.setCharacterSize(14);
        percentText.setFillColor(sf::Color::White);
        
        progress = 0.0f;
    }

    void setProgress(float value) {
        progress = std::max(0.0f, std::min(100.0f, value));
        
        float width = (background.getSize().x - 4) * (progress / 100.0f);
        fill.setSize(sf::Vector2f(width, fill.getSize().y));

        percentText.setString(std::to_string((int)progress) + "%");
        sf::FloatRect textBounds = percentText.getLocalBounds();
        percentText.setPosition(
            background.getPosition().x + background.getSize().x / 2 - textBounds.width / 2,
            background.getPosition().y + background.getSize().y / 2 - textBounds.height / 2 - 3
        );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(fill);
        window.draw(percentText);
    }

    float getProgress() const { return progress; }
};

#endif