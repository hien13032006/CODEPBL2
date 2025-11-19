#ifndef MANAGE_READERS_SCREEN_HPP
#define MANAGE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "LibrarySystem.h"

struct ReaderDisplayItem {
    sf::RectangleShape box;
    sf::Text idText;
    sf::Text nameText;
    sf::Text infoText;
    sf::Text statusText;
};

class ManageReadersScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    InputField* searchField;
    Button* searchButton;
    Button* viewAllButton;
    std::vector<ReaderDisplayItem*> readerItems;
    LibrarySystem* libSystem;

public:
    ManageReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Quan Ly Doc Gia");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));

        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(400, 50),
                                     "Tim kiem doc gia...", font);
        searchButton = new Button(sf::Vector2f(700, 100), sf::Vector2f(150, 50),
                                 "Tim", font);
        viewAllButton = new Button(sf::Vector2f(870, 100), sf::Vector2f(180, 50),
                                  "Xem Tat Ca", font, 0, sf::Color(100, 150, 200));

        loadReaders(font);
    }

    ~ManageReadersScreen() {
        delete sidebar;
        delete scrollView;
        delete searchField;
        delete searchButton;
        delete viewAllButton;
        for (auto item : readerItems) delete item;
    }

    void loadReaders(sf::Font& font) {
    for (auto item : readerItems) delete item;
    readerItems.clear();

    if (!libSystem) return;

    NodeReader* current = libSystem->getDanhSachDocGia();
    float yPos = 180;

    while (current != nullptr) {
        Reader* docGia = current->data;
        ReaderDisplayItem* item = new ReaderDisplayItem();
        
        item->box.setSize(sf::Vector2f(1000, 90));
        item->box.setPosition(300, yPos);
        item->box.setFillColor(sf::Color(25, 28, 40));
        item->box.setOutlineThickness(1);
        item->box.setOutlineColor(sf::Color(40, 43, 55));

        item->idText.setFont(font);
        item->idText.setString(docGia->getMaID());
        item->idText.setCharacterSize(18);
        item->idText.setFillColor(sf::Color(255, 193, 94));
        item->idText.setPosition(320, yPos + 15);

        item->nameText.setFont(font);
        item->nameText.setString(docGia->getHoTen());
        item->nameText.setCharacterSize(16);
        item->nameText.setFillColor(sf::Color::White);
        item->nameText.setPosition(420, yPos + 15);

        item->infoText.setFont(font);
        std::string info = "Email: " + docGia->getEmail() + " | SDT: " + docGia->getSDT();
        item->infoText.setString(info);
        item->infoText.setCharacterSize(13);
        item->infoText.setFillColor(sf::Color(150, 150, 150));
        item->infoText.setPosition(420, yPos + 40);

        int daMuon = docGia->DemSachDaMuon();
        int quaHan = docGia->DemSachQuaHan();
        
        item->statusText.setFont(font);
        std::string status = "Dang muon: " + std::to_string(daMuon) + "/" + 
                            std::to_string(docGia->getGioiHanMuon()) + " cuon";
        if (quaHan > 0) {
            status += " | QUA HAN: " + std::to_string(quaHan);
            item->statusText.setFillColor(sf::Color(255, 100, 100));
        } else {
            item->statusText.setFillColor(sf::Color(100, 180, 100));
        }
        item->statusText.setString(status);
        item->statusText.setCharacterSize(14);
        item->statusText.setPosition(420, yPos + 62);

        readerItems.push_back(item);
        yPos += 100;
        current = current->next;
    }

    scrollView->setMaxScroll(std::max(0.0f, yPos - 600.0f));
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchField->update();
        searchButton->update(mousePos);
        viewAllButton->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
        scrollView->handleScroll(event);
    }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (searchButton->handleClick(mousePos)) {
            // Tìm kiếm độc giả
        }
        if (viewAllButton->handleClick(mousePos)) {
            loadReaders(font); // TODO: Pass font properly
        }
    }
    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        searchField->draw(window);
        searchButton->draw(window);
        viewAllButton->draw(window);

        for (auto item : readerItems) {
            window.draw(item->box);
            window.draw(item->idText);
            window.draw(item->nameText);
            window.draw(item->infoText);
            window.draw(item->statusText);
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif