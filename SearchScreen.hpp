#ifndef SEARCH_SCREEN_HPP
#define SEARCH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <locale>
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
        for (auto card : resultCards) delete card;
        resultCards.clear();

        std::string keyword = searchField->getText();
        if (keyword.empty()) {
            resultText.setString("Vui long nhap tu khoa!");
            resultText.setFillColor(sf::Color(255, 100, 100));
            return;
        }

        if (!libSystem) return;

        std::string keywordLower = keyword;
        // Chuyển từ khóa sang chữ thường
        std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60),
            sf::Color(140, 80, 180)
        };

        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 250;
        int col = 0;
        int count = 0;

        while (current != nullptr) {
            Sach* book = current->data;
            
            std::string tenSach = book->getTenSach();
            std::string tacGia = book->getTacGia();
            std::string theLoai = book->getTheLoai();
            std::string maSach = book->getMaSach();
            
            // Chuyển tất cả thông tin sách sang chữ thường để so sánh
            std::transform(tenSach.begin(), tenSach.end(), tenSach.begin(), ::tolower);
            std::transform(tacGia.begin(), tacGia.end(), tacGia.begin(), ::tolower);
            std::transform(theLoai.begin(), theLoai.end(), theLoai.begin(), ::tolower);
            std::transform(maSach.begin(), maSach.end(), maSach.begin(), ::tolower);
            
            if (tenSach.find(keywordLower) != std::string::npos ||
                tacGia.find(keywordLower) != std::string::npos ||
                theLoai.find(keywordLower) != std::string::npos ||
                maSach.find(keywordLower) != std::string::npos) {
                
                Card* card = new Card(
                    sf::Vector2f(cardX, cardY),
                    sf::Vector2f(180, 250),
                    book->getMaSach(),
                    book->getTenSach(),
                    book->getTacGia(),
                    std::to_string(book->getNamXuatBan()),
                    book->getDiemTrungBinh(),
                    colors[count % 5],
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
                count++;
            }
            
            current = current->next;
        }

        if (count == 0) {
            resultText.setString("Khong tim thay ket qua nao cho: \"" + keyword + "\"");
            resultText.setFillColor(sf::Color(255, 193, 94));
        } else {
            resultText.setString("Tim thay " + std::to_string(count) + " ket qua");
            resultText.setFillColor(sf::Color(100, 180, 100));
        }
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