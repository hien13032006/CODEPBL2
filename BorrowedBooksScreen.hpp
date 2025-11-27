#ifndef BORROWED_BOOKS_SCREEN_HPP
#define BORROWED_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cmath> // Cần cho hàm sin/cos vẽ sao
#include "Sidebar.hpp"
#include "Button.hpp"
#include "Reader.h"
#include "Book.h"
#include "ScrollView.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"
#include "LibrarySystem.h" 

struct BorrowedBookItem { 
    RoundedRectangleShape box; 
    sf::Text titleText, infoText, statusText; 
    Button* rateButton; 
    sf::ConvexShape starIcon; // [MỚI] Hình ngôi sao vẽ bằng code
};

class BorrowedBooksScreen {
private:
    sf::RectangleShape background; 
    sf::Text titleText, emptyText; 
    Sidebar* sidebar;
    std::vector<BorrowedBookItem*> bookItems; 
    Reader* currentReader; 
    ScrollView* scrollView;
    LibrarySystem* libSystem; 
    const float LIST_Y_START = 120.0f; 
    const float VIEW_HEIGHT = 600.0f;

    // [MỚI] Hàm tạo hình ngôi sao (Copy từ RatingBookScreen sang)
    sf::ConvexShape createStarShape(float x, float y, float radius) {
        sf::ConvexShape star;
        star.setPointCount(10); 
        float angle = -3.14159 / 2; 
        float step = 3.14159 / 5;   
        float innerRadius = radius * 0.45f; 

        for (int i = 0; i < 10; i++) {
            float r = (i % 2 == 0) ? radius : innerRadius;
            star.setPoint(i, sf::Vector2f(std::cos(angle) * r, std::sin(angle) * r));
            angle += step;
        }
        // Đặt vị trí
        star.setPosition(x, y);
        return star;
    }

public:
    BorrowedBooksScreen(sf::Font& font, Reader* reader, LibrarySystem* lib = nullptr) 
        : currentReader(reader), libSystem(lib) {
        background.setSize(sf::Vector2f(1300, 720)); 
        background.setPosition(250, 0); 
        background.setFillColor(Theme::Background);
        
        titleText.setFont(font); 
        titleText.setString("SACH DANG MUON"); 
        titleText.setCharacterSize(32); 
        titleText.setFillColor(Theme::TextDark); 
        titleText.setPosition(280, 30);
        
        emptyText.setFont(font); 
        emptyText.setString("Ban chua muon sach nao!"); 
        emptyText.setCharacterSize(20); 
        emptyText.setFillColor(Theme::TextLight); 
        emptyText.setPosition(550, 300);
        
        sidebar = new Sidebar(font); 
        scrollView = new ScrollView(sf::FloatRect(250, LIST_Y_START, 1050, VIEW_HEIGHT));
        
        loadBorrowedBooks(font);
    }

