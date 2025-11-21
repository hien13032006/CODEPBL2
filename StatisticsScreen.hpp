#ifndef STATISTICS_SCREEN_HPP
#define STATISTICS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Sidebar.hpp"
#include "LibrarySystem.h"
#include "RoundedRectangle.hpp"

struct StatBox {
    RoundedRectangleShape box;
    sf::Text title;
    sf::Text value;
};

class StatisticsScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    
    StatBox stats[4]; // 0: Sách, 1: Độc giả, 2: Đang mượn, 3: Quá hạn
    LibrarySystem* libSystem;

public:
    StatisticsScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("THONG KE HE THONG");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        // Config Stats
        std::string titles[] = {"TONG SO SACH", "TONG DOC GIA", "SACH DANG MUON", "SACH QUA HAN"};
        sf::Color colors[] = {sf::Color(60, 100, 180), sf::Color(100, 180, 100), sf::Color(200, 150, 60), sf::Color(200, 80, 80)};
        float xPos[] = {300, 750, 300, 750};
        float yPos[] = {150, 150, 450, 450};

        for(int i=0; i<4; i++) {
            stats[i].box.setSize({400, 250});
            stats[i].box.setCornerRadius(20.0f);
            stats[i].box.setPosition(xPos[i], yPos[i]);
            stats[i].box.setFillColor(colors[i]);

            stats[i].title.setFont(font);
            stats[i].title.setString(titles[i]);
            stats[i].title.setCharacterSize(20);
            stats[i].title.setFillColor(sf::Color(255, 255, 255, 200));
            stats[i].title.setPosition(xPos[i] + 30, yPos[i] + 30);

            stats[i].value.setFont(font);
            stats[i].value.setString("0");
            stats[i].value.setCharacterSize(60);
            stats[i].value.setStyle(sf::Text::Bold);
            stats[i].value.setFillColor(sf::Color::White);
            stats[i].value.setPosition(xPos[i] + 30, yPos[i] + 100);
        }
        loadStatistics();
    }

    ~StatisticsScreen() { delete sidebar; }

    void loadStatistics() {
        if (!libSystem) return;
        int totalBooks = 0, totalReaders = 0, borrowed = 0, overdue = 0;

        NodeBook* curBook = libSystem->getDanhSachSach();
        while (curBook) { totalBooks++; curBook = curBook->next; }

        NodeReader* curReader = libSystem->getDanhSachDocGia();
        while (curReader) {
            totalReaders++;
            borrowed += curReader->data->DemSachDaMuon();
            overdue += curReader->data->DemSachQuaHan();
            curReader = curReader->next;
        }

        stats[0].value.setString(std::to_string(totalBooks));
        stats[1].value.setString(std::to_string(totalReaders));
        stats[2].value.setString(std::to_string(borrowed));
        stats[3].value.setString(std::to_string(overdue));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        for(int i=0; i<4; i++) {
            if(stats[i].box.getGlobalBounds().contains(mousePos)) {
                stats[i].box.setOutlineThickness(4);
                stats[i].box.setOutlineColor(sf::Color(255, 255, 255, 150));
            } else {
                stats[i].box.setOutlineThickness(0);
            }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    
    int handleClick(sf::Vector2f mousePos) {
        if(stats[0].box.getGlobalBounds().contains(mousePos)) return 1; // Home
        if(stats[1].box.getGlobalBounds().contains(mousePos)) return 2; // Readers
        if(stats[2].box.getGlobalBounds().contains(mousePos)) return 3; // Borrowed (Optional)
        if(stats[3].box.getGlobalBounds().contains(mousePos)) return 4; // Overdue
        return 0;
    }

    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        for(int i=0; i<4; i++) {
            window.draw(stats[i].box);
            window.draw(stats[i].title);
            window.draw(stats[i].value);
        }
    }
};

#endif 
