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
#include "ScrollView.hpp"

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
    ScrollView* scrollView;

public:
    SearchScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(245, 245, 250));

        titleText.setFont(font); titleText.setString("TIM KIEM SACH");
        titleText.setCharacterSize(30); titleText.setFillColor(sf::Color(30, 30, 30)); titleText.setPosition(280, 30);

        resultText.setFont(font); resultText.setString("Nhap tu khoa de tim kiem...");
        resultText.setCharacterSize(18); resultText.setFillColor(sf::Color(150, 150, 150)); resultText.setPosition(280, 160);

        sidebar = new Sidebar(font);
        // ScrollView bắt đầu từ Y=200 (Dưới thanh search và text kết quả)
        scrollView = new ScrollView(sf::FloatRect(250, 200, 1150, 700));

        searchField = new InputField(sf::Vector2f(280, 90), sf::Vector2f(600, 50), "Ten sach, tac gia, the loai...", font);
        searchButton = new Button(sf::Vector2f(900, 90), sf::Vector2f(150, 50), "Tim", font);
    }

    ~SearchScreen() {
        delete sidebar; delete searchField; delete searchButton; delete scrollView;
        for (auto card : resultCards) delete card;
    }

    void setSearchQuery(const std::string& query) { searchField->setText(query); }

    void performSearch(sf::Font& font) {
        for (auto card : resultCards) delete card; resultCards.clear();
        std::string keyword = searchField->getText();
        if (keyword.empty()) { resultText.setString("Vui long nhap tu khoa!"); resultText.setFillColor(sf::Color(255, 100, 100)); return; }
        if (!libSystem) return;

        std::string kLower = keyword; std::transform(kLower.begin(), kLower.end(), kLower.begin(), ::tolower);
        std::vector<sf::Color> colors = { sf::Color(200,80,60), sf::Color(60,140,200), sf::Color(100,180,100), sf::Color(200,150,60), sf::Color(140,80,180) };

        NodeBook* current = libSystem->getDanhSachSach();
        
        // Grid Config
        float startX = 35; float startY = 10;
        float cardW = 180; float cardH = 260;
        float gapX = 220;  float gapY = 300;
        
        int col = 0; int row = 0; int count = 0;

        while (current != nullptr) {
            Sach* book = current->data;
            std::string ten = book->getTenSach(); std::string tg = book->getTacGia();
            std::string tl = book->getTheLoai(); std::string ma = book->getMaSach();
            std::transform(ten.begin(), ten.end(), ten.begin(), ::tolower);
            std::transform(tg.begin(), tg.end(), tg.begin(), ::tolower);
            std::transform(tl.begin(), tl.end(), tl.begin(), ::tolower);
            std::transform(ma.begin(), ma.end(), ma.begin(), ::tolower);
            
            if (ten.find(kLower)!=std::string::npos || tg.find(kLower)!=std::string::npos || 
                tl.find(kLower)!=std::string::npos || ma.find(kLower)!=std::string::npos) {
                
                Card* card = new Card(sf::Vector2f(startX + col*gapX, startY + row*gapY), sf::Vector2f(cardW, cardH),
                    book->getMaSach(), book->getTenSach(), book->getTacGia(), book->getDiemTrungBinh(),
                    colors[count % 5], font, (book->getDiemTrungBinh()>=8.0));
                resultCards.push_back(card);

                col++; count++;
                if (col >= 5) { col = 0; row++; }
            }
            current = current->next;
        }

        if (count == 0) { resultText.setString("Khong tim thay ket qua nao cho: \"" + keyword + "\""); resultText.setFillColor(sf::Color(255, 193, 94)); } 
        else { resultText.setString("Tim thay " + std::to_string(count) + " ket qua"); resultText.setFillColor(sf::Color(100, 180, 100)); }
        
        scrollView->setMaxScroll(std::max(0.0f, ((row + 1) * gapY + 50.0f) - 700.0f));
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos); searchField->update(); searchButton->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 200 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 200 + scrollOffset);
            for (auto card : resultCards) card->update(relativeMouse, 0, 0);
        } else for (auto card : resultCards) card->update(sf::Vector2f(-1000, -1000), 0, 0);
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); }
    bool handleSearchClick(sf::Vector2f mousePos, sf::Font& font) { if (searchButton->handleClick(mousePos)) { performSearch(font); return true; } return false; }

    std::string handleCardClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 200 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 200 + scrollOffset);
            for (auto card : resultCards) if (card->isClicked(relativeMouse)) return card->getBookId();
        } return "";
    }

    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); sidebar->draw(window); window.draw(titleText);
        searchField->draw(window); searchButton->draw(window); window.draw(resultText);

        sf::View gridView = window.getDefaultView();
        gridView.setViewport(sf::FloatRect(250.f/1400.f, 200.f/900.f, 1150.f/1400.f, 700.f/900.f));
        gridView.setSize(1150, 700);
        gridView.setCenter(1150.f/2.f, 700.f/2.f + scrollView->getScrollOffset());
        window.setView(gridView);
        for (auto card : resultCards) card->draw(window);
        window.setView(window.getDefaultView());
    }
};

#endif