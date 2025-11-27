#ifndef LOGIN_CHOICE_SCREEN_HPP
#define LOGIN_CHOICE_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class LoginChoiceScreen {
private:
    Modal* modal; RoundedRectangleShape panel; sf::Text titleText, descText;
    Button *librarianButton, *readerButton, btnClose; sf::Texture bgTexture; sf::Sprite bgSprite; bool closed; sf::Font& font;
public:
    LoginChoiceScreen(sf::Font& f, Modal* modalRef) : font(f), modal(modalRef), btnClose({1000, 130}, {50, 50}, "X", f, 0, Theme::Error), closed(false) {
        panel.setSize({800, 500}); panel.setCornerRadius(20.0f); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(250, 110);
        if (bgTexture.loadFromFile("images/picChoice1.png")) { bgSprite.setTexture(bgTexture); float scaleY = 500.0f / bgTexture.getSize().y; bgSprite.setScale(scaleY, scaleY); bgSprite.setPosition(250, 110); }
        titleText.setFont(font); titleText.setString("CHAO MUNG"); titleText.setCharacterSize(36); titleText.setFillColor(Theme::Primary); titleText.setPosition(700, 180);
        descText.setFont(font); descText.setString("Ban muon dang nhap voi tu cach?"); descText.setCharacterSize(18); descText.setFillColor(Theme::TextLight); descText.setPosition(680, 240);
        librarianButton = new Button({700, 300}, {250, 60}, "Thu Thu", font, 0, Theme::Primary); readerButton = new Button({700, 380}, {250, 60}, "Doc Gia", font, 0, Theme::Secondary);
    }
    ~LoginChoiceScreen() { delete librarianButton; delete readerButton; }
    void reset() { closed = false; } bool isClosed() const { return closed; }
    void handleEvent(sf::Event& e, const sf::Vector2f& mousePos) { if (e.type == sf::Event::MouseButtonPressed && btnClose.handleClick(mousePos)) closed = true; }
    void update(sf::Vector2f mousePos) { librarianButton->update(mousePos); readerButton->update(mousePos); btnClose.update(mousePos); }
    int handleClick(sf::Vector2f mousePos) { if (librarianButton->handleClick(mousePos)) return 1; if (readerButton->handleClick(mousePos)) return 2; return 0; }
    void render(sf::RenderWindow& window) { if (modal && modal->isShown()) { window.draw(panel); if (bgTexture.getSize().x > 0) window.draw(bgSprite); window.draw(titleText); window.draw(descText); librarianButton->draw(window); readerButton->draw(window); btnClose.draw(window); } }
};
#endif