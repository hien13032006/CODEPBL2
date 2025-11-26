#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "InputField.hpp"
#include "LibrarySystem.h"
#include "ScrollView.hpp"
#include "Theme.hpp"

class HomeScreen {
private:
    sf::RectangleShape background; sf::Text titleTop10, titleAll; Sidebar* sidebar; InputField* searchBox; Button* searchBtnIcon; 
    std::vector<Card*> top10Cards, allBookCards; ScrollView *scrollTop10, *scrollAll; LibrarySystem* libSystem; bool isSearchTriggered;
    const float VIEW_X = 250.0f; const float TOP_Y = 130.0f; const float TOP_H = 280.0f; const float ALL_Y = 460.0f; const float ALL_H = 280.0f; const float VIEW_W = 1050.0f;

public:
    HomeScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        sidebar = new Sidebar(font);
        searchBox = new InputField(sf::Vector2f(280, 25), sf::Vector2f(400, 45), "Tim sach, tac gia...", font);
        searchBtnIcon = new Button(sf::Vector2f(690, 25), sf::Vector2f(100, 45), "Tim", font, 0, Theme::Primary);
        titleTop10.setFont(font); titleTop10.setString("TOP 10 SACH HAY NHAT"); titleTop10.setCharacterSize(20); titleTop10.setFillColor(Theme::TextDark); titleTop10.setPosition(280, 90);
        titleAll.setFont(font); titleAll.setString("KHAM PHA THEM"); titleAll.setCharacterSize(20); titleAll.setFillColor(Theme::TextDark); titleAll.setPosition(280, 420); 
        scrollTop10 = new ScrollView(sf::FloatRect(VIEW_X, TOP_Y, VIEW_W, TOP_H), ScrollDirection::HORIZONTAL);
        scrollAll = new ScrollView(sf::FloatRect(VIEW_X, ALL_Y, VIEW_W, ALL_H), ScrollDirection::HORIZONTAL);
        isSearchTriggered = false; loadBooks(font);
    }
    ~HomeScreen() { delete sidebar; delete searchBox; delete searchBtnIcon; delete scrollTop10; delete scrollAll; for (auto card : top10Cards) delete card; for (auto card : allBookCards) delete card; }

    void loadBooks(sf::Font& font) {
        for (auto card : top10Cards) delete card; top10Cards.clear(); for (auto card : allBookCards) delete card; allBookCards.clear(); if (!libSystem) return;
        libSystem->XepHangSach(); const auto& topIDs = libSystem->getTop10IDs();
        std::vector<sf::Color> colors = { sf::Color(255, 182, 193), sf::Color(176, 224, 230), sf::Color(255, 228, 181), sf::Color(221, 160, 221), sf::Color(152, 251, 152) };
        float startX = 10; float cardW = 160; float cardH = 240; float gapX = 180;  int idx = 0;
        
        // Load Top 10
        for (const auto& id : topIDs) {
            NodeBook* node = libSystem->getDanhSachSach();
            while(node) {
                if (node->data->getMaSach() == id) {
                    Card* card = new Card(
                        sf::Vector2f(startX + idx * gapX, 10), sf::Vector2f(cardW, cardH), 
                        node->data->getMaSach(), 
                        node->data->getImagePath(), // [NEW] Ảnh bìa
                        node->data->getTenSach(), node->data->getTacGia(), node->data->getDiemTrungBinh(), 
                        colors[idx % 5], font, true
                    );
                    top10Cards.push_back(card); idx++; break;
                } node = node->next;
            }
        }
        scrollTop10->setMaxScroll(idx * gapX + 400.0f); 

        // Load All Books
        idx = 0; NodeBook* curr = libSystem->getDanhSachSach();
        while (curr) {
            bool isHot = (curr->data->getDiemTrungBinh() >= 8.0);
            Card* card = new Card(
                sf::Vector2f(startX + idx * gapX, 10), sf::Vector2f(cardW, cardH), 
                curr->data->getMaSach(), 
                curr->data->getImagePath(), // [NEW] Ảnh bìa
                curr->data->getTenSach(), curr->data->getTacGia(), curr->data->getDiemTrungBinh(), 
                colors[idx % 5], font, isHot
            );
            allBookCards.push_back(card); idx++; curr = curr->next;
        }
        scrollAll->setMaxScroll(idx * gapX + 400.0f);
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos); searchBox->update(); searchBtnIcon->update(mousePos);
        float offsetTop = scrollTop10->getScrollOffset(); float offsetAll = scrollAll->getScrollOffset();
        if (mousePos.y >= TOP_Y && mousePos.y <= TOP_Y + TOP_H && mousePos.x >= VIEW_X) { sf::Vector2f rel(mousePos.x - VIEW_X + offsetTop, mousePos.y - TOP_Y); for (auto card : top10Cards) card->update(rel, 0, 0); } else { for (auto card : top10Cards) card->update(sf::Vector2f(-1000,-1000)); }
        if (mousePos.y >= ALL_Y && mousePos.y <= ALL_Y + ALL_H && mousePos.x >= VIEW_X) { sf::Vector2f rel(mousePos.x - VIEW_X + offsetAll, mousePos.y - ALL_Y); for (auto card : allBookCards) card->update(rel, 0, 0); } else { for (auto card : allBookCards) card->update(sf::Vector2f(-1000,-1000)); }
    }
    void handleSearchEvent(sf::Event& event, sf::Vector2f mousePos) { searchBox->handleEvent(event, mousePos); if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) { if (searchBox->hasFocus() && !searchBox->getText().empty()) isSearchTriggered = true; } scrollTop10->handleScroll(event, mousePos); scrollAll->handleScroll(event, mousePos); }
    std::string getSearchQuery() { if (isSearchTriggered) { isSearchTriggered = false; return searchBox->getText(); } return ""; }
    bool isSearchRequested() { return isSearchTriggered; }
    bool checkSearchClick(sf::Vector2f mousePos) { if (searchBtnIcon->handleClick(mousePos) && !searchBox->getText().empty()) { isSearchTriggered = true; return true; } return false; }
    std::string handleCardClick(sf::Vector2f mousePos) { float offsetTop = scrollTop10->getScrollOffset(); if (mousePos.y >= TOP_Y && mousePos.y <= TOP_Y + TOP_H && mousePos.x >= VIEW_X) { sf::Vector2f rel(mousePos.x - VIEW_X + offsetTop, mousePos.y - TOP_Y); for (auto card : top10Cards) if (card->isClicked(rel)) return card->getBookId(); } float offsetAll = scrollAll->getScrollOffset(); if (mousePos.y >= ALL_Y && mousePos.y <= ALL_Y + ALL_H && mousePos.x >= VIEW_X) { sf::Vector2f rel(mousePos.x - VIEW_X + offsetAll, mousePos.y - ALL_Y); for (auto card : allBookCards) if (card->isClicked(rel)) return card->getBookId(); } return ""; }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView()); window.draw(background);
        sf::View viewTop = window.getDefaultView(); viewTop.setViewport(sf::FloatRect(VIEW_X/1300.f, TOP_Y/720.f, VIEW_W/1300.f, TOP_H/720.f)); viewTop.setSize(VIEW_W, TOP_H); viewTop.setCenter(VIEW_W/2.f + scrollTop10->getScrollOffset(), TOP_H/2.f); window.setView(viewTop); for (auto card : top10Cards) card->draw(window);
        sf::View viewAll = window.getDefaultView(); viewAll.setViewport(sf::FloatRect(VIEW_X/1300.f, ALL_Y/720.f, VIEW_W/1300.f, ALL_H/720.f)); viewAll.setSize(VIEW_W, ALL_H); viewAll.setCenter(VIEW_W/2.f + scrollAll->getScrollOffset(), ALL_H/2.f); window.setView(viewAll); for (auto card : allBookCards) card->draw(window);
        window.setView(window.getDefaultView()); sidebar->draw(window); window.draw(titleTop10); window.draw(titleAll); searchBox->draw(window); searchBtnIcon->draw(window);
    }
};
#endif