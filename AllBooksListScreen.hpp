#ifndef ALL_BOOKS_LIST_SCREEN_HPP
#define ALL_BOOKS_LIST_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Theme.hpp"

class AllBooksListScreen {
private:
    sf::RectangleShape background; 
    sf::Text titleText; 
    Sidebar* sidebar;
    std::vector<Card*> allBookCards; 
    LibrarySystem* libSystem; 
    ScrollView* scrollView;
    const float VIEW_X = 270.0f; 
    const float VIEW_Y = 100.0f; 
    const float VIEW_W = 990.0f; 
    const float VIEW_H = 668.0f;

public:
    AllBooksListScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); 
        background.setPosition(250, 0); 
        background.setFillColor(Theme::Background);

        titleText.setFont(font); 
        titleText.setString("TAT CA SACH"); 
        titleText.setCharacterSize(30); 
        titleText.setFillColor(Theme::TextDark); 
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font); 
        scrollView = new ScrollView(sf::FloatRect(VIEW_X, VIEW_Y, VIEW_W, VIEW_H));
        loadBooks(font);
    }
    ~AllBooksListScreen() { 
        delete sidebar; 
        delete scrollView; 
        for (auto card : allBookCards) delete card; 
    }

    void loadBooks(sf::Font& font) {
        for (auto card : allBookCards) delete card; 
        allBookCards.clear(); 
        if (!libSystem) 
            return;
        std::vector<sf::Color> colors = { 
            sf::Color(255, 182, 193), 
            sf::Color(176, 224, 230), 
            sf::Color(221, 160, 221), 
            sf::Color(240, 230, 140), 
            sf::Color(152, 251, 152) 
        };
        NodeBook* current = libSystem->getDanhSachSach();

        float startX = 10; 
        float startY = 20; 
        float cardW = 170; 
        float cardH = 250; 
        float gapX = 200; 
        float gapY = 280; 
        int col = 0; int row = 0;
        while (current != nullptr) {
            Card* card = new Card(
                sf::Vector2f(startX + col * gapX, startY + row * gapY), sf::Vector2f(cardW, cardH), 
                current->data->getMaSach(), 
                current->data->getImagePath(), // [NEW] Ảnh bìa
                current->data->getTenSach(), 
                current->data->getTacGia(), 
                current->data->getDiemTrungBinh(), 
                colors[allBookCards.size() % 5], font, (current->data->getDiemTrungBinh() >= 8.0)
            );
            allBookCards.push_back(card); 
            col++; 
            if (col >= 5) { 
                col = 0; row++; 
            } 
            current = current->next;
        }

        float contentHeight = startY + (row + 1) * gapY + 950.0f;
        scrollView->setMaxScroll(std::max(0.0f, contentHeight - VIEW_H)); 
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) { 
        sidebar->update(mousePos); 
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > VIEW_Y && mousePos.x > 250) { 
            sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); 
            for (auto card : allBookCards) 
                card->update(relativeMouse, 0, 0); 
        } else { 
            for (auto card : allBookCards) 
                card->update(sf::Vector2f(-1000, -1000), 0, 0); 
        } 
    }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { 
        scrollView->handleScroll(event, mousePos); 
    }
    std::string handleCardClick(sf::Vector2f mousePos) { 
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > VIEW_Y && mousePos.x > 250) { 
            sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); 
            for (auto card : allBookCards) 
                if (card->isClicked(relativeMouse)) 
                    return card->getBookId(); 
        } 
        return ""; 
    }
    int handleSidebarClick(sf::Vector2f mousePos) { 
        return sidebar->handleClick(mousePos); 
    }
    void setUserRole(UserRole role, sf::Font& font) { 
        sidebar->setUserRole(role, font); 
    } 
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) { 
        window.setView(window.getDefaultView());
        window.draw(background); 
        sidebar->draw(window); 
        window.draw(titleText); 
        sf::View gridView = window.getDefaultView(); 
        gridView.setViewport(sf::FloatRect(VIEW_X/1300.f, VIEW_Y/720.f, VIEW_W/1300.f, VIEW_H/720.f)); 
        gridView.setSize(VIEW_W, VIEW_H); 
        gridView.setCenter(VIEW_W/2.f, VIEW_H/2.f + scrollView->getScrollOffset()); 
        window.setView(gridView); 
        for (auto card : allBookCards) 
            card->draw(window); 
        window.setView(window.getDefaultView()); 
    }
};
#endif