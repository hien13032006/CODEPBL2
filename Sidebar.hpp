#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "USER.h"

struct MenuItem {
    sf::RectangleShape shape;
    sf::Text text;
    std::string label;
    bool isActive;
};

class Sidebar {
private:
    sf::RectangleShape background;
    sf::Text logo;
    std::vector<MenuItem> items;
    UserRole currentRole;

public:
    Sidebar(sf::Font& font) {
        background.setSize(sf::Vector2f(250, 900));
        background.setFillColor(sf::Color::White);
        
        logo.setFont(font);
        logo.setString("Library");
        logo.setCharacterSize(36);
        logo.setStyle(sf::Text::Bold);
        logo.setFillColor(sf::Color(41, 98, 255)); // Xanh dương
        logo.setPosition(40, 40);
        
        currentRole = UserRole::NONE;
        buildMenu(font);
    }

    void buildMenu(sf::Font& font) {
        items.clear();
        std::vector<std::string> labels;

        if (currentRole == UserRole::NONE) {
            labels = {"Home", "Top 10 sach", "Tat ca sach", "The loai", "Tim kiem", "Thanh vien"};
        } else if (currentRole == UserRole::READER) {
            labels = {"Home", "Top 10 sach", "Tat ca sach", "The loai", "Tim kiem", "Sach dang muon", "Lich su", "Thong tin", "Dang xuat"};
        } else if (currentRole == UserRole::LIBRARIAN) {
            labels = {"Home", "Top 10 sach", "Tat ca sach", "The loai", "Tim kiem", "Quan ly sach", "Quan ly doc gia", "Doc gia qua han", "Thong ke", "Thong tin", "Dang xuat"};
        }

        float y = 120;
        for (const auto& lbl : labels) {
            MenuItem item;
            item.label = lbl;
            item.isActive = false;
            
            item.shape.setSize(sf::Vector2f(230, 50));
            item.shape.setPosition(10, y);
            item.shape.setFillColor(sf::Color::White);
            item.shape.setOutlineThickness(0);

            item.text.setFont(font);
            item.text.setString(lbl);
            item.text.setCharacterSize(18); // Chữ thường 18
            item.text.setFillColor(sf::Color(80, 80, 80));
            item.text.setPosition(30, y + 12);

            items.push_back(item);
            y += 55;
        }
    }

    void setUserRole(UserRole role, sf::Font& font) {
        currentRole = role;
        buildMenu(font);
    }

    void update(sf::Vector2f mousePos) {
        for (auto& item : items) {
            if (item.shape.getGlobalBounds().contains(mousePos)) {
                item.shape.setFillColor(sf::Color(240, 245, 255)); // Hover xanh nhạt
                item.text.setFillColor(sf::Color(41, 98, 255));
            } else {
                item.shape.setFillColor(sf::Color::White);
                item.text.setFillColor(sf::Color(80, 80, 80));
            }
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].shape.getGlobalBounds().contains(mousePos)) {
                return i;
            }
        }
        return -1;
    }

    std::string getMenuLabel(int index) const {
        if (index >= 0 && index < items.size()) return items[index].label;
        return "";
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(logo);
        for (const auto& item : items) {
            window.draw(item.shape);
            window.draw(item.text);
        }
    }
};

#endif