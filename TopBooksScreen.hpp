#ifndef TOP_BOOKS_SCREEN_HPP
#define TOP_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Node.h" 

class TopBooksScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    std::vector<Card*> topBookCards;
    LibrarySystem* libSystem;
    ScrollView* scrollView;

public:
    TopBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        // 1. Background Light Theme
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(245, 245, 250)); 

        // 2. Title
        titleText.setFont(font);
        titleText.setString("TOP 10 SACH DUOC DANH GIA CAO");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color(30, 30, 30)); 
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        
        // 3. ScrollView
        scrollView = new ScrollView(sf::FloatRect(250, 100, 1150, 800));
        
        loadBooks(font);
    }

    ~TopBooksScreen() {
        delete sidebar; delete scrollView;
        for (auto card : topBookCards) delete card;
    }

    void loadBooks(sf::Font& font) {
        for (auto card : topBookCards) delete card;
        topBookCards.clear();

        if (!libSystem) return;

        libSystem->XepHangSach(); 
        const auto& topIDs = libSystem->getTop10IDs();

        std::vector<sf::Color> colors = { 
            sf::Color(255, 182, 193), sf::Color(173, 216, 230), 
            sf::Color(144, 238, 144), sf::Color(255, 218, 185), 
            sf::Color(221, 160, 221), sf::Color(255, 228, 181), 
            sf::Color(176, 224, 230), sf::Color(240, 230, 140), 
            sf::Color(255, 192, 203), sf::Color(175, 238, 238)  
        };
        
        // Cấu hình Grid (Size 180x260)
        float startX = 35; float startY = 10;
        float cardW = 180; float cardH = 260;
        float gapX = 220;  float gapY = 300;
        
        int col = 0; int row = 0; int count = 0;

        for (const std::string& id : topIDs) {
            NodeBook* current = libSystem->getDanhSachSach();
            while(current != nullptr) {
                if (current->data->getMaSach() == id) {
                    float posX = startX + col * gapX;
                    float posY = startY + row * gapY;

                    Card* card = new Card(
                        sf::Vector2f(posX, posY),
                        sf::Vector2f(cardW, cardH),
                        current->data->getMaSach(),
                        current->data->getTenSach(),
                        current->data->getTacGia(),
                        current->data->getDiemTrungBinh(),
                        colors[count % 10],
                        font,
                        true // Luôn là HOT
                    );
                    topBookCards.push_back(card);

                    col++; count++;
                    if (col >= 5) { col = 0; row++; }
                    break;
                }
                current = current->next;
            }
        }
        
        float totalHeight = (row + 1) * gapY + 50.0f;
        scrollView->setMaxScroll(std::max(0.0f, totalHeight - 800.0f)); 
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        
        if (mousePos.y > 100 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 100 + scrollOffset);
            for (auto card : topBookCards) card->update(relativeMouse, 0, 0);
        } else {
            for (auto card : topBookCards) card->update(sf::Vector2f(-1000, -1000), 0, 0);
        }
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos); 
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > 100 && mousePos.x > 250) {
            sf::Vector2f relativeMouse(mousePos.x - 250, mousePos.y - 100 + scrollOffset);
            for (auto card : topBookCards) if (card->isClicked(relativeMouse)) return card->getBookId();
        }
        return "";
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
        
        for (auto card : topBookCards) card->draw(window);
        
        window.setView(window.getDefaultView());
    }
};

#endif