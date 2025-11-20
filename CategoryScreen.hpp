#ifndef CATEGORY_SCREEN_HPP
#define CATEGORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <string>
#include "Button.hpp"
#include "Card.hpp"
#include "Sidebar.hpp"
#include "LibrarySystem.h"
#include "Node.h"

class CategoryScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    Button* backButton;
    std::vector<Button*> categoryButtons;
    std::vector<Card*> categoryBookCards;
    LibrarySystem* libSystem;
    std::string selectedCategory;

public:
    CategoryScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("The Loai Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        selectedCategory = "";

        backButton = new Button(sf::Vector2f(280, 80), sf::Vector2f(150, 45),
                               "< Quay Lai", font, 0, sf::Color(100, 100, 120));

        loadCategories(font);
    }

    ~CategoryScreen() {
        delete sidebar;
        delete backButton;
        for (auto btn : categoryButtons) delete btn;
        for (auto card : categoryBookCards) delete card;
    }

    void loadCategories(sf::Font& font) {
        for (auto btn : categoryButtons) delete btn;
        categoryButtons.clear();
        selectedCategory = ""; // Đảm bảo reset trạng thái category

        if (!libSystem) return;

        std::set<std::string> categories;
        NodeBook* current = libSystem->getDanhSachSach();
        
        while (current != nullptr) {
            categories.insert(current->data->getTheLoai());
            current = current->next;
        }

        float btnX = 280;
        float btnY = 150;
        int col = 0;

        for (const auto& cat : categories) {
            Button* btn = new Button(
                sf::Vector2f(btnX, btnY),
                sf::Vector2f(200, 50),
                cat,
                font,
                0,
                sf::Color(60, 100, 180)
            );
            
            categoryButtons.push_back(btn);

            col++;
            if (col == 5) {
                col = 0;
                btnX = 280;
                btnY += 70;
            } else {
                btnX += 220;
            }
        }
    }

    void loadBooksOfCategory(const std::string& category, sf::Font& font) {
        selectedCategory = category;
        titleText.setString("The Loai: " + category);

        for (auto card : categoryBookCards) delete card;
        categoryBookCards.clear();

        if (!libSystem) return;

        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60),
            sf::Color(140, 80, 180)
        };

        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 150;
        int col = 0;
        int count = 0;

        while (current != nullptr) {
            if (current->data->getTheLoai() == category) {
                Card* card = new Card(
                    sf::Vector2f(cardX, cardY),
                    sf::Vector2f(180, 250),
                    current->data->getMaSach(),
                    current->data->getTenSach(),
                    current->data->getTacGia(),
                    std::to_string(current->data->getNamXuatBan()),
                    current->data->getDiemTrungBinh(),
                    colors[count % 5],
                    font
                );
                categoryBookCards.push_back(card);

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
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        
        // Chỉ update backButton khi đang xem sách theo thể loại
        if (!selectedCategory.empty()) {
            backButton->update(mousePos);
        }
        
        for (auto btn : categoryButtons) btn->update(mousePos);
        for (auto card : categoryBookCards) card->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!selectedCategory.empty() && backButton->handleClick(mousePos)) {
            // FIX: Chỉ cần gọi loadCategories để reset màn hình về danh sách nút
            loadCategories(font); 
            titleText.setString("The Loai Sach");
            return;
        }
    }

    bool handleCategoryButtonClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!selectedCategory.empty()) return false;
        
        std::set<std::string> categories;
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            categories.insert(current->data->getTheLoai());
            current = current->next;
        }
        
        int index = 0;
        for (const auto& cat : categories) {
            if (index < (int)categoryButtons.size()) {
                if (categoryButtons[index]->handleClick(mousePos)) {
                    loadBooksOfCategory(cat, font);
                    return true;
                }
            }
            index++;
        }
        return false;
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        for (auto card : categoryBookCards) {
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

        if (selectedCategory.empty()) {
            for (auto btn : categoryButtons) {
                btn->draw(window);
            }
        } else {
            backButton->draw(window);
            for (auto card : categoryBookCards) {
                card->draw(window);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif
