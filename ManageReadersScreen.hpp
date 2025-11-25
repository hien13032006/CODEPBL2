#ifndef MANAGE_READERS_SCREEN_HPP
#define MANAGE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Node.h" 
#include "RoundedRectangle.hpp" 
#include "Theme.hpp"

struct ReaderDisplayItem { RoundedRectangleShape box; sf::Text idText, nameText, infoText, statusText; };
struct DetailBookRow { sf::Text stt, tenSach, ngayMuon, hanTra, trangThai, soNgayQH, tienPhat; };

class ManageReadersScreen {
private:
    sf::RenderWindow* windowRef; RoundedRectangleShape background; sf::Text titleText;
    Sidebar* sidebar; ScrollView* scrollView; InputField* searchField;
    Button *searchButton, *viewAllButton, *addReaderButton;
    std::vector<ReaderDisplayItem*> readerItems; LibrarySystem* libSystem;
    
    Modal* readerDetailModal; Reader* selectedReader = nullptr; RoundedRectangleShape detailPanel;
    sf::Text dtTitle, dtInfo, headerSTT, headerTen, headerMuon, headerTra, headerTrangThai, headerNgayQH, headerPhat;
    Button* dtCloseBtn; std::vector<DetailBookRow> detailBookRows;

    Modal* addReaderModal; RoundedRectangleShape addPanel; sf::Text addTitle, addMsg;
    InputField *inName, *inPhone, *inEmail, *inUser, *inPass; Button *btnAddSave, *btnAddCancel;

    sf::View getListView() {
        if (!windowRef) return sf::View(); sf::View view = windowRef->getDefaultView();
        view.setViewport(sf::FloatRect(250.f/1300.f, 180.f/720.f, 1050.f/1300.f, 540.f/720.f)); view.setSize(1050, 540); view.setCenter(250 + 1050/2, 180 + 540/2 + scrollView->getScrollOffset()); return view;
    }
    std::string dateToString(time_t t) { tm* ltm = localtime(&t); char buffer[20]; sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900); return std::string(buffer); }
    std::string formatMoney(long money) { std::string s = std::to_string(money); int n = s.length() - 3; while (n > 0) { s.insert(n, ","); n -= 3; } return s + "d"; }

public:
    ManageReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        windowRef = nullptr;
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("QUAN LY DOC GIA"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font); scrollView = new ScrollView(sf::FloatRect(250, 180, 1050, 540)); 
        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(400, 50), "Tim kiem...", font);
        searchButton = new Button(sf::Vector2f(700, 100), sf::Vector2f(120, 50), "Tim", font, 0, Theme::Primary);
        viewAllButton = new Button(sf::Vector2f(840, 100), sf::Vector2f(150, 50), "Xem Tat Ca", font, 0, Theme::Secondary);
        addReaderButton = new Button(sf::Vector2f(1010, 100), sf::Vector2f(50, 50), "+", font, 0, Theme::Success); addReaderButton->getText().setCharacterSize(36);

        readerDetailModal = new Modal(font); detailPanel.setSize({1150, 600}); detailPanel.setCornerRadius(15.0f); detailPanel.setFillColor(sf::Color::White); detailPanel.setOutlineThickness(2); detailPanel.setOutlineColor(Theme::Primary); detailPanel.setPosition(75, 60); 
        dtTitle.setFont(font); dtTitle.setCharacterSize(24); dtTitle.setFillColor(Theme::Primary); dtTitle.setPosition(110, 90);
        dtInfo.setFont(font); dtInfo.setCharacterSize(18); dtInfo.setFillColor(Theme::TextDark); dtInfo.setPosition(110, 140); dtInfo.setLineSpacing(1.4f);
        dtCloseBtn = new Button({550, 600}, {200, 50}, "Dong", font, 0, sf::Color(150, 150, 150));
        auto setupHeader = [&](sf::Text& txt, std::string s, float x, float y) { txt.setFont(font); txt.setString(s); txt.setCharacterSize(15); txt.setFillColor(sf::Color(120, 120, 120)); txt.setStyle(sf::Text::Bold); txt.setPosition(x, y); };
        float tY = 300; setupHeader(headerSTT, "STT", 110, tY); setupHeader(headerTen, "TEN SACH", 160, tY); setupHeader(headerMuon, "NGAY MUON", 430, tY); setupHeader(headerTra, "HAN TRA", 560, tY); setupHeader(headerTrangThai, "TRANG THAI", 690, tY); setupHeader(headerNgayQH, "QUA HAN", 840, tY); setupHeader(headerPhat, "TIEN PHAT", 960, tY);

        addReaderModal = new Modal(font); addPanel.setSize({600, 550}); addPanel.setCornerRadius(15.0f); addPanel.setFillColor(sf::Color::White); addPanel.setOutlineThickness(2); addPanel.setOutlineColor(Theme::Primary); addPanel.setPosition(350, 85);
        addTitle.setFont(font); addTitle.setString("THEM DOC GIA MOI"); addTitle.setCharacterSize(26); addTitle.setFillColor(Theme::Primary); addTitle.setPosition(530, 110);
        float formY = 160; float gap = 70;
        inName = new InputField({450, formY}, {400, 50}, "Ho va Ten", font); inPhone = new InputField({450, formY+gap}, {400, 50}, "So Dien Thoai", font);
        inEmail = new InputField({450, formY+gap*2}, {400, 50}, "Email", font); inUser = new InputField({450, formY+gap*3}, {400, 50}, "Username", font); inPass = new InputField({450, formY+gap*4}, {400, 50}, "Password", font, true);
        addMsg.setFont(font); addMsg.setCharacterSize(16); addMsg.setPosition(450, formY + gap*5 + 5);
        btnAddSave = new Button({450, 550}, {190, 50}, "Dang Ky", font, 0, Theme::Success); btnAddCancel = new Button({660, 550}, {190, 50}, "Huy", font, 0, sf::Color(150, 150, 150));
        loadReaders(font);
    }
    ~ManageReadersScreen() { /* Delete all */ }
    void setWindow(sf::RenderWindow* win) { windowRef = win; }

    void showAddModal() { inName->clear(); inPhone->clear(); inEmail->clear(); inUser->clear(); inPass->clear(); addMsg.setString(""); addReaderModal->show(); }
    void handleAddReaderLogic(sf::Font& font) {
        if (inName->getText().empty() || inUser->getText().empty() || inPass->getText().empty()) { addMsg.setString("Vui long nhap du thong tin!"); addMsg.setFillColor(Theme::Error); return; }
        if (libSystem->KiemTraDocGiaDaDangKy(inUser->getText())) { addMsg.setString("Username da ton tai!"); addMsg.setFillColor(Theme::Error); return; }
        Reader* newR = new Reader(); newR->SignUp(inName->getText(), inPhone->getText(), inEmail->getText(), inUser->getText(), inPass->getText());
        std::ofstream out("DocGia.txt", std::ios::app); if (out.is_open()) { out << newR->toCSV() << "\n"; out.close(); }
        libSystem->DocFileDocGia(); loadReaders(font); addReaderModal->hide();
    }

    void prepareModalData(Reader* r, sf::Font& font) {
        dtTitle.setString("THONG TIN: " + r->getHoTen());
        std::string s = "Ma ID: " + r->getMaID() + "\nUsername: " + r->getUsername() + "   |   Email: " + r->getEmail() + "\nSo dien thoai: " + r->getSDT() + "\nTong sach dang muon: " + std::to_string(r->DemSachDaMuon()) + " cuon\nSach qua han: " + std::to_string(r->DemSachQuaHan()) + " cuon\n"; dtInfo.setString(s);
        detailBookRows.clear(); NodeMuonSach* curr = r->getDanhSachPhieuMuon(); int stt = 1; float rowY = 340; const long FINE_PER_DAY = 5000;
        while(curr != nullptr) {
            PhieuMuonSach* p = curr->phieu; DetailBookRow row;
            auto setRowTxt = [&](sf::Text& txt, std::string str, float x, sf::Color c = Theme::TextDark) { txt.setFont(font); txt.setString(str); txt.setCharacterSize(16); txt.setFillColor(c); txt.setPosition(x, rowY); };
            setRowTxt(row.stt, std::to_string(stt), 110); std::string ten = p->sach->getTenSach(); if(ten.size() > 25) ten = ten.substr(0, 22) + "..."; setRowTxt(row.tenSach, ten, 160, Theme::Primary); setRowTxt(row.ngayMuon, dateToString(p->ngayMuon), 430); setRowTxt(row.hanTra, dateToString(p->ngayHetHan), 560);
            if (p->daQuaHan()) { setRowTxt(row.trangThai, p->trangThaiHan(), 690, Theme::Error); int days = std::abs(p->soNgayConLai()); setRowTxt(row.soNgayQH, std::to_string(days) + " ngay", 840, Theme::Error); setRowTxt(row.tienPhat, formatMoney(days * FINE_PER_DAY), 960, Theme::Error); } 
            else { setRowTxt(row.trangThai, p->trangThaiHan(), 690, Theme::Success); setRowTxt(row.soNgayQH, "-", 860, Theme::TextLight); setRowTxt(row.tienPhat, "-", 980, Theme::TextLight); }
            detailBookRows.push_back(row); rowY += 35; stt++; curr = curr->next;
        }
    }

    void loadReaders(sf::Font& font, const std::string& keyword = "") {
        for (auto item : readerItems) delete item; readerItems.clear(); if (!libSystem) return;
        std::string kLower = keyword; std::transform(kLower.begin(), kLower.end(), kLower.begin(), ::tolower);
        NodeReader* current = libSystem->getDanhSachDocGia(); float itemY = 180; const float ITEM_HEIGHT = 120.0f; 
        while (current != nullptr) {
            Reader* r = current->data; std::string infoSearch = r->getMaID() + r->getHoTen() + r->getEmail(); std::transform(infoSearch.begin(), infoSearch.end(), infoSearch.begin(), ::tolower);
            if (keyword.empty() || infoSearch.find(kLower) != std::string::npos) {
                ReaderDisplayItem* item = new ReaderDisplayItem(); item->box.setSize(sf::Vector2f(1000, ITEM_HEIGHT)); item->box.setCornerRadius(12.0f); item->box.setPosition(275, itemY); item->box.setFillColor(sf::Color::White); item->box.setOutlineThickness(1); item->box.setOutlineColor(Theme::Border);
                item->idText.setFont(font); item->idText.setString(r->getMaID()); item->idText.setCharacterSize(20); item->idText.setFillColor(Theme::Primary); item->idText.setPosition(305, itemY + 20);
                item->nameText.setFont(font); item->nameText.setString(r->getHoTen()); item->nameText.setCharacterSize(24); item->nameText.setFillColor(Theme::TextDark); item->nameText.setPosition(400, itemY + 18);
                std::string info = "Email: " + r->getEmail() + " | SDT: " + r->getSDT(); item->infoText.setFont(font); item->infoText.setString(info); item->infoText.setCharacterSize(16); item->infoText.setFillColor(Theme::TextLight); item->infoText.setPosition(400, itemY + 55);
                int qh = r->DemSachQuaHan(); item->statusText.setFont(font); item->statusText.setString("Dang muon: " + std::to_string(r->DemSachDaMuon()) + (qh>0 ? " | QUA HAN: "+std::to_string(qh) : "")); item->statusText.setCharacterSize(16); item->statusText.setFillColor(qh > 0 ? Theme::Error : Theme::Success); item->statusText.setPosition(400, itemY + 85);
                readerItems.push_back(item); itemY += ITEM_HEIGHT + 15.0f;
            } current = current->next;
        }
        // FIX: Padding 600.0f
        scrollView->setMaxScroll(std::max(0.0f, itemY - 180.0f + 600.0f)); scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        if (!windowRef) return;
        if (readerDetailModal->isShown()) { dtCloseBtn->update(mousePos); return; }
        if (addReaderModal->isShown()) { inName->update(); inPhone->update(); inEmail->update(); inUser->update(); inPass->update(); btnAddSave->update(mousePos); btnAddCancel->update(mousePos); return; }
        sidebar->update(mousePos); searchField->update(); searchButton->update(mousePos); viewAllButton->update(mousePos); addReaderButton->update(mousePos);
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); if (pixelMouse.y > 180) { sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : readerItems) { if (item->box.getGlobalBounds().contains(listMousePos)) { item->box.setFillColor(sf::Color(255, 245, 250)); item->box.setOutlineColor(Theme::Secondary); } else { item->box.setFillColor(sf::Color::White); item->box.setOutlineColor(Theme::Border); } } }
    }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { if (addReaderModal->isShown()) { inName->handleEvent(event, mousePos); inPhone->handleEvent(event, mousePos); inEmail->handleEvent(event, mousePos); inUser->handleEvent(event, mousePos); inPass->handleEvent(event, mousePos); } else if (!readerDetailModal->isShown()) { searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); } }
    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!windowRef) return;
        if (addReaderModal->isShown()) { if (btnAddSave->handleClick(mousePos)) { handleAddReaderLogic(font); } else if (btnAddCancel->handleClick(mousePos) || addReaderModal->handleClose(mousePos)) { addReaderModal->hide(); } return; }
        if (readerDetailModal->isShown()) { if (dtCloseBtn->handleClick(mousePos) || readerDetailModal->handleClose(mousePos)) readerDetailModal->hide(); return; }
        if (addReaderButton->handleClick(mousePos)) { showAddModal(); return; } if (searchButton->handleClick(mousePos)) { loadReaders(font, searchField->getText()); return; } if (viewAllButton->handleClick(mousePos)) { searchField->clear(); loadReaders(font); return; }
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); if (pixelMouse.y > 180) { sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : readerItems) { if (item->box.getGlobalBounds().contains(listMousePos)) { std::string id = item->idText.getString(); NodeReader* t = libSystem->getDanhSachDocGia(); while(t) { if(t->data->getMaID() == id) { selectedReader = t->data; libSystem->DocDanhSachMuonCuaDocGia(selectedReader); prepareModalData(selectedReader, font); readerDetailModal->show(); break; } t = t->next; } break; } } }
    }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    void render(sf::RenderWindow& window) {
        window.setView(window.getDefaultView()); window.draw(background); sidebar->draw(window); window.draw(titleText); searchField->draw(window); searchButton->draw(window); viewAllButton->draw(window); addReaderButton->draw(window);
        window.setView(getListView()); for (auto item : readerItems) { window.draw(item->box); window.draw(item->idText); window.draw(item->nameText); window.draw(item->infoText); window.draw(item->statusText); }
        window.setView(window.getDefaultView());
        if (readerDetailModal->isShown()) { readerDetailModal->draw(window); window.draw(detailPanel); window.draw(dtTitle); window.draw(dtInfo); window.draw(headerSTT); window.draw(headerTen); window.draw(headerMuon); window.draw(headerTra); window.draw(headerTrangThai); window.draw(headerNgayQH); window.draw(headerPhat); for(const auto& row : detailBookRows) { window.draw(row.stt); window.draw(row.tenSach); window.draw(row.ngayMuon); window.draw(row.hanTra); window.draw(row.trangThai); window.draw(row.soNgayQH); window.draw(row.tienPhat); } dtCloseBtn->draw(window); }
        if (addReaderModal->isShown()) { addReaderModal->draw(window); window.draw(addPanel); window.draw(addTitle); inName->draw(window); inPhone->draw(window); inEmail->draw(window); inUser->draw(window); inPass->draw(window); window.draw(addMsg); btnAddSave->draw(window); btnAddCancel->draw(window); }
    }
};
#endif