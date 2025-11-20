#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "InputField.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"

class HomeScreen {
private:
    sf::RectangleShape background;
    sf::Text titleTop10;
    sf::Text titleAllBooks;
    Sidebar* sidebar;
    InputField* searchBox;
    std::vector<Card*> top10Cards;
    std::vector<Card*> allBookCards;
    LibrarySystem* libSystem;
    ScrollView* scrollView;

public:
    HomeScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        // LIGHT THEME: Background sáng
        background.setFillColor(sf::Color(245, 245, 250));

        titleTop10.setFont(font);
        titleTop10.setString("Top 10 Sach Noi Bat");
        titleTop10.setCharacterSize(28);
        titleTop10.setFillColor(sf::Color(30, 30, 30)); // Text đen
        titleTop10.setPosition(280, 30);

        titleAllBooks.setFont(font);
        titleAllBooks.setString("Tat Ca Sach");
        titleAllBooks.setCharacterSize(28);
        titleAllBooks.setFillColor(sf::Color(30, 30, 30));
        titleAllBooks.setPosition(280, 700); // FIX: Vị trí Y ban đầu cho Title All Books

        sidebar = new Sidebar(font);
        searchBox = new InputField(sf::Vector2f(280, 80), sf::Vector2f(400, 45), 
                                   "Tim kiem sach...", font);

        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadBooks(font);
    }

    ~HomeScreen() {
        delete sidebar;
        delete searchBox;
        delete scrollView;
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
    }

    void loadBooks(sf::Font& font) {
        // Xóa cards cũ
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
        top10Cards.clear();
        allBookCards.clear();

        if (!libSystem) return;

        // LIGHT THEME: Màu sắc pastel sáng hơn
        std::vector<sf::Color> colors = {
            sf::Color(255, 182, 193), // Light pink
            sf::Color(173, 216, 230), // Light blue
            sf::Color(144, 238, 144), // Light green
            sf::Color(255, 218, 185), // Peach
            sf::Color(221, 160, 221), // Plum
            sf::Color(255, 228, 181), // Moccasin
            sf::Color(176, 224, 230), // Powder blue
            sf::Color(240, 230, 140), // Khaki
            sf::Color(255, 192, 203), // Pink
            sf::Color(175, 238, 238)  // Pale turquoise
        };

        // Top 10 sách (Tọa độ cố định)
        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 140;
        int count = 0;

        // Top 10 - 2 hàng
        while (current != nullptr && count < 10) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardY),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[count % 10],
                font
            );
            top10Cards.push_back(card);
            
            count++;
            if (count == 5) {
                cardX = 280;
                cardY = 420; // Hàng thứ 2
            } else {
                cardX += 200;
            }
            current = current->next;
        }

        // TẤT CẢ SÁCH - Bắt đầu từ 750
        current = libSystem->getDanhSachSach();
        cardX = 280;
        float cardYCurrent = 750; // Vị trí Y ban đầu cho Card All Books
        int col = 0;
        int totalBooks = 0;
        
        while (current != nullptr) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardYCurrent),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[totalBooks % 10],
                font
            );
            allBookCards.push_back(card);

            col++;
            totalBooks++;
            
            if (col == 5) {
                col = 0;
                cardX = 280;
                cardYCurrent += 270; // Next row
            } else {
                cardX += 200;
            }
            
            current = current->next;
        }

        // Set scroll để có thể cuộn xem hết
        // Vị trí Title All Books là Y=700. Khu vực Top 10 chiếm 680px.
        // maxScroll = Vị trí Y cuối cùng - Vị trí Y bắt đầu cuộn hiệu quả
        scrollView->setMaxScroll(std::max(0.0f, cardYCurrent - 680.0f));
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchBox->update();
        
        float scrollOffset = scrollView->getScrollOffset();
        
        // Top 10 (Không cuộn - giữ vị trí cũ)
        for (auto card : top10Cards) {
            card->update(mousePos);
        }
        
        // All Books (Áp dụng offset ngược lại cho vị trí chuột để kiểm tra hover)
        for (auto card : allBookCards) {
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            card->update(adjustedMousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleSearchEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchBox->handleEvent(event, mousePos);
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
        
        // Top 10 (Không cuộn)
        for (auto card : top10Cards) {
            if (card->isClicked(mousePos)) {
                return card->getBookId();
            }
        }
        // All Books (Áp dụng offset cho chuột)
        for (auto card : allBookCards) {
            if (card->isClicked(adjustedMousePos)) {
                return card->getBookId();
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
        
        float scrollOffset = scrollView->getScrollOffset();
        
        // Title Top 10 (không scroll)
        window.draw(titleTop10);
        searchBox->draw(window);
        
        // Cần thiết lập View để chỉ cuộn phần danh sách sách
        sf::View originalView = window.getView();
        
        // Vẽ Top 10 (Cố định)
        for (auto card : top10Cards) {
            card->draw(window);
        }
        
        // Áp dụng cuộn cho View
        sf::View scrolledView = originalView;
        scrolledView.move(0, -scrollOffset);
        window.setView(scrolledView);
        
        // Title "Tất cả sách" với scroll
        sf::Text tempTitle = titleAllBooks;
        // Tọa độ 700 sẽ bị dịch chuyển lên/xuống theo scrollOffset
        window.draw(tempTitle);
        
        // Vẽ all book cards với scroll
        for (auto card : allBookCards) {
            card->draw(window);
        }
        
        // KHÔI PHỤC View
        window.setView(originalView);
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif