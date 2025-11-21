#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "InputField.hpp"
#include "LibrarySystem.h"
#include "ScrollView.hpp"

class HomeScreen {
private:
    sf::RectangleShape background;
    sf::Text titleTop10;
    sf::Text titleAll;
    
    Sidebar* sidebar;
    InputField* searchBox;
    Button* searchBtnIcon; 
    
    std::vector<Card*> top10Cards;
    std::vector<Card*> allBookCards;
    
    // Scroll ngang
    ScrollView* scrollTop10;
    ScrollView* scrollAll;
    
    LibrarySystem* libSystem;
    bool isSearchTriggered;

public:
    HomeScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        // 1. Background
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(245, 245, 250));

        sidebar = new Sidebar(font);
        
        // 2. Search Bar (Đẩy lên cao chút: Y=25)
        searchBox = new InputField(sf::Vector2f(280, 25), sf::Vector2f(400, 45), "Tim sach, tac gia...", font);
        searchBtnIcon = new Button(sf::Vector2f(690, 25), sf::Vector2f(100, 45), "Tim", font, 0, sf::Color(60, 100, 180));

        // 3. Title Top 10 (Y=90)
        titleTop10.setFont(font);
        titleTop10.setString("Top 10 Sach Hay Nhat");
        titleTop10.setCharacterSize(22);
        // titleTop10.setStyle(sf::Text::Bold); // Bỏ in đậm theo yêu cầu trước
        titleTop10.setFillColor(sf::Color(30, 30, 30));
        titleTop10.setPosition(280, 80);

        // 4. Title All Books (Y=500) - Đẩy xuống để tránh bị hàng trên đè
        titleAll.setFont(font);
        titleAll.setString("Tat Ca Sach");
        titleAll.setCharacterSize(22);
        // titleAll.setStyle(sf::Text::Bold); 
        titleAll.setFillColor(sf::Color(30, 30, 30));
        titleAll.setPosition(280, 400); 

        // 5. Scroll View (Chứa Card to 340px)
        // Vùng Top 10: Y=130, Cao=350 (Kết thúc ở 480 -> Cách Title dưới 20px)
        scrollTop10 = new ScrollView(sf::FloatRect(250, 130, 1150, 350), ScrollDirection::HORIZONTAL);
        
        // Vùng All Books: Y=540, Cao=350
        scrollAll = new ScrollView(sf::FloatRect(250, 540, 1150, 350), ScrollDirection::HORIZONTAL);

        isSearchTriggered = false;
        loadBooks(font);
    }

    ~HomeScreen() {
        delete sidebar; delete searchBox; delete searchBtnIcon;
        delete scrollTop10; delete scrollAll;
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
    }

    void loadBooks(sf::Font& font) {
        for (auto card : top10Cards) delete card; top10Cards.clear();
        for (auto card : allBookCards) delete card; allBookCards.clear();

        if (!libSystem) return;

        libSystem->XepHangSach();
        const auto& topIDs = libSystem->getTop10IDs();
        std::vector<sf::Color> colors = { 
            sf::Color(200, 80, 60), sf::Color(60, 140, 200), 
            sf::Color(100, 180, 100), sf::Color(200, 150, 60), 
            sf::Color(140, 80, 180) 
        };
        
        float startX = 10; 
        float cardW = 220; // Card To
        float cardH = 340; 
        float gapX = 240;  

        // Load Top 10
        int idx = 0;
        for (const auto& id : topIDs) {
            NodeBook* node = libSystem->getDanhSachSach();
            while(node) {
                if (node->data->getMaSach() == id) {
                    Card* card = new Card(
                        sf::Vector2f(startX + idx * gapX, 5), 
                        sf::Vector2f(cardW, cardH), 
                        node->data->getMaSach(), node->data->getTenSach(),
                        node->data->getTacGia(), node->data->getDiemTrungBinh(),
                        colors[idx % 5], font, true
                    );
                    top10Cards.push_back(card);
                    idx++; break;
                }
                node = node->next;
            }
        }
        scrollTop10->setMaxScroll(idx * gapX + 50); 

        // Load All Books
        idx = 0;
        NodeBook* curr = libSystem->getDanhSachSach();
        while (curr) {
            bool isHot = (curr->data->getDiemTrungBinh() >= 8.0);
            Card* card = new Card(
                sf::Vector2f(startX + idx * gapX, 5), 
                sf::Vector2f(cardW, cardH),
                curr->data->getMaSach(), curr->data->getTenSach(),
                curr->data->getTacGia(), curr->data->getDiemTrungBinh(),
                colors[idx % 5], font, isHot
            );
            allBookCards.push_back(card);
            idx++; curr = curr->next;
        }
        scrollAll->setMaxScroll(idx * gapX + 50);
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos); 
        searchBox->update(); 
        searchBtnIcon->update(mousePos);

        float offsetTop = scrollTop10->getScrollOffset();
        float offsetAll = scrollAll->getScrollOffset();
        
        // Check hover Top 10
        if (mousePos.y >= 130 && mousePos.y <= 480 && mousePos.x >= 250) {
            sf::Vector2f rel(mousePos.x - 250 + offsetTop, mousePos.y - 130);
            for (auto card : top10Cards) card->update(rel, 0, 0);
        } else {
            for (auto card : top10Cards) card->update(sf::Vector2f(-1000,-1000));
        }

        // Check hover All Books
        if (mousePos.y >= 540 && mousePos.y <= 890 && mousePos.x >= 250) {
             sf::Vector2f rel(mousePos.x - 250 + offsetAll, mousePos.y - 540);
             for (auto card : allBookCards) card->update(rel, 0, 0);
        } else {
             for (auto card : allBookCards) card->update(sf::Vector2f(-1000,-1000));
        }
    }

    void handleSearchEvent(sf::Event& event, sf::Vector2f mousePos) { 
        searchBox->handleEvent(event, mousePos);
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            if (searchBox->hasFocus() && !searchBox->getText().empty()) isSearchTriggered = true;
        }
        scrollTop10->handleScroll(event, mousePos);
        scrollAll->handleScroll(event, mousePos);
    }

    std::string getSearchQuery() { if (isSearchTriggered) { isSearchTriggered = false; return searchBox->getText(); } return ""; }
    bool isSearchRequested() { return isSearchTriggered; }
    bool checkSearchClick(sf::Vector2f mousePos) { if (searchBtnIcon->handleClick(mousePos) && !searchBox->getText().empty()) { isSearchTriggered = true; return true; } return false; }
    
    std::string handleCardClick(sf::Vector2f mousePos) {
        float offsetTop = scrollTop10->getScrollOffset();
        if (mousePos.y >= 130 && mousePos.y <= 480 && mousePos.x >= 250) {
            sf::Vector2f rel(mousePos.x - 250 + offsetTop, mousePos.y - 130);
            for (auto card : top10Cards) if (card->isClicked(rel)) return card->getBookId();
        }
        float offsetAll = scrollAll->getScrollOffset();
        if (mousePos.y >= 540 && mousePos.y <= 890 && mousePos.x >= 250) {
             sf::Vector2f rel(mousePos.x - 250 + offsetAll, mousePos.y - 540);
             for (auto card : allBookCards) if (card->isClicked(rel)) return card->getBookId();
        }
        return "";
    }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        // BƯỚC 1: Vẽ Background (Lớp dưới cùng)
        window.setView(window.getDefaultView());
        window.draw(background);

        // BƯỚC 2: Vẽ nội dung cuộn (Lớp giữa)
        // Top 10
        sf::View viewTop = window.getDefaultView();
        viewTop.setViewport(sf::FloatRect(250.f/1400.f, 130.f/900.f, 1150.f/1400.f, 350.f/900.f));
        viewTop.setSize(1150, 350);
        viewTop.setCenter(1150.f/2.f + scrollTop10->getScrollOffset(), 350.f/2.f);
        window.setView(viewTop);
        for (auto card : top10Cards) card->draw(window);

        // All Books
        sf::View viewAll = window.getDefaultView();
        viewAll.setViewport(sf::FloatRect(250.f/1400.f, 540.f/900.f, 1150.f/1400.f, 350.f/900.f));
        viewAll.setSize(1150, 350);
        viewAll.setCenter(1150.f/2.f + scrollAll->getScrollOffset(), 350.f/2.f);
        window.setView(viewAll);
        for (auto card : allBookCards) card->draw(window);

        // BƯỚC 3: Vẽ Sidebar và UI Tĩnh (Lớp trên cùng - Luôn nổi)
        // Reset về view mặc định để vẽ đè lên
        window.setView(window.getDefaultView());
        
        // Sidebar vẽ sau cùng -> Sẽ đè lên các sách nếu sách bị trôi sang trái
        sidebar->draw(window); 
        
        // Tiêu đề và thanh tìm kiếm vẽ sau cùng -> Sẽ đè lên sách nếu sách trôi lên
        window.draw(titleTop10);
        window.draw(titleAll);
        searchBox->draw(window);
        searchBtnIcon->draw(window);
    }
};

#endif