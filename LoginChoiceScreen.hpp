#ifndef LOGIN_CHOICE_SCREEN_HPP
#define LOGIN_CHOICE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"

class LoginChoiceScreen {
private:
    Modal* modal;
    sf::Text titleText;
    sf::Text descText;
    Button* librarianButton;
    Button* readerButton;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    Button btnClose;
    bool closed;

    sf::Font& font;

public:
    LoginChoiceScreen(sf::Font& f, Modal* modalRef)
        : font(f),
          modal(modalRef),
          btnClose({1190, 150}, {40, 40}, "X", f),
          closed(false)
    {
        // Load background
        if (!bgTexture.loadFromFile("picChoice1.png")) {
            std::cout << "Khong the load hinh nen!\n";
        }
        bgSprite.setTexture(bgTexture);
        bgSprite.setPosition(280, 150);

        // Title
        titleText.setFont(font);
        titleText.setString("Chon Loai Nguoi Dung");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::Black);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(715, 220);

        // Description
        descText.setFont(font);
        descText.setString("Ban la?");
        descText.setCharacterSize(24);
        descText.setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect descBounds = descText.getLocalBounds();
        descText.setOrigin(descBounds.width / 2, descBounds.height / 2);
        descText.setPosition(725, 275);

        // Buttons
        librarianButton = new Button(
            sf::Vector2f(830, 500),
            sf::Vector2f(220, 60),
            "Thu Thu",
            font, 0,
            sf::Color(100, 150, 255)
        );

        readerButton = new Button(
            sf::Vector2f(480, 500),
            sf::Vector2f(220, 60),
            "Doc Gia",
            font, 0,
            sf::Color(255, 150, 100)
        );
    }

    ~LoginChoiceScreen() {
        delete librarianButton;
        delete readerButton;
    }

    bool isClosed() const { return closed; }

    void handleEvent(sf::Event& e, const sf::Vector2f& mousePos) {
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) {
                closed = true;
            }
        }
    }

    void update(sf::Vector2f mousePos) {
        librarianButton->update(mousePos);
        readerButton->update(mousePos);
        btnClose.update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (librarianButton->handleClick(mousePos)) return 1;
        if (readerButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(bgSprite);
            window.draw(titleText);
            window.draw(descText);

            librarianButton->draw(window);
            readerButton->draw(window);
            btnClose.draw(window);
        }
    }
};

#endif
