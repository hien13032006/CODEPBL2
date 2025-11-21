#ifndef OVERDUE_READERS_SCREEN_HPP
#define OVERDUE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "RoundedRectangle.hpp" 

struct OverdueReaderItem {
    RoundedRectangleShape box; 
    sf::Text idText; sf::Text nameText; sf::Text contactText; sf::Text overdueText;
};

class OverdueReadersScreen {
private:
    sf::RectangleShape background; sf::Text titleText; sf::Text emptyText;
    Sidebar* sidebar; ScrollView* scrollView;
    std::vector<OverdueReaderItem*> readerItems; LibrarySystem* libSystem;
    const float LIST_Y_START = 160.0f; const float VIEW_HEIGHT = 740.0f;

public:
    OverdueReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900)); background.setPosition(250, 0); background.setFillColor(sf::Color(13, 15, 23)); 
        titleText.setFont(font); titleText.setString("DOC GIA CO SACH QUA HAN"); 
        titleText.setCharacterSize(28); titleText.setFillColor(sf::Color(255, 80, 80)); titleText.setPosition(280, 40); 
        emptyText.setFont(font); emptyText.setString("Tuyet voi! Khong co doc gia nao qua han."); emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(100, 180, 100)); emptyText.setPosition(500, 400);
        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, LIST_Y_START, 1150, VIEW_HEIGHT));
        loadOverdueReaders(font);
    }

    ~OverdueReadersScreen() { delete sidebar; delete scrollView; for (auto item : readerItems) delete item; }

    void loadOverdueReaders(sf::Font& font) {
        for (auto item : readerItems) delete item; readerItems.clear();
        if (!libSystem) return;
        NodeReader* current = libSystem->getDanhSachDocGia();
        float itemY = LIST_Y_START; 
        const float ITEM_HEIGHT = 90.0f; const float GAP = 10.0f;

        while (current != nullptr) {
            Reader* docGia = current->data;
            int soQuaHan = docGia->DemSachQuaHan();
            if (soQuaHan > 0) {
                OverdueReaderItem* item = new OverdueReaderItem();
                item->box.setSize(sf::Vector2f(1000, ITEM_HEIGHT)); item->box.setCornerRadius(8.0f); 
                item->box.setPosition(300, itemY); item->box.setFillColor(sf::Color(45, 30, 35)); 
                item->box.setOutlineThickness(1.0f); item->box.setOutlineColor(sf::Color(200, 60, 60)); 

                item->idText.setFont(font); item->idText.setString(docGia->getMaID());
                item->idText.setCharacterSize(16); item->idText.setFillColor(sf::Color(255, 193, 94)); item->idText.setPosition(320, itemY + 15);

                item->nameText.setFont(font); item->nameText.setString(docGia->getHoTen());
                item->nameText.setCharacterSize(20); item->nameText.setFillColor(sf::Color::White); item->nameText.setPosition(420, itemY + 12);

                std::string c = docGia->getSDT() + " | " + docGia->getEmail(); if(c.size()>55) c=c.substr(0,52)+"...";
                item->contactText.setFont(font); item->contactText.setString(c); item->contactText.setCharacterSize(14); 
                item->contactText.setFillColor(sf::Color(180, 180, 180)); item->contactText.setPosition(420, itemY + 45);

                item->overdueText.setFont(font); item->overdueText.setString("QUA HAN: " + std::to_string(soQuaHan));
                item->overdueText.setCharacterSize(16); item->overdueText.setFillColor(sf::Color(255, 80, 80)); 
                sf::FloatRect b = item->overdueText.getLocalBounds(); item->overdueText.setPosition(1250 - b.width, itemY + 35);

                readerItems.push_back(item); itemY += ITEM_HEIGHT + GAP;
            }
            current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_Y_START)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        float scrollOffset = scrollView->getScrollOffset();
        if (mousePos.y > LIST_Y_START) {
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            for (auto item : readerItems) {
                if (item->box.getGlobalBounds().contains(adjustedMousePos)) { item->box.setFillColor(sf::Color(60, 40, 45)); item->box.setOutlineColor(sf::Color(255, 100, 100)); } 
                else { item->box.setFillColor(sf::Color(45, 30, 35)); item->box.setOutlineColor(sf::Color(200, 60, 60)); }
            }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); sidebar->draw(window); window.draw(titleText);

        if (readerItems.empty()) window.draw(emptyText);
        else {
            sf::View listView = window.getDefaultView();
            listView.setViewport(sf::FloatRect(250.f/1400.f, LIST_Y_START/900.f, 1150.f/1400.f, VIEW_HEIGHT/900.f));
            listView.setSize(1150, VIEW_HEIGHT);
            listView.setCenter(250 + 1150/2, LIST_Y_START + VIEW_HEIGHT/2 + scrollView->getScrollOffset());
            window.setView(listView);
            for (auto item : readerItems) {
                window.draw(item->box); window.draw(item->idText); window.draw(item->nameText);
                window.draw(item->contactText); window.draw(item->overdueText);
            }
            window.setView(window.getDefaultView());
        }
    }
};

#endif