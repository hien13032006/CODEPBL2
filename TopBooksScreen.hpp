#ifndef TOP_BOOKS_SCREEN_HPP
#define TOP_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Theme.hpp"

class TopBooksScreen {
private:
    sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar;
    std::vector<Card*> topBookCards; LibrarySystem* libSystem; ScrollView* scrollView;
    const float VIEW_X = 270.0f; const float VIEW_Y = 100.0f; const float VIEW_W = 990.0f; const float VIEW_H = 668.0f;

public:
    TopBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("TOP 10 SACH DUOC DANH GIA CAO"); titleText.setCharacterSize(30);
        titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font); scrollView = new ScrollView(sf::FloatRect(VIEW_X, VIEW_Y, VIEW_W, VIEW_H));
        loadBooks(font);
    }
    ~TopBooksScreen() { delete sidebar; delete scrollView; for (auto card : topBookCards) delete card; }

    void loadBooks(sf::Font& font) {
        for (auto card : topBookCards) delete card; 
        topBookCards.clear(); 
        if (!libSystem) return;
        
        libSystem->XepHangSach(); 
        int soLuong = libSystem->getTop10Count();
        
        // Danh sách màu (10 màu)
        std::vector<sf::Color> colors = { 
            sf::Color(255, 182, 193), sf::Color(173, 216, 230), sf::Color(144, 238, 144), 
            sf::Color(255, 218, 185), sf::Color(221, 160, 221), sf::Color(255, 228, 181), 
            sf::Color(176, 224, 230), sf::Color(240, 230, 140), sf::Color(255, 192, 203), 
            sf::Color(175, 238, 238) 
        };

        float startX = 20; float startY = 10; float cardW = 170; float cardH = 250; 
        float gapX = 200; float gapY = 280; 
        int col = 0, row = 0, count = 0;

        for (int i = 0; i < soLuong; ++i) {
            std::string id = libSystem->getTop10ID(i);
            
            NodeBook* current = libSystem->getDanhSachSach();
            while(current != nullptr) {
                if (current->data->getMaSach() == id) {
                    
                    // [SỬA LỖI] Dùng % colors.size() để đảm bảo không bao giờ vượt quá phạm vi
                    sf::Color c = colors.empty() ? sf::Color::White : colors[count % colors.size()];

                    Card* card = new Card(
                        sf::Vector2f(startX + col * gapX, startY + row * gapY), sf::Vector2f(cardW, cardH), 
                        current->data->getMaSach(), 
                        current->data->getImagePath(), 
                        current->data->getTenSach(), current->data->getTacGia(), current->data->getDiemTrungBinh(), 
                        c, font, true
                    );
                    topBookCards.push_back(card); col++; count++; if (col >= 5) { col = 0; row++; } break;
                } current = current->next;
            }
        }
        
        float totalHeight = startY + (row + 1) * gapY + 950.0f;
        scrollView->setMaxScroll(std::max(0.0f, totalHeight - VIEW_H)); scrollView->reset();
    }

    // Các hàm khác giữ nguyên
    void update(sf::Vector2f mousePos) { sidebar->update(mousePos); float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto card : topBookCards) card->update(relativeMouse, 0, 0); } else { for (auto card : topBookCards) card->update(sf::Vector2f(-1000, -1000), 0, 0); } }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }
    std::string handleCardClick(sf::Vector2f mousePos) { float scrollOffset = scrollView->getScrollOffset(); if (mousePos.y > VIEW_Y && mousePos.x > 250) { sf::Vector2f relativeMouse(mousePos.x - VIEW_X, mousePos.y - VIEW_Y + scrollOffset); for (auto card : topBookCards) if (card->isClicked(relativeMouse)) return card->getBookId(); } return ""; }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    void render(sf::RenderWindow& window) { window.setView(window.getDefaultView()); window.draw(background); sidebar->draw(window); window.draw(titleText); sf::View gridView = window.getDefaultView(); gridView.setViewport(sf::FloatRect(VIEW_X/1300.f, VIEW_Y/720.f, VIEW_W/1300.f, VIEW_H/720.f)); gridView.setSize(VIEW_W, VIEW_H); gridView.setCenter(VIEW_W/2.f, VIEW_H/2.f + scrollView->getScrollOffset()); window.setView(gridView); for (auto card : topBookCards) card->draw(window); window.setView(window.getDefaultView()); }
};
#endif