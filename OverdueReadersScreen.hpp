#ifndef OVERDUE_READERS_SCREEN_HPP
#define OVERDUE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Reader.h"

struct OverdueReaderItem {
    sf::RectangleShape box;
    sf::Text idText;
    sf::Text nameText;
    sf::Text contactText;
    sf::Text overdueText;
};

class OverdueReadersScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    std::vector<OverdueReaderItem*> readerItems;
    LibrarySystem* libSystem;

public:
    OverdueReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Doc Gia Co Sach Qua Han");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color(255, 80, 80));
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Khong co doc gia nao qua han!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(100, 180, 100));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadOverdueReaders(font);
    }

    ~OverdueReadersScreen() {
        delete sidebar;
        delete scrollView;
        for (auto item : readerItems) delete item;
    }

    void loadOverdueReaders(sf::Font& font) {
        // Xóa items cũ
        for (auto item : readerItems) delete item;
        readerItems.clear();

        if (!libSystem) return;

        // Lấy danh sách độc giả từ LibrarySystem
        NodeReader* current = libSystem->getDanhSachDocGia();
        
        float yPos = 120;
        int count = 0;

        // Header bảng
        while (current != nullptr) {
            Reader* docGia = current->data;
            int soQuaHan = docGia->DemSachQuaHan();
            
            if (soQuaHan > 0) {
                OverdueReaderItem* item = new OverdueReaderItem();
                
                item->box.setSize(sf::Vector2f(1000, 80));
                item->box.setPosition(300, yPos);
                item->box.setFillColor(sf::Color(40, 30, 30));
                item->box.setOutlineThickness(2);
                item->box.setOutlineColor(sf::Color(200, 80, 80));

                item->idText.setFont(font);
                item->idText.setString(docGia->getMaID());
                item->idText.setCharacterSize(16);
                item->idText.setFillColor(sf::Color(255, 193, 94));
                item->idText.setPosition(320, yPos + 15);

                item->nameText.setFont(font);
                item->nameText.setString(docGia->getHoTen());
                item->nameText.setCharacterSize(16);
                item->nameText.setFillColor(sf::Color::White);
                item->nameText.setPosition(450, yPos + 15);

                item->contactText.setFont(font);
                std::string contact = docGia->getSDT() + " | " + docGia->getEmail();
                if (contact.length() > 40) contact = contact.substr(0, 40) + "...";
                item->contactText.setString(contact);
                item->contactText.setCharacterSize(13);
                item->contactText.setFillColor(sf::Color(150, 150, 150));
                item->contactText.setPosition(450, yPos + 40);

                item->overdueText.setFont(font);
                item->overdueText.setString("SO SACH QUA HAN: " + std::to_string(soQuaHan));
                item->overdueText.setCharacterSize(16);
                item->overdueText.setFillColor(sf::Color(255, 100, 100));
                item->overdueText.setPosition(900, yPos + 28);

                readerItems.push_back(item);
                yPos += 90;
                count++;
            }
            
            current = current->next;
        }

        scrollView->setMaxScroll(std::max(0.0f, yPos - 700.0f));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleScrollEvent(sf::Event& event) {
        scrollView->handleScroll(event);
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (readerItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : readerItems) {
                window.draw(item->box);
                window.draw(item->idText);
                window.draw(item->nameText);
                window.draw(item->contactText);
                window.draw(item->overdueText);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif