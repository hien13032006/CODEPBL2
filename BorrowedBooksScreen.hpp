#ifndef BORROWED_BOOKS_SCREEN_HPP
#define BORROWED_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "Reader.h"

struct BorrowedBookItem {
    sf::RectangleShape box;
    sf::Text titleText;
    sf::Text infoText;
    sf::Text statusText;
    Button* returnButton;
};

class BorrowedBooksScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    std::vector<BorrowedBookItem*> bookItems;
    Reader* currentReader;

public:
    BorrowedBooksScreen(sf::Font& font, Reader* reader) : currentReader(reader) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Sach Dang Muon");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Ban chua muon sach nao!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        
        loadBorrowedBooks(font);
    }

    ~BorrowedBooksScreen() {
        delete sidebar;
        for (auto item : bookItems) {
            delete item->returnButton;
            delete item;
        }
    }

    void loadBorrowedBooks(sf::Font& font) {
    for (auto item : bookItems) {
        delete item->returnButton;
        delete item;
    }
    bookItems.clear();

    if (!currentReader) return;

    // Đọc danh sách phiếu mượn từ Reader
    NodeMuonSach* current = currentReader->getDanhSachPhieuMuon();
    
    float yPos = 120;
    int index = 0;

    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        BorrowedBookItem* item = new BorrowedBookItem();
        
        item->box.setSize(sf::Vector2f(1000, 100));
        item->box.setPosition(300, yPos);
        item->box.setFillColor(sf::Color(25, 28, 40));
        item->box.setOutlineThickness(1);
        item->box.setOutlineColor(sf::Color(40, 43, 55));

        item->titleText.setFont(font);
        item->titleText.setString(phieu->sach->getTenSach());
        item->titleText.setCharacterSize(18);
        item->titleText.setFillColor(sf::Color::White);
        item->titleText.setPosition(320, yPos + 15);

        // Format ngày mượn và hạn trả
        char dateBuffer[50];
        tm* tmMuon = localtime(&phieu->ngayMuon);
        tm* tmHan = localtime(&phieu->ngayHetHan);
        sprintf(dateBuffer, "Ma: %s | Ngay muon: %02d/%02d/%04d | Han tra: %02d/%02d/%04d",
                phieu->sach->getMaSach().c_str(),
                tmMuon->tm_mday, tmMuon->tm_mon + 1, tmMuon->tm_year + 1900,
                tmHan->tm_mday, tmHan->tm_mon + 1, tmHan->tm_year + 1900);

        item->infoText.setFont(font);
        item->infoText.setString(dateBuffer);
        item->infoText.setCharacterSize(13);
        item->infoText.setFillColor(sf::Color(150, 150, 150));
        item->infoText.setPosition(320, yPos + 40);

        item->statusText.setFont(font);
        std::string status = phieu->trangThaiHan();
        item->statusText.setString(status);
        item->statusText.setCharacterSize(14);
        
        if (phieu->daQuaHan()) {
            item->statusText.setFillColor(sf::Color(255, 100, 100));
            item->box.setOutlineColor(sf::Color(200, 80, 80));
        } else {
            item->statusText.setFillColor(sf::Color(100, 180, 100));
        }
        item->statusText.setPosition(320, yPos + 65);

        item->returnButton = new Button(
            sf::Vector2f(1100, yPos + 25),
            sf::Vector2f(150, 50),
            "Tra Sach",
            font,
            index,
            sf::Color(100, 150, 200)
        );
        
        // Lưu mã sách vào button
        item->returnButton->setOnClick([this, phieu]() {
            // Callback sẽ được xử lý ở App
        });

        bookItems.push_back(item);
        yPos += 120;
        index++;
        current = current->next;
    }
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        for (auto item : bookItems) {
            item->returnButton->update(mousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    std::string handleReturnClick(sf::Vector2f mousePos) {
        for (auto item : bookItems) {
            if (item->returnButton->handleClick(mousePos)) {
                // Trả về ID sách để xử lý
                return "BOOK_ID_" + std::to_string(item->returnButton->getId());
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (bookItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : bookItems) {
                window.draw(item->box);
                window.draw(item->titleText);
                window.draw(item->infoText);
                window.draw(item->statusText);
                item->returnButton->draw(window);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif