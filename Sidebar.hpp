#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "State.hpp"
#include"USER.h"

struct MenuItem {
    std::string label;
    sf::RectangleShape shape;
    sf::Text text;
    bool isActive;
    int id;
};

class Sidebar {
private:
    sf::RectangleShape background;
    sf::Text logoText;
    std::vector<MenuItem> menuItems;
    int activeIndex;
    UserRole currentRole;

public:
    Sidebar(sf::Font& font) {
        background.setSize(sf::Vector2f(250, 900));
        background.setPosition(0, 0);
        background.setFillColor(sf::Color(20, 23, 35));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(32);
        logoText.setFillColor(sf::Color(255, 193, 94));
        logoText.setPosition(30, 30);

        activeIndex = 0;
        currentRole = UserRole::NONE;

        buildMenu(font);
    }

    void buildMenu(sf::Font& font) {
        menuItems.clear();
        std::vector<std::string> labels;

        // Menu theo phân quyền
        if (currentRole == UserRole::NONE) {
            labels = {"Home", "Top 10 sach", "Danh muc sach", "The loai", 
                     "Tim kiem", "Thanh vien"};
        } else if (currentRole == UserRole::READER) {
            labels = {"Home", "The doc gia", "Cap nhat thong tin ca nhan", "Top 10 sach", "Danh muc sach", "The loai",
                     "Tim kiem", "Muon sach", "Tra sach", "Sach dang muon", "Lich su muon tra", "Dang xuat"};
        } else if (currentRole == UserRole::LIBRARIAN) {
            labels = {"Home", "The thu thu","Cap nhat thong tin ca nhan", "Top 10 sach", "Danh muc sach", "The loai",
                     "Tim kiem","Them sach", "Xoa sach", "Cap nhat sach", "Thong ke", "Quan ly doc gia", 
                     "Dang xuat"};
        }

        float yPos = 120;
        for (size_t i = 0; i < labels.size(); i++) {
            MenuItem item;
            item.label = labels[i];
            item.id = i;
            item.isActive = (i == 0);
            
            item.shape.setSize(sf::Vector2f(220, 45));
            item.shape.setPosition(15, yPos);
            item.shape.setFillColor(item.isActive ? 
                sf::Color(40, 43, 55) : sf::Color(20, 23, 35));

            item.text.setFont(font);
            item.text.setString(labels[i]);
            item.text.setCharacterSize(15);
            item.text.setFillColor(sf::Color(200, 200, 200));
            item.text.setPosition(30, yPos + 12);

            menuItems.push_back(item);
            yPos += 55;
        }
    }

    void setUserRole(UserRole role, sf::Font& font) {
        currentRole = role;
        buildMenu(font);
    }

    void update(sf::Vector2f mousePos) {
        for (auto& item : menuItems) {
            if (item.shape.getGlobalBounds().contains(mousePos)) {
                if (!item.isActive) {
                    item.shape.setFillColor(sf::Color(35, 38, 50));
                }
            } else {
                if (!item.isActive) {
                    item.shape.setFillColor(sf::Color(20, 23, 35));
                }
            }
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (menuItems[i].shape.getGlobalBounds().contains(mousePos)) {
                for (auto& item : menuItems) {
                    item.isActive = false;
                    item.shape.setFillColor(sf::Color(20, 23, 35));
                }
                menuItems[i].isActive = true;
                menuItems[i].shape.setFillColor(sf::Color(40, 43, 55));
                activeIndex = i;
                return i;
            }
        }
        return -1;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(logoText);
        for (const auto& item : menuItems) {
            window.draw(item.shape);
            window.draw(item.text);
        }
    }

    UserRole getUserRole() const { return currentRole; }
    std::string getMenuLabel(int index) const {
        if (index >= 0 && index < (int)menuItems.size()) {
            return menuItems[index].label;
        }
        return "";
    }
};

#endif