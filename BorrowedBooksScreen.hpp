#ifndef BORROWED_BOOKS_SCREEN_HPP
#define BORROWED_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "Reader.h"
#include "Book.h"
#include "ScrollView.hpp"
#include "RoundedRectangle.hpp"

struct BorrowedBookItem {
    RoundedRectangleShape box;
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
    ScrollView* scrollView;

    // Các hằng số kích thước
    const float LIST_Y_START = 120.0f;
    const float VIEW_HEIGHT = 780.0f; // 900 - 120

public:
    BorrowedBooksScreen(sf::Font& font, Reader* reader) : currentReader(reader) {
        // 1. Background
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23)); // Nền tối

        // 2. Title
        titleText.setFont(font);
        titleText.setString("SACH DANG MUON"); 
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Ban chua muon sach nao!");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        
        // 3. ScrollView
        scrollView = new ScrollView(sf::FloatRect(250, LIST_Y_START, 1150, VIEW_HEIGHT));
        
        loadBorrowedBooks(font);
    }

    ~BorrowedBooksScreen() {
        delete sidebar;
        delete scrollView;
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

        NodeMuonSach* current = currentReader->getDanhSachPhieuMuon();
        
        float itemY = LIST_Y_START; // Bắt đầu vẽ từ vị trí khớp với vùng View
        const float ITEM_HEIGHT = 120.0f; // Ô to (120px)
        const float GAP = 15.0f;
        int index = 0;

        while (current != nullptr) {
            PhieuMuonSach* phieu = current->phieu;
            BorrowedBookItem* item = new BorrowedBookItem();
            
            // Box nền
            item->box.setSize(sf::Vector2f(1000, ITEM_HEIGHT));
            item->box.setCornerRadius(12.0f); 
            item->box.setPosition(300, itemY);
            item->box.setFillColor(sf::Color(30, 33, 45)); 

            // 1. Tên sách (Size 24, Trắng)
            item->titleText.setFont(font);
            std::string tName = phieu->sach->getTenSach();
            if (tName.length() > 35) tName = tName.substr(0, 32) + "...";
            item->titleText.setString(tName);
            item->titleText.setCharacterSize(24);
            item->titleText.setFillColor(sf::Color::White);
            item->titleText.setPosition(330, itemY + 20);

            // 2. Thông tin (Size 16, Xám)
            char dateBuffer[100];
            tm* tmMuon = localtime(&phieu->ngayMuon);
            tm* tmHan = localtime(&phieu->ngayHetHan);
            sprintf(dateBuffer, "Ma: %s  |  Ngay muon: %02d/%02d/%04d  |  Han tra: %02d/%02d/%04d",
                    phieu->sach->getMaSach().c_str(),
                    tmMuon->tm_mday, tmMuon->tm_mon + 1, tmMuon->tm_year + 1900,
                    tmHan->tm_mday, tmHan->tm_mon + 1, tmHan->tm_year + 1900);

            item->infoText.setFont(font);
            item->infoText.setString(dateBuffer);
            item->infoText.setCharacterSize(16);
            item->infoText.setFillColor(sf::Color(180, 180, 180));
            item->infoText.setPosition(330, itemY + 55);

            // 3. Trạng thái (Size 16)
            item->statusText.setFont(font);
            std::string status = phieu->trangThaiHan();
            item->statusText.setString(status);
            item->statusText.setCharacterSize(16);
            
            if (phieu->daQuaHan()) {
                item->statusText.setFillColor(sf::Color(255, 80, 80));
                item->box.setOutlineThickness(1.5f);
                item->box.setOutlineColor(sf::Color(200, 60, 60));
            } else {
                item->statusText.setFillColor(sf::Color(100, 220, 100));
            }
            item->statusText.setPosition(330, itemY + 85);

            // 4. Nút Trả (Nổi bật)
            item->returnButton = new Button(
                sf::Vector2f(1130, itemY + 35),
                sf::Vector2f(120, 50),
                "Tra Sach",
                font,
                index, // Lưu index để truy xuất
                sf::Color(60, 100, 180)
            );

            bookItems.push_back(item);
            itemY += ITEM_HEIGHT + GAP;
            index++;
            current = current->next;
        }
        
        // Set Max Scroll
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_Y_START));
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        
        float scrollOffset = scrollView->getScrollOffset();
        
        // Chỉ xử lý khi chuột nằm trong vùng danh sách
        if (mousePos.y > LIST_Y_START) {
            // Tính tọa độ thực tế trong list = MouseY + ScrollOffset
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            
            for (auto item : bookItems) {
                item->returnButton->update(adjustedMousePos);
            }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    std::string handleReturnClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        
        if (mousePos.y > LIST_Y_START) {
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            
            for (auto item : bookItems) {
                if (item->returnButton->handleClick(adjustedMousePos)) {
                    // Lấy mã sách dựa trên index nút bấm
                    int idx = item->returnButton->getId();
                    NodeMuonSach* temp = currentReader->getDanhSachPhieuMuon();
                    for(int i=0; i<idx && temp!=nullptr; ++i) temp = temp->next;
                    
                    if(temp) return "BOOK_ID_" + temp->phieu->sach->getMaSach();
                }
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        // 1. Tĩnh
        window.setView(window.getDefaultView());
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (bookItems.empty()) {
            window.draw(emptyText);
        } else {
            // 2. Danh sách cuộn (Dùng Viewport chuẩn để không giãn chữ)
            sf::View listView = window.getDefaultView();
            
            // Viewport: X=250, Y=120, W=1150, H=780 (Tỉ lệ 0-1)
            listView.setViewport(sf::FloatRect(250.f/1400.f, LIST_Y_START/900.f, 1150.f/1400.f, VIEW_HEIGHT/900.f));
            
            // Kích thước thực tế (Pixel)
            listView.setSize(1150, VIEW_HEIGHT);
            
            // Tâm Camera: Y = Start + Height/2 + Offset
            listView.setCenter(250 + 1150/2, LIST_Y_START + VIEW_HEIGHT/2 + scrollView->getScrollOffset());
            
            window.setView(listView);

            for (auto item : bookItems) {
                window.draw(item->box);
                window.draw(item->titleText);
                window.draw(item->infoText);
                window.draw(item->statusText);
                item->returnButton->draw(window);
            }
            
            // Reset View
            window.setView(window.getDefaultView());
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif