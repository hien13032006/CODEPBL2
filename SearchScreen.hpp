#ifndef SEARCH_SCREEN_HPP
#define SEARCH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "Card.hpp"
#include "LibrarySystem.h"

class SearchScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text resultText;
    Sidebar* sidebar;
    InputField* searchField;
    Button* searchButton;
    std::vector<Card*> resultCards;
    LibrarySystem* libSystem;

public:
    SearchScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Tim Kiem Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        resultText.setFont(font);
        resultText.setString("Nhap tu khoa de tim kiem...");
        resultText.setCharacterSize(16);
        resultText.setFillColor(sf::Color(150, 150, 150));
        resultText.setPosition(280, 200);

        sidebar = new Sidebar(font);

        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(600, 50),
                                     "Ten sach, tac gia, the loai...", font);
        searchButton = new Button(sf::Vector2f(900, 100), sf::Vector2f(150, 50),
                                 "Tim Kiem", font);
    }

    ~SearchScreen() {
        delete sidebar;
        delete searchField;
        delete searchButton;
        for (auto card : resultCards) delete card;
    }

    void performSearch(sf::Font& font) {
        // Xóa kết quả cũ
        for (auto card : resultCards) delete card;
        resultCards.clear();

        std::string keyword = searchField->getText();
        if (keyword.empty()) {
            resultText.setString("Vui long nhap tu khoa!");
            return;
        }

        // TODO: Gọi libSystem->TimSach(keyword);
        // BuildHashTable trước nếu chưa có

        // Tạm thời dùng dữ liệu mẫu
        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60)
        };

        float cardX = 280;
        float cardY = 250;
        int col = 0;

        for (int i = 0; i < 8; i++) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardY),
                sf::Vector2f(180, 250),
                "SEARCH" + std::to_string(i),
                "Ket qua " + std::to_string(i+1),
                "Tac Gia",
                "2020",
                7.5f,
                colors[i % 4],
                font
            );
            resultCards.push_back(card);

            col++;
            if (col == 5) {
                col = 0;
                cardX = 280;
                cardY += 270;
            } else {
                cardX += 200;
            }
        }

        resultText.setString("Tim thay " + std::to_string(resultCards.size()) + " ket qua");
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchField->update();
        searchButton->update(mousePos);
        for (auto card : resultCards) card->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
    }

    bool handleSearchClick(sf::Vector2f mousePos, sf::Font& font) {
        if (searchButton->handleClick(mousePos)) {
            performSearch(font);
            return true;
        }
        return false;
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        for (auto card : resultCards) {
            if (card->isClicked(mousePos)) {
                return card->getBookId();
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        searchField->draw(window);
        searchButton->draw(window);
        window.draw(resultText);

        for (auto card : resultCards) {
            card->draw(window);
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif