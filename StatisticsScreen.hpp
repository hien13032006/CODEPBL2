#ifndef STATISTICS_SCREEN_HPP
#define STATISTICS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Sidebar.hpp"
#include "LibrarySystem.h"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

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
    StatBox stats[4]; 
    LibrarySystem* libSystem;

public:
    StatisticsScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1280, 768));
        background.setPosition(250, 0);
        background.setFillColor(Theme::Background);

        titleText.setFont(font);
        titleText.setString("THONG KE HE THONG");
        titleText.setCharacterSize(30);
        titleText.setFillColor(Theme::TextDark);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        std::string titles[] = {"TONG SO SACH", "TONG DOC GIA", "SACH DANG MUON", "SACH QUA HAN"};
        // Màu Pastel
        sf::Color colors[] = {sf::Color(173, 216, 230), sf::Color(152, 251, 152), sf::Color(255, 228, 181), sf::Color(255, 182, 193)};
        float xPos[] = {300, 750, 300, 750};
        float yPos[] = {120, 120, 400, 400};

        for(int i=0; i<4; i++) {
            stats[i].box.setSize({400, 250});
            stats[i].box.setCornerRadius(20.0f);
            stats[i].box.setPosition(xPos[i], yPos[i]);
            stats[i].box.setFillColor(colors[i]);

            stats[i].title.setFont(font);
            stats[i].title.setString(titles[i]);
            stats[i].title.setCharacterSize(20);
            stats[i].title.setFillColor(sf::Color(80, 80, 80));
            stats[i].title.setPosition(xPos[i] + 30, yPos[i] + 30);

            stats[i].value.setFont(font);
            stats[i].value.setString("0");
            stats[i].value.setCharacterSize(60);
            stats[i].value.setStyle(sf::Text::Bold);
            stats[i].value.setFillColor(Theme::TextDark);
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
                stats[i].box.setOutlineColor(sf::Color(255, 255, 255, 200));
            } else { stats[i].box.setOutlineThickness(0); }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    
    int handleClick(sf::Vector2f mousePos) {
        if(stats[0].box.getGlobalBounds().contains(mousePos)) return 1;
        if(stats[1].box.getGlobalBounds().contains(mousePos)) return 2;
        if(stats[2].box.getGlobalBounds().contains(mousePos)) return 3;
        if(stats[3].box.getGlobalBounds().contains(mousePos)) return 4;
        return 0;
    }

    void setUserRole(UserRole role, sf::Font& font) { 
        sidebar->setUserRole(role, font); 
    }
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
