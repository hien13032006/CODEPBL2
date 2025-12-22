#ifndef OVERDUE_READERS_SCREEN_HPP
#define OVERDUE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "RoundedRectangle.hpp" 
#include "Theme.hpp"

struct OverdueReaderItem {
    RoundedRectangleShape box; 
    sf::Text idText, nameText, contactText, overdueText; 
};

class OverdueReadersScreen {
private:
    sf::RenderWindow* windowRef;
    sf::RectangleShape background; 
    sf::Text titleText, emptyText;
    Sidebar* sidebar; 
    ScrollView* scrollView;
    std::vector<OverdueReaderItem*> readerItems; 
    LibrarySystem* libSystem;
    const float LIST_Y_START = 120.0f;
    const float VIEW_HEIGHT = 600.0f;
    sf::View getListView() { 
        if (!windowRef) 
            return sf::View(); 
        sf::View view = windowRef->getDefaultView(); 
        view.setViewport(sf::FloatRect(250.f/1300.f, LIST_Y_START/720.f, 1050.f/1300.f, VIEW_HEIGHT/720.f)); 
        view.setSize(1050, VIEW_HEIGHT); 
        view.setCenter(250 + 1050/2, LIST_Y_START + VIEW_HEIGHT/2 + scrollView->getScrollOffset()); 
        return view;
    }

public:
    OverdueReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        windowRef = nullptr;
        background.setSize(sf::Vector2f(1300, 720));
        background.setPosition(250, 0); 
        background.setFillColor(Theme::Background); 
        titleText.setFont(font);
        titleText.setString("DOC GIA CO SACH QUA HAN");
        titleText.setCharacterSize(30); 
        titleText.setFillColor(Theme::Error);
        titleText.setPosition(280, 40); 
        emptyText.setFont(font);
        emptyText.setString("Tuyet voi! Khong co doc gia nao qua han.");
        emptyText.setCharacterSize(20); 
        emptyText.setFillColor(Theme::Success);
        emptyText.setPosition(500, 300);
        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, LIST_Y_START, 1050, VIEW_HEIGHT));
        loadOverdueReaders(font);
    }
    ~OverdueReadersScreen() {
        delete sidebar;
        delete scrollView; 
        for (auto item : readerItems) 
            delete item; 
    }
    void setWindow(sf::RenderWindow* win) { windowRef = win; }

    void loadOverdueReaders(sf::Font& font) {
        for (auto item : readerItems) 
            delete item;
        readerItems.clear();
        if (!libSystem) 
            return;
        NodeReader* current = libSystem->getDanhSachDocGia(); 
        float itemY = LIST_Y_START;
        const float ITEM_HEIGHT = 100.0f; 
        const float GAP = 15.0f;
        while (current != nullptr) {
            Reader* docGia = current->data; 
            int soQuaHan = docGia->DemSachQuaHan();
            if (soQuaHan > 0) {
                OverdueReaderItem* item = new OverdueReaderItem();
                item->box.setSize(sf::Vector2f(980, ITEM_HEIGHT));
                item->box.setCornerRadius(12.0f); 
                item->box.setPosition(275, itemY);
                item->box.setFillColor(sf::Color::White); 
                item->box.setOutlineThickness(1.5f); 
                item->box.setOutlineColor(Theme::Error);
                item->idText.setFont(font); 
                item->idText.setString(docGia->getMaID());
                item->idText.setCharacterSize(18);
                item->idText.setFillColor(Theme::Primary); 
                item->idText.setPosition(305, itemY + 15);
                item->nameText.setFont(font);
                item->nameText.setString(docGia->getHoTen()); 
                item->nameText.setCharacterSize(22);
                item->nameText.setFillColor(Theme::TextDark); 
                item->nameText.setPosition(420, itemY + 12);
                std::string c = docGia->getSDT() + " | " + docGia->getEmail();
                if(c.size()>60)
                    c=c.substr(0,57)+"...";
                item->contactText.setFont(font); 
                item->contactText.setString(c);
                item->contactText.setCharacterSize(16);
                item->contactText.setFillColor(Theme::TextLight);
                item->contactText.setPosition(420, itemY + 50);
                item->overdueText.setFont(font);
                item->overdueText.setString("QUA HAN: " + std::to_string(soQuaHan));
                item->overdueText.setCharacterSize(18);
                item->overdueText.setFillColor(Theme::Error);
                sf::FloatRect b = item->overdueText.getLocalBounds();
                item->overdueText.setPosition(1200 - b.width, itemY + 40);
                readerItems.push_back(item);
                itemY += ITEM_HEIGHT + GAP;
            } current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_Y_START + 600.0f));
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        if (!windowRef) return; 
        sidebar->update(mousePos);
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
        if (pixelMouse.y > LIST_Y_START) {
            sf::View listView = getListView(); 
            sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); 
            for (auto item : readerItems) { 
                if (item->box.getGlobalBounds().contains(listMousePos)) {
                    item->box.setFillColor(sf::Color(255, 245, 245)); 
                    item->box.setOutlineThickness(2.5f);
                } else { 
                    item->box.setFillColor(sf::Color::White);
                    item->box.setOutlineThickness(1.5f); 
                } 
            }
        }
    }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) { scrollView->handleScroll(event, mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }
    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); 
        sidebar->draw(window); 
        window.draw(titleText);
        if (readerItems.empty()) {
            window.draw(emptyText);
        } else { 
            window.setView(getListView());
            for (auto item : readerItems) {
                window.draw(item->box);
                window.draw(item->idText);
                window.draw(item->nameText);
                window.draw(item->contactText);
                window.draw(item->overdueText); 
            } 
            window.setView(window.getDefaultView());
        } 
    }
};
#endif
