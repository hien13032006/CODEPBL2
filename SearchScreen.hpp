#ifndef SEARCH_SCREEN_HPP
#define SEARCH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Sidebar.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "Card.hpp"
#include "LibrarySystem.h"
#include "ScrollView.hpp"
#include "Theme.hpp"

class SearchScreen {
private:
    sf::RectangleShape background; sf::Text titleText, resultText; Sidebar* sidebar; InputField* searchField; Button* searchButton;
    std::vector<Card*> resultCards; LibrarySystem* libSystem; ScrollView* scrollView;
    const float VIEW_X = 270.0f; const float VIEW_Y = 200.0f; const float VIEW_W = 990.0f; const float VIEW_H = 540.0f;

public:
    SearchScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("TIM KIEM SACH"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        resultText.setFont(font); resultText.setString("Nhap tu khoa de tim kiem..."); resultText.setCharacterSize(18); resultText.setFillColor(Theme::TextLight); resultText.setPosition(280, 160);
        sidebar = new Sidebar(font); scrollView = new ScrollView(sf::FloatRect(VIEW_X, VIEW_Y, VIEW_W, VIEW_H));
        searchField = new InputField(sf::Vector2f(280, 90), sf::Vector2f(600, 50), "Ten sach, tac gia...", font);
        searchButton = new Button(sf::Vector2f(900, 90), sf::Vector2f(150, 50), "Tim", font, 0, Theme::Primary);
    }
    ~SearchScreen() { delete sidebar; delete searchField; delete searchButton; delete scrollView; for (auto card : resultCards) delete card; }
    void setSearchQuery(const std::string& query) { searchField->setText(query); }

    void performSearch(sf::Font& font) {
        for (auto card : resultCards) delete card; resultCards.clear(); std::string keyword = searchField->getText();
        if (keyword.empty()) { resultText.setString("Vui long nhap tu khoa!"); resultText.setFillColor(Theme::Error); return; }
        if (!libSystem) return; std::string kLower = keyword; std::transform(kLower.begin(), kLower.end(), kLower.begin(), ::tolower);
        std::vector<sf::Color> colors = { sf::Color(255, 182, 193), sf::Color(176, 224, 230), sf::Color(221, 160, 221), sf::Color(240, 230, 140), sf::Color(152, 251, 152) };
        NodeBook* current = libSystem->getDanhSachSach(); float startX = 10; float startY = 10; float cardW = 170; float cardH = 250; float gapX = 200; float gapY = 280; int col = 0, row = 0, count = 0;
        while (current != nullptr) {
            Sach* book = current->data; std::string ten = book->getTenSach(), tg = book->getTacGia(), tl = book->getTheLoai(), ma = book->getMaSach();
            std::transform(ten.begin(), ten.end(), ten.begin(), ::tolower); std::transform(tg.begin(), tg.end(), tg.begin(), ::tolower); std::transform(tl.begin(), tl.end(), tl.begin(), ::tolower); std::transform(ma.begin(), ma.end(), ma.begin(), ::tolower);
            if (ten.find(kLower)!=std::string::npos || tg.find(kLower)!=std::string::npos || tl.find(kLower)!=std::string::npos || ma.find(kLower)!=std::string::npos) {
                Card* card = new Card(sf::Vector2f(startX + col*gapX, startY + row*gapY), sf::Vector2f(cardW, cardH), book->getMaSach(), book->getTenSach(), book->getTacGia(), book->getDiemTrungBinh(), colors[count % 5], font, (book->getDiemTrungBinh()>=8.0));
                resultCards.push_back(card); col++; count++; if (col >= 5) { col = 0; row++; }
            } current = current->next;
        }
        if (count == 0) { resultText.setString("Khong tim thay ket qua nao."); resultText.setFillColor(Theme::Error); } 
        else { resultText.setString("Tim thay " + std::to_string(count) + " ket qua"); resultText.setFillColor(Theme::Success); }
        // FIX: Padding 950.0f
        float totalH = startY + (row + 1) * gapY + 950.0f; 
        scrollView->setMaxScroll(std::max(0.0f, totalH - VIEW_H)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) { sidebar->update(mousePos); searchField->update(); searchButton->update(mousePos); float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto card : resultCards) card->update(relativeMouse, 0, 0); } else { for (auto card : resultCards) card->update(sf::Vector2f(-1000, -1000), 0, 0); } }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); }
    bool handleSearchClick(sf::Vector2f mousePos, sf::Font& font) { if (searchButton->handleClick(mousePos)) { performSearch(font); return true; } return false; }
    std::string handleCardClick(sf::Vector2f mousePos) { float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto card : resultCards) if (card->isClicked(relativeMouse)) return card->getBookId(); } return ""; }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    void render(sf::RenderWindow& window) { window.setView(window.getDefaultView()); window.draw(background); sidebar->draw(window); window.draw(titleText); searchField->draw(window); searchButton->draw(window); window.draw(resultText); sf::View gridView = window.getDefaultView(); gridView.setViewport(sf::FloatRect(VIEW_X/1300.f, VIEW_Y/720.f, VIEW_W/1300.f, VIEW_H/720.f)); gridView.setSize(VIEW_W, VIEW_H); gridView.setCenter(VIEW_W/2.f, VIEW_H/2.f + scrollView->getScrollOffset()); window.setView(gridView); for (auto card : resultCards) card->draw(window); window.setView(window.getDefaultView()); }
};
#endif