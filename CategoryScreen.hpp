#ifndef CATEGORY_SCREEN_HPP
#define CATEGORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <string>
#include <map>
#include "Button.hpp"
#include "Card.hpp"
#include "Sidebar.hpp"
#include "LibrarySystem.h"
#include "Node.h"
#include "ScrollView.hpp"
#include "Theme.hpp"
#include "RoundedRectangle.hpp"

struct CategoryBox { RoundedRectangleShape box; sf::Text name, count; bool isHovered; };

class CategoryScreen {
private:
    sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar; Button* backButton;
    std::vector<CategoryBox*> catBoxes; std::vector<Card*> categoryBookCards;
    LibrarySystem* libSystem; std::string selectedCategory; ScrollView* scrollView;
    const float VIEW_X = 270.0f; const float VIEW_Y = 120.0f; const float VIEW_W = 990.0f; const float VIEW_H = 580.0f;

public:
    CategoryScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("THE LOAI SACH"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font); selectedCategory = "";
        backButton = new Button(sf::Vector2f(280, 70), sf::Vector2f(150, 40), "< Quay Lai", font, 0, Theme::Secondary);
        scrollView = new ScrollView(sf::FloatRect(VIEW_X, VIEW_Y, 1300 - VIEW_X, VIEW_H));
        loadCategories(font);
    }
    ~CategoryScreen() { delete sidebar; delete backButton; delete scrollView; for (auto box : catBoxes) delete box; for (auto card : categoryBookCards) delete card; }

    void loadCategories(sf::Font& font) {
        for (auto box : catBoxes) delete box; catBoxes.clear(); selectedCategory = ""; titleText.setString("THE LOAI SACH");
        if (!libSystem) return;
        std::map<std::string, int> catCounts; NodeBook* current = libSystem->getDanhSachSach();
        while (current) { catCounts[current->data->getTheLoai()]++; current = current->next; }
        std::vector<sf::Color> colors = {sf::Color(173, 216, 230), sf::Color(255, 182, 193), sf::Color(152, 251, 152), sf::Color(255, 228, 181), sf::Color(221, 160, 221)};
        float startX = 20; float startY = 20; float boxW = 280; float boxH = 180; float gapX = 320; float gapY = 220; int col = 0, row = 0, idx = 0;
        for (const auto& pair : catCounts) {
            CategoryBox* box = new CategoryBox();
            box->box.setSize({boxW, boxH}); box->box.setCornerRadius(15.0f); box->box.setPosition(startX + col*gapX, startY + row*gapY); box->box.setFillColor(colors[idx % 5]);
            box->name.setFont(font); box->name.setString(pair.first); box->name.setCharacterSize(24); box->name.setFillColor(Theme::TextDark);
            sf::FloatRect b = box->name.getLocalBounds(); box->name.setOrigin(b.width/2, b.height/2); box->name.setPosition(box->box.getPosition().x + boxW/2, box->box.getPosition().y + boxH/2 - 10);
            box->count.setFont(font); box->count.setString(std::to_string(pair.second) + " dau sach"); box->count.setCharacterSize(16); box->count.setFillColor(sf::Color(80, 80, 80));
            sf::FloatRect c = box->count.getLocalBounds(); box->count.setOrigin(c.width/2, c.height/2); box->count.setPosition(box->box.getPosition().x + boxW/2, box->box.getPosition().y + boxH/2 + 25);
            box->isHovered = false; catBoxes.push_back(box);
            idx++; col++; if (col == 3) { col = 0; row++; }
        }
        // FIX: Padding 950.0f
        float contentBottom = startY + ((catBoxes.size() + 2) / 3) * gapY + 950.0f;
        scrollView->setMaxScroll(std::max(0.0f, contentBottom - VIEW_H)); scrollView->reset();
    }

    void loadBooksOfCategory(const std::string& category, sf::Font& font) {
        selectedCategory = category; titleText.setString("THE LOAI: " + category);
        for (auto card : categoryBookCards) delete card; categoryBookCards.clear();
        std::vector<sf::Color> colors = { sf::Color(255, 182, 193), sf::Color(176, 224, 230), sf::Color(221, 160, 221), sf::Color(240, 230, 140), sf::Color(152, 251, 152) };
        NodeBook* current = libSystem->getDanhSachSach();
        float startX = 10; float startY = 20; float cardW = 170; float cardH = 250; float gapX = 200; float gapY = 280; int col = 0, row = 0;
        while (current != nullptr) {
            if (current->data->getTheLoai() == category) {
                Card* card = new Card(
                    sf::Vector2f(startX+col*gapX, startY+row*gapY), sf::Vector2f(cardW, cardH), 
                    current->data->getMaSach(), 
                    current->data->getImagePath(), // [NEW] Ảnh bìa
                    current->data->getTenSach(), current->data->getTacGia(), current->data->getDiemTrungBinh(), 
                    colors[categoryBookCards.size() % 5], font, (current->data->getDiemTrungBinh()>=8.0)
                );
                categoryBookCards.push_back(card); col++; if (col == 5) { col = 0; row++; }
            } current = current->next;
        }
        // FIX: Padding 950.0f
        float contentBottom = startY + ((categoryBookCards.size() + 4) / 5) * gapY + 950.0f;
        scrollView->setMaxScroll(std::max(0.0f, contentBottom - VIEW_H)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos); if (!selectedCategory.empty()) backButton->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > VIEW_Y && mousePos.x > 250) {
            sf::Vector2f rel(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset);
            if (selectedCategory.empty()) {
                for (auto box : catBoxes) { if(box->box.getGlobalBounds().contains(rel)) { if(!box->isHovered) { box->isHovered=true; box->box.setOutlineThickness(3); box->box.setOutlineColor(Theme::Secondary); } } else { if(box->isHovered) { box->isHovered=false; box->box.setOutlineThickness(0); } } }
            } else { for (auto card : categoryBookCards) card->update(rel, 0, 0); }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }
    void handleClick(sf::Vector2f mousePos, sf::Font& font) { if (!selectedCategory.empty() && backButton->handleClick(mousePos)) { loadCategories(font); return; } }
    bool handleCategoryButtonClick(sf::Vector2f mousePos, sf::Font& font) { if (!selectedCategory.empty()) return false; float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f rel(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto box : catBoxes) { if (box->box.getGlobalBounds().contains(rel)) { loadBooksOfCategory(box->name.getString(), font); return true; } } } return false; }
    std::string handleCardClick(sf::Vector2f mousePos) { if (selectedCategory.empty()) return ""; float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f rel(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto card : categoryBookCards) if (card->isClicked(rel)) return card->getBookId(); } return ""; }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView()); window.draw(background); sidebar->draw(window); window.draw(titleText);
        sf::View view = window.getDefaultView(); view.setViewport(sf::FloatRect(VIEW_X/1300.f, VIEW_Y/720.f, VIEW_W/1300.f, VIEW_H/720.f)); view.setSize(VIEW_W, VIEW_H); view.setCenter(VIEW_W/2.f, VIEW_H/2.f + scrollView->getScrollOffset()); window.setView(view);
        if (selectedCategory.empty()) { for (auto box : catBoxes) { window.draw(box->box); window.draw(box->name); window.draw(box->count); } } else { for (auto card : categoryBookCards) card->draw(window); }
        window.setView(window.getDefaultView()); if (!selectedCategory.empty()) backButton->draw(window);
    }
};
#endif