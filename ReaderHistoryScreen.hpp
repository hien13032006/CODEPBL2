#ifndef READER_HISTORY_SCREEN_HPP
#define READER_HISTORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"
#include "Theme.hpp"
#include "LibrarySystem.h"

struct HistoryItem { 
    RoundedRectangleShape box; 
    sf::Text hanhDong, maSach, tenSach, thoiGian; 
    Button* rateButton;       // Nút đánh giá
    sf::ConvexShape starIcon; // Icon ngôi sao
    bool canRate;             // Cờ đánh dấu dòng này có được đánh giá không
    std::string bookID;       // Lưu mã sách để xử lý
};

class ReaderHistoryScreen {
private:
    sf::Font &font; 
    Reader* reader; 
    LibrarySystem* libSystem; // Cần để check đã đánh giá chưa
    
    sf::RectangleShape bgOverlay, panel; 
    sf::Text header, colHanhDong, colMaSach, colTenSach, colThoiGian, colDanhGia;
    
    std::vector<HistoryItem*> items; 
    Button btnBack; 
    bool closed; 
    ScrollView* scrollView;
    
    const float LIST_START_Y = 200.0f; 
    const float VIEW_HEIGHT = 400.0f;

    // Hàm vẽ ngôi sao (tái sử dụng)
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
        star.setPosition(x, y);
        return star;
    }

