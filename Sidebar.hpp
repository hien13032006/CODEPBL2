// ============================================
// ui/Sidebar.hpp - LIGHT THEME + FIXED MENU
// ============================================
#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "State.hpp"
#include "USER.h"

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
        // LIGHT THEME: Sidebar trắng
        background.setFillColor(sf::Color(255, 255, 255));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(32);
        // Logo màu xanh dương đậm
        logoText.setFillColor(sf::Color(41, 98, 255));
        logoText.setPosition(30, 30);

        activeIndex = 0;
        currentRole = UserRole::NONE;

        buildMenu(font);
    }

    void buildMenu(sf::Font& font) {
        menuItems.clear();
        std::vector<std::string> labels;

        // FIX: Menu đầy đủ với các mục rõ ràng
        if (currentRole == UserRole::NONE) {
            labels = {
                "Home", 
                "Top 10 sach",      // Mục riêng cho Top 10
                "Tat ca sach",      // Mục riêng cho tất cả sách
                "The loai", 
                "Tim kiem", 
                "Thanh vien"
            };
        } else if (currentRole == UserRole::READER) {
            labels = {
                "Home", 
                "Top 10 sach", 
                "Tat ca sach",
                "The loai",
                "Tim kiem", 
                "Sach dang muon", 
                "Lich su", 
                "Dang xuat"
            };
        } else if (currentRole == UserRole::LIBRARIAN) {
            labels = {
                "Home", 
                "Top 10 sach", 
                "Tat ca sach",
                "The loai",
                "Tim kiem", 
                "Quan ly sach",      // Chức năng quản lý sách
                "Quan ly doc gia", 
                "Doc gia qua han", 
                "Thong ke", 
                "Dang xuat"
            };
        }

        float yPos = 120;
        for (size_t i = 0; i < labels.size(); i++) {
            MenuItem item;
            item.label = labels[i];
            item.id = i;
            item.isActive = (i == 0);
            
            item.shape.setSize(sf::Vector2f(220, 45));
            item.shape.setPosition(15, yPos);
            
            // LIGHT THEME: Menu item colors
            if (item.isActive) {
                item.shape.setFillColor(sf::Color(240, 242, 255)); // Xanh nhạt khi active
            } else {
                item.shape.setFillColor(sf::Color(255, 255, 255)); // Trắng
            }

            item.text.setFont(font);
            item.text.setString(labels[i]);
            item.text.setCharacterSize(14);
            // Text màu đen/xám đậm
            item.text.setFillColor(sf::Color(60, 60, 60));
            item.text.setPosition(30, yPos + 14);

            menuItems.push_back(item);
            yPos += 52;
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
                    // LIGHT THEME: Hover effect
                    item.shape.setFillColor(sf::Color(235, 238, 250));
                }
            } else {
                if (!item.isActive) {
                    item.shape.setFillColor(sf::Color(255, 255, 255));
                }
            }
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (menuItems[i].shape.getGlobalBounds().contains(mousePos)) {
                // Reset all items
                for (auto& item : menuItems) {
                    item.isActive = false;
                    item.shape.setFillColor(sf::Color(255, 255, 255));
                }
                
                // Set clicked item as active
                menuItems[i].isActive = true;
                menuItems[i].shape.setFillColor(sf::Color(240, 242, 255));
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