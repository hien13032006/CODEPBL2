#ifndef ALL_BOOKS_LIST_SCREEN_HPP
#define ALL_BOOKS_LIST_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"

class AllBooksListScreen {
private:
    sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar;
    std::vector<Card*> allBookCards; LibrarySystem* libSystem; ScrollView* scrollView;

public:
    AllBooksListScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900)); background.setPosition(250, 0); background.setFillColor(sf::Color(245, 245, 250));
        titleText.setFont(font); titleText.setString("TAT CA SACH"); titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color(30, 30, 30)); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 100, 1150, 800));
        loadBooks(font);
    }

    ~AllBooksListScreen() { delete sidebar; delete scrollView; for (auto card : allBookCards) delete card; }

    void loadBooks(sf::Font& font) {
        for (auto card : allBookCards) delete card; allBookCards.clear();
        if (!libSystem) return;
        std::vector<sf::Color> colors = { sf::Color(200,80,60), sf::Color(60,140,200), sf::Color(100,180,100), sf::Color(200,150,60), sf::Color(140,80,180) };
        NodeBook* current = libSystem->getDanhSachSach();
        
        // Size nhỏ gọn (180x260)
        float startX = 35; float startY = 10; 
        float cardW = 180; float cardH = 260; float gapX = 220; float gapY = 300;  
        int col = 0; int row = 0; int count = 0;
        
        while (current != nullptr) {
            float posX = startX + col * gapX; float posY = startY + row * gapY;
            bool isHot = (current->data->getDiemTrungBinh() >= 8.0);
            Card* card = new Card(sf::Vector2f(posX, posY), sf::Vector2f(cardW, cardH),
                current->data->getMaSach(), current->data->getTenSach(), current->data->getTacGia(),
                current->data->getDiemTrungBinh(), colors[count % 5], font, isHot);
            allBookCards.push_back(card);
            col++; count++; if (col >= 5) { col = 0; row++; }
            current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, ((row + 1) * gapY + 50.0f) - 800.0f)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 100 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 100 + scrollOffset);
            for (auto card : allBookCards) card->update(relativeMouse, 0, 0);
        } else { for (auto card : allBookCards) card->update(sf::Vector2f(-1000, -1000), 0, 0); }
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }
    
    std::string handleCardClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 100 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 100 + scrollOffset);
            for (auto card : allBookCards) if (card->isClicked(relativeMouse)) return card->getBookId();
        } return "";
    }
    
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); sidebar->draw(window); window.draw(titleText);
        
        sf::View gridView = window.getDefaultView();
        gridView.setViewport(sf::FloatRect(250.f/1400.f, 100.f/900.f, 1150.f/1400.f, 800.f/900.f));
        gridView.setSize(1150, 800);
        gridView.setCenter(1150.f/2.f, 800.f/2.f + scrollView->getScrollOffset());
        window.setView(gridView);
        for (auto card : allBookCards) card->draw(window);
        window.setView(window.getDefaultView());
    }
};

#endif