public:
    // Thêm tham số LibrarySystem* vào constructor
    ReaderHistoryScreen(sf::Font &f, Reader* r, LibrarySystem* lib) 
        : font(f), reader(r), libSystem(lib), 
          btnBack({1050, 110}, {50, 50}, "X", f, 0, Theme::Error), closed(false) {
        
        bgOverlay.setSize({1300, 720}); 
        bgOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        
        panel.setSize({1000, 600}); 
        panel.setFillColor(sf::Color::White); 
        panel.setOutlineThickness(2); 
        panel.setOutlineColor(Theme::Primary); 
        panel.setPosition(150, 60);
        
        header.setFont(font); header.setCharacterSize(32); 
        header.setString("LICH SU MUON TRA"); 
        header.setFillColor(Theme::TextDark); 
        header.setPosition(450, 110);
        
        auto setupCol = [&](sf::Text& txt, std::string s, float x) { 
            txt.setFont(font); txt.setString(s); txt.setCharacterSize(18); 
            txt.setFillColor(Theme::Secondary); txt.setPosition(x, 160); 
        };
        setupCol(colHanhDong, "HANH DONG", 200); 
        setupCol(colMaSach, "MA SACH", 340); 
        setupCol(colTenSach, "TEN SACH", 480); 
        setupCol(colThoiGian, "THOI GIAN", 750);
        setupCol(colDanhGia, "DANH GIA", 950); // Cột mới

        scrollView = new ScrollView(sf::FloatRect(150, LIST_START_Y, 1000, VIEW_HEIGHT));
        loadFromFile();
    }

    ~ReaderHistoryScreen() { 
        delete scrollView; 
        for(auto item : items) {
            if(item->canRate) delete item->rateButton;
            delete item;
        }
    }

    void reset() { closed = false; loadFromFile(); scrollView->reset(); }

    void setReader(Reader* r) {
        this->reader = r;
        loadFromFile();
    }

    void loadFromFile() {
        // Dọn dẹp cũ
        for(auto item : items) {
            if(item->canRate) delete item->rateButton;
            delete item;
        }
        items.clear(); 

        if (!reader) return; 
        std::ifstream in(reader->getFileLichSu()); 
        if (!in.is_open()) return;

        std::string line; 
        float itemY = LIST_START_Y; 
        const float ITEM_HEIGHT = 60.0f;
        int index = 0;

        while (std::getline(in, line)) {
            if (line.empty()) continue; 
            std::stringstream ss(line); 
            std::string hanhDong, maDocGia, maSach, tenSach, thoiGian;
            
            getline(ss, hanhDong, '|'); 
            getline(ss, maDocGia, '|'); 
            getline(ss, maSach, '|'); 
            getline(ss, tenSach, '|'); 
            getline(ss, thoiGian, '|');

            HistoryItem* item = new HistoryItem();
            item->bookID = maSach;
            item->box.setSize({950, ITEM_HEIGHT - 10}); 
            item->box.setCornerRadius(8.0f); 
            item->box.setPosition(175, itemY); 
            item->box.setFillColor(Theme::Background);

            auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = Theme::TextDark) { 
                txt.setFont(font); txt.setString(s); txt.setCharacterSize(16); 
                txt.setFillColor(c); txt.setPosition(x, itemY + (ITEM_HEIGHT-10)/2 - 10); 
            };

            // Tô màu hành động: Muon (Xanh/Đen), Tra (Hồng/Đỏ)
            sf::Color actColor = (hanhDong == "Muon") ? Theme::TextDark : Theme::Primary;
            setupText(item->hanhDong, hanhDong, 210, actColor); 
            setupText(item->maSach, maSach, 340);
            
            if (tenSach.length() > 25) tenSach = tenSach.substr(0, 22) + "..."; 
            setupText(item->tenSach, tenSach, 480); 
            setupText(item->thoiGian, thoiGian, 750, Theme::TextLight);

            // [LOGIC ĐÁNH GIÁ]
            // Chỉ cho phép đánh giá nếu hành động là "Tra" (Đã đọc xong)
            if (hanhDong == "Tra") {
                item->canRate = true;
                
                bool rated = false;
                if (libSystem) rated = libSystem->KiemTraDaDanhGia(maDocGia, maSach);
                
                sf::Color starColor = rated ? sf::Color(255, 193, 7) : sf::Color(200, 200, 200);

                // Nút ẩn
                item->rateButton = new Button({970, itemY + 5}, {40, 40}, "", font, index, starColor);
                item->rateButton->setCornerRadius(20.0f);

                // Icon ngôi sao
                item->starIcon = createStarShape(990, itemY + 25, 12.0f);
                item->starIcon.setFillColor(sf::Color::White);
            } else {
                item->canRate = false;
                item->rateButton = nullptr;
            }

            items.push_back(item); 
            itemY += ITEM_HEIGHT;
            index++;
        }
        in.close();
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_START_Y + 600.0f));
    }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { 
        if (e.type == sf::Event::MouseButtonPressed && btnBack.handleClick(mousePos)) closed = true; 
        scrollView->handleScroll(e, mousePos); 
    }

    // [MỚI] Xử lý click vào ngôi sao trong lịch sử
    std::string handleClickRate(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > LIST_START_Y) { 
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
            for (auto item : items) {
                if (item->canRate && item->rateButton->handleClick(adjustedMousePos)) {
                    return item->bookID;
                }
            }
        }
        return "";
    }

    void update(sf::Vector2f mousePos) { 
        btnBack.update(mousePos); 
        
        float scrollOffset = scrollView->getScrollOffset(); 
        if (mousePos.y > LIST_START_Y) { 
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
            for (auto item : items) {
                if (item->canRate) {
                    item->rateButton->update(adjustedMousePos);
                    // Hiệu ứng hover
                    if (item->rateButton->handleClick(adjustedMousePos)) item->starIcon.setScale(1.2f, 1.2f);
                    else item->starIcon.setScale(1.0f, 1.0f);
                }
            }
        }
    }

    void render(sf::RenderWindow &window) {
        window.setView(window.getDefaultView()); 
        window.draw(bgOverlay); 
        window.draw(panel); 
        window.draw(header); 
        window.draw(colHanhDong); window.draw(colMaSach); window.draw(colTenSach); window.draw(colThoiGian); window.draw(colDanhGia);
        btnBack.draw(window);
        
        sf::View listView = window.getDefaultView(); 
        listView.setViewport(sf::FloatRect(150.f/1300.f, LIST_START_Y/720.f, 1000.f/1300.f, VIEW_HEIGHT/720.f)); 
        listView.setSize(1000, VIEW_HEIGHT); 
        listView.setCenter(150 + 1000/2, LIST_START_Y + VIEW_HEIGHT/2 + scrollView->getScrollOffset()); 
        window.setView(listView);
        
        for (const auto &item : items) { 
            window.draw(item->box); 
            window.draw(item->hanhDong); 
            window.draw(item->maSach); 
            window.draw(item->tenSach); 
            window.draw(item->thoiGian); 
            
            if (item->canRate) {
                item->rateButton->draw(window);
                window.draw(item->starIcon);
            }
        } 
        window.setView(window.getDefaultView());
    }
    bool wasClosed() const { return closed; }
};
#endif