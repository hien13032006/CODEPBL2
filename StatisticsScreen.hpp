#ifndef STATISTICS_SCREEN_HPP
#define STATISTICS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Sidebar.hpp"
#include "LibrarySystem.h"

class StatisticsScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    
    // Các thống kê
    sf::RectangleShape statBox1; // 1. Tổng số sách
    sf::RectangleShape statBox2; // 2. Tổng độc giả
    sf::RectangleShape statBox3; // 3. Sách đang mượn
    sf::RectangleShape statBox4; // 4. Sách quá hạn
    
    sf::Text stat1Title;
    sf::Text stat1Value;
    sf::Text stat2Title;
    sf::Text stat2Value;
    sf::Text stat3Title;
    sf::Text stat3Value;
    sf::Text stat4Title;
    sf::Text stat4Value;
    
    LibrarySystem* libSystem;

public:
    StatisticsScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Thong Ke Thu Vien");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        // Box 1: Tổng số sách
        statBox1.setSize(sf::Vector2f(450, 150));
        statBox1.setPosition(300, 120);
        statBox1.setFillColor(sf::Color(60, 100, 180));

        stat1Title.setFont(font);
        stat1Title.setString("Tong So Sach");
        stat1Title.setCharacterSize(18);
        stat1Title.setFillColor(sf::Color(200, 200, 200));
        stat1Title.setPosition(320, 140);

        stat1Value.setFont(font);
        stat1Value.setString("1,250");
        stat1Value.setCharacterSize(48);
        stat1Value.setFillColor(sf::Color::White);
        stat1Value.setPosition(320, 180);

        // Box 2: Tổng độc giả
        statBox2.setSize(sf::Vector2f(450, 150));
        statBox2.setPosition(780, 120);
        statBox2.setFillColor(sf::Color(100, 180, 100));

        stat2Title.setFont(font);
        stat2Title.setString("Tong Doc Gia");
        stat2Title.setCharacterSize(18);
        stat2Title.setFillColor(sf::Color(200, 200, 200));
        stat2Title.setPosition(800, 140);

        stat2Value.setFont(font);
        stat2Value.setString("567");
        stat2Value.setCharacterSize(48);
        stat2Value.setFillColor(sf::Color::White);
        stat2Value.setPosition(800, 180);

        // Box 3: Sách đang mượn
        statBox3.setSize(sf::Vector2f(450, 150));
        statBox3.setPosition(300, 300);
        statBox3.setFillColor(sf::Color(200, 150, 60));

        stat3Title.setFont(font);
        stat3Title.setString("Sach Dang Muon");
        stat3Title.setCharacterSize(18);
        stat3Title.setFillColor(sf::Color(200, 200, 200));
        stat3Title.setPosition(320, 320);

        stat3Value.setFont(font);
        stat3Value.setString("342");
        stat3Value.setCharacterSize(48);
        stat3Value.setFillColor(sf::Color::White);
        stat3Value.setPosition(320, 360);

        // Box 4: Sách quá hạn
        statBox4.setSize(sf::Vector2f(450, 150));
        statBox4.setPosition(780, 300);
        statBox4.setFillColor(sf::Color(200, 80, 80));

        stat4Title.setFont(font);
        stat4Title.setString("Sach Qua Han");
        stat4Title.setCharacterSize(18);
        stat4Title.setFillColor(sf::Color(200, 200, 200));
        stat4Title.setPosition(800, 320);

        stat4Value.setFont(font);
        stat4Value.setString("23");
        stat4Value.setCharacterSize(48);
        stat4Value.setFillColor(sf::Color::White);
        stat4Value.setPosition(800, 360);

        loadStatistics();
    }

    ~StatisticsScreen() {
        delete sidebar;
    }

    void loadStatistics() {
    if (!libSystem) return;
    int tongSoSach = 0;
    int tongSoLuong = 0;
    int tongDocGia = 0;
    int soSachDangMuon = 0;
    int soSachQuaHan = 0;

    // Đếm sách
    NodeBook* currentBook = libSystem->getDanhSachSach();
    while (currentBook != nullptr) {
        tongSoSach++;
        tongSoLuong += currentBook->data->getSoLuong();
        currentBook = currentBook->next;
    }

    // Đếm độc giả và sách đang mượn
    NodeReader* currentReader = libSystem->getDanhSachDocGia();
    while (currentReader != nullptr) {
        tongDocGia++;
        soSachDangMuon += currentReader->data->DemSachDaMuon();
        soSachQuaHan += currentReader->data->DemSachQuaHan();
        currentReader = currentReader->next;
    }

    // Cập nhật UI
    stat1Value.setString(std::to_string(tongSoSach));
    stat2Value.setString(std::to_string(tongDocGia));
    stat3Value.setString(std::to_string(soSachDangMuon));
    stat4Value.setString(std::to_string(soSachQuaHan));
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }
    
    // FIX: Thêm hàm xử lý click trên các ô thống kê
    int handleClick(sf::Vector2f mousePos) {
        if (statBox1.getGlobalBounds().contains(mousePos)) return 1; // Tổng số sách -> HOME
        if (statBox2.getGlobalBounds().contains(mousePos)) return 2; // Tổng độc giả -> MANAGE_READERS
        if (statBox3.getGlobalBounds().contains(mousePos)) return 3; // Sách đang mượn
        if (statBox4.getGlobalBounds().contains(mousePos)) return 4; // Sách quá hạn -> OVERDUE_READERS
        return 0;
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        window.draw(statBox1);
        window.draw(stat1Title);
        window.draw(stat1Value);

        window.draw(statBox2);
        window.draw(stat2Title);
        window.draw(stat2Value);

        window.draw(statBox3);
        window.draw(stat3Title);
        window.draw(stat3Value);

        window.draw(statBox4);
        window.draw(stat4Title);
        window.draw(stat4Value);
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif