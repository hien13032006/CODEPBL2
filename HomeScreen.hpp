#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "InputField.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include"USER.h"

class HomeScreen {
private:
    sf::RectangleShape background;
    sf::Text titleTop10;
    sf::Text titleAllBooks;
    Sidebar* sidebar;
    InputField* searchBox;
    std::vector<Card*> top10Cards;
    std::vector<Card*> allBookCards;
    LibrarySystem* libSystem;
    ScrollView* scrollView;

public:
    HomeScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleTop10.setFont(font);
        titleTop10.setString("Top 10 Sach Noi Bat");
        titleTop10.setCharacterSize(28);
        titleTop10.setFillColor(sf::Color::White);
        titleTop10.setPosition(280, 30);

        titleAllBooks.setFont(font);
        titleAllBooks.setString("Tat Ca Sach");
        titleAllBooks.setCharacterSize(28);
        titleAllBooks.setFillColor(sf::Color::White);
        titleAllBooks.setPosition(280, 320);

        sidebar = new Sidebar(font);
        searchBox = new InputField(sf::Vector2f(280, 80), sf::Vector2f(400, 45), 
                                   "Tim kiem sach...", font);

        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadBooks(font);
    }

    ~HomeScreen() {
        delete sidebar;
        delete searchBox;
        delete scrollView;
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
    }

    void loadBooks(sf::Font& font) {
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
        top10Cards.clear();
        allBookCards.clear();

        if (!libSystem) return;

        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60),
            sf::Color(140, 80, 180), sf::Color(220, 100, 140),
            sf::Color(80, 180, 200), sf::Color(180, 140, 80),
            sf::Color(100, 120, 180), sf::Color(200, 100, 100)
        };

        // TODO: Lấy Top 10 từ libSystem->XepHangSach()
        // Tạm thời lấy 10 sách đầu
        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        int count = 0;

        while (current != nullptr && count < 10) {
            Card* card = new Card(
                sf::Vector2f(cardX, 130),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[count % 10],
                font
            );
            top10Cards.push_back(card);
            
            cardX += 200;
            if (count == 4) cardX = 280;
            count++;
            current = current->next;
        }

        // Tất cả sách
        current = libSystem->getDanhSachSach();
        cardX = 280;
        float cardY = 420;
        int col = 0;

        while (current != nullptr) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardY),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[col % 10],
                font
            );
            allBookCards.push_back(card);

            col++;
            if (col == 5) {
                col = 0;
                cardX = 280;
                cardY += 270;
            } else {
                cardX += 200;
            }
            current = current->next;
        }

        scrollView->setMaxScroll(std::max(0.0f, cardY - 600.0f));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchBox->update();
        for (auto card : top10Cards) card->update(mousePos);
        for (auto card : allBookCards) card->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleSearchEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchBox->handleEvent(event, mousePos);
    }

    void handleScrollEvent(sf::Event& event) {
        scrollView->handleScroll(event);
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        for (auto card : top10Cards) {
            if (card->isClicked(mousePos)) {
                return card->getBookId();
            }
        }
        for (auto card : allBookCards) {
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
        window.draw(titleTop10);
        searchBox->draw(window);
        
        for (auto card : top10Cards) card->draw(window);
        
        window.draw(titleAllBooks);
        for (auto card : allBookCards) card->draw(window);
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif