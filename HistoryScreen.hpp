#ifndef HISTORY_SCREEN_HPP
#define HISTORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "Reader.h"

struct HistoryItem {
    sf::RectangleShape box;
    sf::Text actionText;
    sf::Text bookText;
    sf::Text timeText;
};

class HistoryScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    std::vector<HistoryItem*> historyItems;
    Reader* currentReader;

public:
    HistoryScreen(sf::Font& font, Reader* reader) : currentReader(reader) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Lich Su Muon - Tra Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Chua co lich su nao!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadHistory(font);
    }

    ~HistoryScreen() {
        delete sidebar;
        delete scrollView;
        for (auto item : historyItems) delete item;
    }

    void loadHistory(sf::Font& font) {
        // Xóa items cũ
        for (auto item : historyItems) delete item;
        historyItems.clear();

        if (!currentReader) return;

        // TODO: Đọc từ file lịch sử
        // currentReader->HienThiLichSuMuonTra();

        float yPos = 120;
        for (int i = 0; i < 15; i++) {
            HistoryItem* item = new HistoryItem();
            
            item->box.setSize(sf::Vector2f(1000, 70));
            item->box.setPosition(300, yPos);
            item->box.setFillColor(sf::Color(25, 28, 40));
            item->box.setOutlineThickness(1);
            item->box.setOutlineColor(sf::Color(40, 43, 55));

            item->actionText.setFont(font);
            bool isBorrow = (i % 2 == 0);
            item->actionText.setString(isBorrow ? "MUON" : "TRA");
            item->actionText.setCharacterSize(16);
            item->actionText.setFillColor(isBorrow ? 
                sf::Color(255, 193, 94) : sf::Color(100, 180, 100));
            item->actionText.setPosition(320, yPos + 25);

            item->bookText.setFont(font);
            item->bookText.setString("Co So Du Lieu - Vo Duc Hoang");
            item->bookText.setCharacterSize(15);
            item->bookText.setFillColor(sf::Color::White);
            item->bookText.setPosition(420, yPos + 25);

            item->timeText.setFont(font);
            item->timeText.setString("15/11/2025 14:30:00");
            item->timeText.setCharacterSize(13);
            item->timeText.setFillColor(sf::Color(120, 120, 120));
            item->timeText.setPosition(1000, yPos + 27);

            historyItems.push_back(item);
            yPos += 80;
        }

        scrollView->setMaxScroll(std::max(0.0f, yPos - 700.0f));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (historyItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : historyItems) {
                window.draw(item->box);
                window.draw(item->actionText);
                window.draw(item->bookText);
                window.draw(item->timeText);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif