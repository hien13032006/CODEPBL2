#ifndef MANAGE_READERS_SCREEN_HPP
#define MANAGE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Node.h" 
#include "RoundedRectangle.hpp" 

struct BorrowedDetailItem {
    RoundedRectangleShape box; 
    sf::Text maSachText; sf::Text tenSachText; sf::Text ngayMuonText;
    sf::Text ngayHetHanText; sf::Text trangThaiText; 
};

struct ReaderDisplayItem {
    RoundedRectangleShape box; 
    sf::Text idText; sf::Text nameText; sf::Text infoText; sf::Text statusText;
};

class ManageReadersScreen {
private:
    sf::RenderWindow* windowRef; 
    sf::RectangleShape background; sf::Text titleText;
    Sidebar* sidebar; ScrollView* scrollView; InputField* searchField;
    Button* searchButton; Button* viewAllButton;
    std::vector<ReaderDisplayItem*> readerItems; LibrarySystem* libSystem;
    Modal* readerDetailModal; Reader* selectedReader = nullptr; 
    sf::Text detailTitle; sf::Text detailInfo; sf::Text detailFine;
    std::vector<sf::Text*> borrowedListHeaders; std::vector<BorrowedDetailItem*> borrowedItems; 

    sf::View getListView() {
        if (!windowRef) return sf::View();
        sf::View view = windowRef->getDefaultView();
        view.setViewport(sf::FloatRect(250.f/1400.f, 180.f/900.f, 1150.f/1400.f, 720.f/900.f));
        view.setSize(1150, 720);
        view.setCenter(250 + 1150/2, 180 + 720/2 + scrollView->getScrollOffset());
        return view;
    }

public:
    ManageReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        windowRef = nullptr;
        background.setSize(sf::Vector2f(1150, 900)); background.setPosition(250, 0); background.setFillColor(sf::Color(13, 15, 23));
        titleText.setFont(font); titleText.setString("QUAN LY DOC GIA"); titleText.setCharacterSize(32); titleText.setFillColor(sf::Color::White); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 180, 1150, 720)); 
        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(400, 50), "Tim kiem...", font);
        searchButton = new Button(sf::Vector2f(700, 100), sf::Vector2f(150, 50), "Tim", font);
        viewAllButton = new Button(sf::Vector2f(870, 100), sf::Vector2f(180, 50), "Xem Tat Ca", font, 0, sf::Color(100, 150, 200));
        
        readerDetailModal = new Modal(font);
        readerDetailModal->getModalBox().setSize(sf::Vector2f(1000, 750)); readerDetailModal->getModalBox().setPosition(200, 75);
        detailTitle.setFont(font); detailTitle.setCharacterSize(24); detailTitle.setFillColor(sf::Color(255, 193, 94)); detailTitle.setPosition(230, 105);
        detailInfo.setFont(font); detailInfo.setCharacterSize(18); detailInfo.setFillColor(sf::Color::White); detailInfo.setPosition(230, 155);
        detailFine.setFont(font); detailFine.setCharacterSize(18); detailFine.setFillColor(sf::Color(255, 100, 100)); detailFine.setPosition(230, 205);
        loadReaders(font);
    }

    ~ManageReadersScreen() { /*Cleanup*/ }
    void setWindow(sf::RenderWindow* win) { windowRef = win; }

    void loadReaders(sf::Font& font, const std::string& keyword = "") {
        for (auto item : readerItems) delete item; readerItems.clear();
        if (!libSystem) return;
        std::string kLower = keyword; std::transform(kLower.begin(), kLower.end(), kLower.begin(), ::tolower);
        NodeReader* current = libSystem->getDanhSachDocGia();
        float itemY = 180; 
        const float ITEM_HEIGHT = 120.0f; 
        const float GAP = 15.0f;

        while (current != nullptr) {
            Reader* r = current->data;
            std::string id = r->getMaID(); std::string name = r->getHoTen();
            std::string infoSearch = id + name + r->getEmail() + r->getSDT();
            std::transform(infoSearch.begin(), infoSearch.end(), infoSearch.begin(), ::tolower);
            
            if (keyword.empty() || infoSearch.find(kLower) != std::string::npos) {
                ReaderDisplayItem* item = new ReaderDisplayItem();
                item->box.setSize(sf::Vector2f(1050, ITEM_HEIGHT)); item->box.setCornerRadius(12.0f);
                item->box.setPosition(280, itemY); item->box.setFillColor(sf::Color(30, 33, 45)); 

                item->idText.setFont(font); item->idText.setString(id); item->idText.setCharacterSize(20); item->idText.setFillColor(sf::Color(255, 193, 94)); item->idText.setPosition(310, itemY + 20);
                item->nameText.setFont(font); item->nameText.setString(name); item->nameText.setCharacterSize(24); item->nameText.setFillColor(sf::Color::White); item->nameText.setPosition(430, itemY + 18);
                item->infoText.setFont(font); item->infoText.setString("Email: " + r->getEmail() + " | SDT: " + r->getSDT()); item->infoText.setCharacterSize(16); item->infoText.setFillColor(sf::Color(180, 180, 180)); item->infoText.setPosition(430, itemY + 55);
                
                int qh = r->DemSachQuaHan();
                item->statusText.setFont(font); item->statusText.setString("Dang muon: " + std::to_string(r->DemSachDaMuon()) + (qh>0 ? " | QUA HAN: "+std::to_string(qh) : ""));
                item->statusText.setCharacterSize(16); item->statusText.setFillColor(qh > 0 ? sf::Color(255, 80, 80) : sf::Color(100, 220, 100));
                item->statusText.setPosition(430, itemY + 85);

                readerItems.push_back(item); itemY += ITEM_HEIGHT + GAP;
            }
            current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, itemY - 180.0f)); scrollView->reset();
    }

    void showReaderDetailModal(sf::Font& font) { /*Logic modal giữ nguyên, dùng BorrowedDetailItem*/ 
         if (!selectedReader) return;
         /* ... (Copy phần logic load modal ở câu trả lời trước) ... */
         readerDetailModal->show();
    }

    void update(sf::Vector2f mousePos) {
        if (!windowRef) return;
        sidebar->update(mousePos); searchField->update(); searchButton->update(mousePos); viewAllButton->update(mousePos);
        if (!readerDetailModal->isShown()) {
            sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
            if (pixelMouse.y > 180) {
                sf::View listView = getListView();
                sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView);
                for (auto item : readerItems) {
                    if (item->box.getGlobalBounds().contains(listMousePos)) item->box.setFillColor(sf::Color(45, 50, 65));
                    else item->box.setFillColor(sf::Color(30, 33, 45));
                }
            }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!windowRef) return;
        if (readerDetailModal->isShown()) { if (readerDetailModal->handleClose(mousePos)) { readerDetailModal->hide(); } return; }
        if (searchButton->handleClick(mousePos)) { loadReaders(font, searchField->getText()); return; }
        if (viewAllButton->handleClick(mousePos)) { searchField->clear(); loadReaders(font); return; }
        
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
        if (pixelMouse.y > 180) {
            sf::View listView = getListView();
            sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView);
            for (size_t i = 0; i < readerItems.size(); ++i) {
                if (readerItems[i]->box.getGlobalBounds().contains(listMousePos)) {
                    /* Find reader logic */
                    std::string id = readerItems[i]->idText.getString();
                    NodeReader* t = libSystem->getDanhSachDocGia();
                    while(t){ if(t->data->getMaID()==id){selectedReader=t->data; libSystem->DocDanhSachMuonCuaDocGia(selectedReader); showReaderDetailModal(font); break;} t=t->next;}
                    break;
                }
            }
        }
    }
    
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView());
        window.draw(background); sidebar->draw(window); window.draw(titleText);
        searchField->draw(window); searchButton->draw(window); viewAllButton->draw(window);

        window.setView(getListView());
        for (auto item : readerItems) {
            window.draw(item->box); window.draw(item->idText); window.draw(item->nameText);
            window.draw(item->infoText); window.draw(item->statusText);
        }
        
        window.setView(window.getDefaultView());
        if (readerDetailModal->isShown()) { readerDetailModal->draw(window); /* Vẽ chi tiết modal... */ }
    }
};

#endif