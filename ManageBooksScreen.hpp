#ifndef MANAGE_BOOKS_SCREEN_HPP
#define MANAGE_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"

class ManageBooksScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    Button* addButton;
    Button* viewAllButton;
    Button* searchButton;
    InputField* searchField;
    Modal* addBookModal;
    
    // Form thêm sách
    InputField* nameField;
    InputField* authorField;
    InputField* categoryField;
    InputField* yearField;
    InputField* publisherField;
    InputField* quantityField;
    Button* submitButton;
    Button* cancelButton;
    sf::Text formTitle;
    
    LibrarySystem* libSystem;
    std::vector<Button*> categoryButtons;

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Quan Ly Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        addButton = new Button(sf::Vector2f(280, 100), sf::Vector2f(200, 60),
                              "Them Sach Moi", font, 0, sf::Color(100, 180, 100));
        
        viewAllButton = new Button(sf::Vector2f(500, 100), sf::Vector2f(200, 60),
                                  "Xem Tat Ca", font, 0, sf::Color(100, 150, 200));
        
        searchField = new InputField(sf::Vector2f(280, 200), sf::Vector2f(400, 50),
                                     "Nhap ma sach de cap nhat/xoa...", font);
        
        searchButton = new Button(sf::Vector2f(700, 200), sf::Vector2f(150, 50),
                                 "Tim Kiem", font);

        // Modal thêm sách
        addBookModal = new Modal(font);

        formTitle.setFont(font);
        formTitle.setString("Them Sach Moi");
        formTitle.setCharacterSize(24);
        formTitle.setFillColor(sf::Color::White);
        formTitle.setPosition(600, 200);

        nameField = new InputField(sf::Vector2f(450, 270), sf::Vector2f(500, 45),
                                  "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, 330), sf::Vector2f(500, 45),
                                    "Tac gia", font);
        categoryField = new InputField(sf::Vector2f(450, 390), sf::Vector2f(500, 45),
                                      "The loai", font);
        yearField = new InputField(sf::Vector2f(450, 450), sf::Vector2f(240, 45),
                                  "Nam xuat ban", font);
        publisherField = new InputField(sf::Vector2f(710, 450), sf::Vector2f(240, 45),
                                       "Nha xuat ban", font);
        quantityField = new InputField(sf::Vector2f(450, 510), sf::Vector2f(240, 45),
                                      "So luong", font);

        submitButton = new Button(sf::Vector2f(450, 590), sf::Vector2f(240, 50),
                                 "Them Vao Thu Vien", font, 0, sf::Color(100, 180, 100));
        cancelButton = new Button(sf::Vector2f(710, 590), sf::Vector2f(240, 50),
                                 "Huy", font, 0, sf::Color(120, 120, 120));

        loadCategoryButtons(font);
    }

    ~ManageBooksScreen() {
        delete sidebar;
        delete addButton;
        delete viewAllButton;
        delete searchButton;
        delete searchField;
        delete addBookModal;
        delete nameField;
        delete authorField;
        delete categoryField;
        delete yearField;
        delete publisherField;
        delete quantityField;
        delete submitButton;
        delete cancelButton;
        for (auto btn : categoryButtons) delete btn;
    }

    void loadCategoryButtons(sf::Font& font) {
        std::vector<std::string> categories = {
            "Giao trinh", "Tham khao", "Tieu thuyet", 
            "Truyen ngan", "Tap chi", "Truyen tranh", "Sach ki nang"
        };

        float btnX = 280;
        float btnY = 300;
        int col = 0;

        for (const auto& cat : categories) {
            Button* btn = new Button(
                sf::Vector2f(btnX, btnY),
                sf::Vector2f(180, 50),
                cat,
                font,
                0,
                sf::Color(60, 100, 180)
            );
            categoryButtons.push_back(btn);

            col++;
            if (col == 5) {
                col = 0;
                btnX = 280;
                btnY += 70;
            } else {
                btnX += 200;
            }
        }
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        addButton->update(mousePos);
        viewAllButton->update(mousePos);
        searchButton->update(mousePos);
        searchField->update();

        if (addBookModal->isShown()) {
            nameField->update();
            authorField->update();
            categoryField->update();
            yearField->update();
            publisherField->update();
            quantityField->update();
            submitButton->update(mousePos);
            cancelButton->update(mousePos);
        } else {
            for (auto btn : categoryButtons) btn->update(mousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
        
        if (addBookModal->isShown()) {
            nameField->handleEvent(event, mousePos);
            authorField->handleEvent(event, mousePos);
            categoryField->handleEvent(event, mousePos);
            yearField->handleEvent(event, mousePos);
            publisherField->handleEvent(event, mousePos);
            quantityField->handleEvent(event, mousePos);
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) {
            if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) {
                addBookModal->hide();
                clearFields();
                return 0;
            }
            if (submitButton->handleClick(mousePos)) {
                return 1; // Thêm sách
            }
        } else {
            if (addButton->handleClick(mousePos)) {
                addBookModal->show();
                return 2;
            }
            if (viewAllButton->handleClick(mousePos)) {
                return 3; // Xem tất cả
            }
            if (searchButton->handleClick(mousePos)) {
                return 4; // Tìm kiếm để cập nhật/xóa
            }
            for (auto btn : categoryButtons) {
                if (btn->handleClick(mousePos)) {
                    return 5; // Xem theo thể loại
                }
            }
        }
        return 0;
    }

    void handleAddBook() {
        // Ghi vào file ThemSach.txt
        std::ofstream out("ThemSach.txt", std::ios::app);
        if (out.is_open()) {
            out << nameField->getText() << "|"
                << authorField->getText() << "|"
                << categoryField->getText() << "|"
                << yearField->getText() << "|"
                << publisherField->getText() << "|"
                << quantityField->getText() << "\n";
            out.close();
        }

        // Gọi hàm thêm sách
        libSystem->DocFileSach("ThemSach.txt");
        libSystem->GhiFileSach("DanhSachSach.txt");
        libSystem->GhiFileHeThong("DanhSachSach.txt");

        addBookModal->hide();
        clearFields();
    }

    void clearFields() {
        nameField->clear();
        authorField->clear();
        categoryField->clear();
        yearField->clear();
        publisherField->clear();
        quantityField->clear();
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (!addBookModal->isShown()) {
            addButton->draw(window);
            viewAllButton->draw(window);
            searchField->draw(window);
            searchButton->draw(window);

            for (auto btn : categoryButtons) {
                btn->draw(window);
            }
        } else {
            addBookModal->draw(window);
            window.draw(formTitle);
            nameField->draw(window);
            authorField->draw(window);
            categoryField->draw(window);
            yearField->draw(window);
            publisherField->draw(window);
            quantityField->draw(window);
            submitButton->draw(window);
            cancelButton->draw(window);
        }
    }

    std::string getSearchText() const { return searchField->getText(); }
    Sidebar* getSidebar() { return sidebar; }
};

#endif