    ~BorrowedBooksScreen() { 
        delete sidebar; delete scrollView; 
        for (auto item : bookItems) { delete item->rateButton; delete item; } 
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    void loadBorrowedBooks(sf::Font& font) {
        for (auto item : bookItems) { delete item->rateButton; delete item; } 
        bookItems.clear(); 
        if (!currentReader) return;

        NodeMuonSach* current = currentReader->getDanhSachPhieuMuon(); 
        float itemY = LIST_Y_START; 
        const float ITEM_HEIGHT = 120.0f; 
        int index = 0;

        while (current != nullptr) {
            PhieuMuonSach* phieu = current->phieu; 
            BorrowedBookItem* item = new BorrowedBookItem();
            
            item->box.setSize(sf::Vector2f(980, ITEM_HEIGHT)); 
            item->box.setCornerRadius(12.0f); 
            item->box.setPosition(275, itemY); 
            item->box.setFillColor(sf::Color::White); 
            item->box.setOutlineThickness(1); 
            
            item->titleText.setFont(font); 
            std::string tName = phieu->sach->getTenSach(); 
            if (tName.length() > 35) tName = tName.substr(0, 32) + "..."; 
            item->titleText.setString(tName); 
            item->titleText.setCharacterSize(24); 
            item->titleText.setFillColor(Theme::TextDark); 
            item->titleText.setPosition(305, itemY + 20);

            struct tm tmMuon, tmHan;
            tm* temp = localtime(&phieu->ngayMuon); if (temp) tmMuon = *temp;
            temp = localtime(&phieu->ngayHetHan); if (temp) tmHan = *temp;

            char dateBuffer[120]; 
            sprintf(dateBuffer, "Ma: %s  |  Ngay muon: %02d/%02d/%04d  |  Han tra: %02d/%02d/%04d", 
                    phieu->sach->getMaSach().c_str(), 
                    tmMuon.tm_mday, tmMuon.tm_mon + 1, tmMuon.tm_year + 1900, 
                    tmHan.tm_mday, tmHan.tm_mon + 1, tmHan.tm_year + 1900);

            item->infoText.setFont(font); 
            item->infoText.setString(dateBuffer); 
            item->infoText.setCharacterSize(16); 
            item->infoText.setFillColor(Theme::TextLight); 
            item->infoText.setPosition(305, itemY + 55);

            item->statusText.setFont(font); 
            item->statusText.setString(phieu->trangThaiHan()); 
            item->statusText.setCharacterSize(16);
            
            if (phieu->daQuaHan()) { 
                item->statusText.setFillColor(Theme::Error); 
                item->statusText.setStyle(sf::Text::Bold);
                item->box.setOutlineColor(Theme::Error); 
                item->box.setOutlineThickness(2);
            } else { 
                item->statusText.setFillColor(Theme::Success); 
                item->box.setOutlineColor(Theme::Border); 
            } 
            item->statusText.setPosition(305, itemY + 85);

            // --- LOGIC NÚT ĐÁNH GIÁ MỚI ---
            bool rated = false;
            if(libSystem) rated = libSystem->KiemTraDaDanhGia(currentReader->getMaID(), phieu->sach->getMaSach());
            
            // Màu nền nút: Vàng (đã đánh giá) hoặc Xám (chưa đánh giá)
            sf::Color btnColor = rated ? sf::Color(255, 193, 7) : sf::Color(200, 200, 200);
            
            // Tạo nút với TEXT RỖNG (để không bị lỗi font ô vuông)
            item->rateButton = new Button(sf::Vector2f(1150, itemY + 35), sf::Vector2f(60, 50), "", font, index, btnColor);
            item->rateButton->setCornerRadius(15.0f); // Bo tròn nút

            // Tạo hình ngôi sao trắng nằm đè lên nút
            // Vị trí nút là (1150, itemY+35). Kích thước 60x50. 
            // Tâm nút là (1150 + 30, itemY + 35 + 25) = (1180, itemY + 60)
            item->starIcon = createStarShape(1180, itemY + 60, 15.0f); // Bán kính sao 15
            item->starIcon.setFillColor(sf::Color::White); // Sao màu trắng

            bookItems.push_back(item); 
            itemY += ITEM_HEIGHT + 15.0f; 
            index++; 
            current = current->next;
        }
        
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_Y_START + 450.0f)); 
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) { 
        sidebar->update(mousePos); 
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > LIST_Y_START) { 
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
            for (auto item : bookItems) {
                item->rateButton->update(adjustedMousePos);
                // Hiệu ứng hover nhẹ cho ngôi sao
                if (item->rateButton->handleClick(adjustedMousePos)) { // Kiểm tra vùng hover
                     item->starIcon.setScale(1.2f, 1.2f);
                } else {
                     item->starIcon.setScale(1.0f, 1.0f);
                }
            }
        } 
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    
    std::string handleRateClick(sf::Vector2f mousePos) { 
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > LIST_Y_START) { 
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
            for (auto item : bookItems) 
                if (item->rateButton->handleClick(adjustedMousePos)) { 
                    int idx = item->rateButton->getId(); 
                    NodeMuonSach* temp = currentReader->getDanhSachPhieuMuon(); 
                    for(int i=0; i<idx && temp!=nullptr; ++i) temp = temp->next; 
                    if(temp) return temp->phieu->sach->getMaSach(); 
                } 
        } 
        return ""; 
    }
    
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } 
    Sidebar* getSidebar() { return sidebar; }
    
    void render(sf::RenderWindow& window) { 
        window.setView(window.getDefaultView()); 
        window.draw(background); 
        sidebar->draw(window); 
        window.draw(titleText); 
        
        if (bookItems.empty()) {
            window.draw(emptyText); 
        } else { 
            sf::View listView = window.getDefaultView(); 
            listView.setViewport(sf::FloatRect(250.f/1300.f, LIST_Y_START/720.f, 1050.f/1300.f, VIEW_HEIGHT/720.f)); 
            listView.setSize(1050, VIEW_HEIGHT); 
            listView.setCenter(250 + 1050/2, LIST_Y_START + VIEW_HEIGHT/2 + scrollView->getScrollOffset()); 
            
            window.setView(listView); 
            for (auto item : bookItems) { 
                window.draw(item->box); 
                window.draw(item->titleText); 
                window.draw(item->infoText); 
                window.draw(item->statusText); 
                
                // Vẽ nút nền trước
                item->rateButton->draw(window); 
                // Vẽ hình ngôi sao đè lên
                window.draw(item->starIcon);
            } 
            window.setView(window.getDefaultView()); 
        } 
    }
};
#endif