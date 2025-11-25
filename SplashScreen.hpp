#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Theme.hpp"

class SplashScreen {
private:
    sf::RectangleShape background, card; sf::Text logoText, titleText, descText; Button* startButton;
public:
    SplashScreen(sf::Font& font) {
        background.setSize(sf::Vector2f(1300, 720)); background.setFillColor(Theme::Background);
        card.setSize(sf::Vector2f(600, 400)); card.setPosition(350, 160); card.setFillColor(sf::Color::White); card.setOutlineThickness(2); card.setOutlineColor(Theme::Primary);
        logoText.setFont(font); logoText.setString("Library"); logoText.setCharacterSize(60); logoText.setFillColor(Theme::Primary); sf::FloatRect logoBounds = logoText.getLocalBounds(); logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2); logoText.setPosition(650, 260);
        titleText.setFont(font); titleText.setString("Quan Ly Thu Vien"); titleText.setCharacterSize(32); titleText.setFillColor(Theme::TextDark); sf::FloatRect titleBounds = titleText.getLocalBounds(); titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2); titleText.setPosition(650, 340);
        descText.setFont(font); descText.setString("He thong quan ly sach nhanh chong va hien dai"); descText.setCharacterSize(16); descText.setFillColor(Theme::TextLight); sf::FloatRect descBounds = descText.getLocalBounds(); descText.setOrigin(descBounds.width / 2, descBounds.height / 2); descText.setPosition(650, 390);
        startButton = new Button(sf::Vector2f(550, 440), sf::Vector2f(200, 50), "Xem Ngay", font, 0, Theme::Secondary);
    }
    ~SplashScreen() { delete startButton; }
    void update(sf::Vector2f mousePos) { startButton->update(mousePos); }
    bool handleClick(sf::Vector2f mousePos) { return startButton->handleClick(mousePos); }
    void render(sf::RenderWindow& window) { window.draw(background); window.draw(card); window.draw(logoText); window.draw(titleText); window.draw(descText); startButton->draw(window); }
};
#endif