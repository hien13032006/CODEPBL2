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
#include "ScrollView.hpp"

class CategoryScreen {
private:
    sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar; Button* backButton;
    std::vector<Button*> categoryButtons; std::vector<Card*> categoryBookCards;
    LibrarySystem* libSystem; std::string selectedCategory; ScrollView* scrollView;

public:
    CategoryScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900)); background.setPosition(250, 0); background.setFillColor(sf::Color(245, 245, 250));
        titleText.setFont(font); titleText.setString("THE LOAI SACH"); titleText.setCharacterSize(30); titleText.setFillColor(sf::Color(30, 30, 30)); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font); selectedCategory = "";
        backButton = new Button(sf::Vector2f(280, 80), sf::Vector2f(150, 45), "< Quay Lai", font, 0, sf::Color(100, 100, 120));
        scrollView = new ScrollView(sf::FloatRect(250, 150, 1150, 750));
        loadCategories(font);
    }

    ~CategoryScreen() { delete sidebar; delete backButton; delete scrollView; for (auto btn : categoryButtons) delete btn; for (auto card : categoryBookCards) delete card; }

    void loadCategories(sf::Font& font) {
        for (auto btn : categoryButtons) delete btn; categoryButtons.clear(); selectedCategory = "";
        if (!libSystem) return;
        std::set<std::string> categories; NodeBook* current = libSystem->getDanhSachSach();
        while (current) { categories.insert(current->data->getTheLoai()); current = current->next; }

        float btnX = 20; float btnY = 10; int col = 0; int row = 0;
        for (const auto& cat : categories) {
            Button* btn = new Button(sf::Vector2f(btnX + col*220, btnY + row*70), sf::Vector2f(200, 50), cat, font, 0, sf::Color(60, 100, 180));
            categoryButtons.push_back(btn);
            col++; if (col == 5) { col = 0; row++; }
        }
        scrollView->setMaxScroll(std::max(0.0f, ((row+1)*70.0f) - 750.0f)); scrollView->reset();
    }

    void loadBooksOfCategory(const std::string& category, sf::Font& font) {
        selectedCategory = category; titleText.setString("THE LOAI: " + category);
        for (auto card : categoryBookCards) delete card; categoryBookCards.clear();
        if (!libSystem) return;
        std::vector<sf::Color> colors = { sf::Color(200, 80, 60), sf::Color(60, 140, 200), sf::Color(100, 180, 100), sf::Color(200, 150, 60), sf::Color(140, 80, 180) };
        NodeBook* current = libSystem->getDanhSachSach();
        
        float startX = 35; float startY = 10; float cardW = 180; float cardH = 260; float gapX = 220; float gapY = 300;
        int col = 0; int row = 0; int count = 0;

        while (current != nullptr) {
            if (current->data->getTheLoai() == category) {
                Card* card = new Card(sf::Vector2f(startX+col*gapX, startY+row*gapY), sf::Vector2f(cardW, cardH),
                    current->data->getMaSach(), current->data->getTenSach(), current->data->getTacGia(),
                    current->data->getDiemTrungBinh(), colors[count % 5], font, (current->data->getDiemTrungBinh()>=8.0));
                categoryBookCards.push_back(card);
                col++; count++; if (col >= 5) { col = 0; row++; }
            }
            current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, ((row + 1) * gapY + 50.0f) - 750.0f)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos); if (!selectedCategory.empty()) backButton->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 150 && mousePos.x > 250) {
            sf::Vector2f rel(mousePos.x - 250, mousePos.y - 150 + scrollOffset);
            if (selectedCategory.empty()) for (auto btn : categoryButtons) btn->update(rel);
            else for (auto card : categoryBookCards) card->update(rel, 0, 0);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!selectedCategory.empty() && backButton->handleClick(mousePos)) { loadCategories(font); titleText.setString("THE LOAI SACH"); return; }
    }

    bool handleCategoryButtonClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!selectedCategory.empty()) return false;
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 150 && mousePos.x > 250) {
            sf::Vector2f rel(mousePos.x - 250, mousePos.y - 150 + scrollOffset);
            for (auto btn : categoryButtons) if (btn->handleClick(rel)) { loadBooksOfCategory(btn->getTextString(), font); return true; }
        }
        return false;
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        if (selectedCategory.empty()) return "";
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 150 && mousePos.x > 250) {
            sf::Vector2f rel(mousePos.x - 250, mousePos.y - 150 + scrollOffset);
            for (auto card : categoryBookCards) if (card->isClicked(rel)) return card->getBookId();
        } return "";
    }

    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); sidebar->draw(window); window.draw(titleText);
        
        sf::View view = window.getDefaultView();
        view.setViewport(sf::FloatRect(250.f/1400.f, 150.f/900.f, 1150.f/1400.f, 750.f/900.f));
        view.setSize(1150, 750);
        view.setCenter(1150.f/2.f, 750.f/2.f + scrollView->getScrollOffset());
        window.setView(view);

        if (selectedCategory.empty()) { for (auto btn : categoryButtons) btn->draw(window); } 
        else { for (auto card : categoryBookCards) card->draw(window); }
        
        window.setView(window.getDefaultView());
        if (!selectedCategory.empty()) backButton->draw(window);
    }
};

#endif