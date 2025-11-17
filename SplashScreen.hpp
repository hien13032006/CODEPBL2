#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"

class SplashScreen {
private:
    sf::RectangleShape background;
    sf::RectangleShape card;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text descText;
    Button* startButton;

public:
    SplashScreen(sf::Font& font) {
        background.setSize(sf::Vector2f(1400, 900));
        background.setFillColor(sf::Color(13, 15, 23));

        card.setSize(sf::Vector2f(600, 400));
        card.setPosition(400, 250);
        card.setFillColor(sf::Color(20, 23, 35));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(60);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(700, 350);

        titleText.setFont(font);
        titleText.setString("Quan Ly Thu Vien");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 430);

        descText.setFont(font);
        descText.setString("He thong quan ly sach nhanh chong va hien dai");
        descText.setCharacterSize(14);
        descText.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect descBounds = descText.getLocalBounds();
        descText.setOrigin(descBounds.width / 2, descBounds.height / 2);
        descText.setPosition(700, 480);

        startButton = new Button(sf::Vector2f(600, 530), sf::Vector2f(200, 50), 
                                "Xem Ngay", font);
    }

    ~SplashScreen() { delete startButton; }

    void update(sf::Vector2f mousePos) {
        startButton->update(mousePos);
    }

    bool handleClick(sf::Vector2f mousePos) {
        return startButton->handleClick(mousePos);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(card);
        window.draw(logoText);
        window.draw(titleText);
        window.draw(descText);
        startButton->draw(window);
    }
};

#endif