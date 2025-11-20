#ifndef MANAGE_READERS_SCREEN_HPP
#define MANAGE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip> 
#include <ctime>   
#include <locale>
#include <cmath> // Cần thiết cho abs()
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
    sf::Text maSachText;
    sf::Text tenSachText;
    sf::Text ngayMuonText;
    sf::Text ngayHetHanText;
    sf::Text trangThaiText; 
};

struct ReaderDisplayItem {
    sf::RectangleShape box;
    sf::Text idText;
    sf::Text nameText;
    sf::Text infoText;
    sf::Text statusText;
};

class ManageReadersScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    InputField* searchField;
    Button* searchButton;
    Button* viewAllButton;
    std::vector<ReaderDisplayItem*> readerItems;
    LibrarySystem* libSystem;
    
    // THÔNG TIN MODAL CHI TIẾT ĐỘC GIẢ
    Modal* readerDetailModal; 
    Reader* selectedReader = nullptr; 
    sf::Text detailTitle;
    sf::Text detailInfo;
    sf::Text detailFine;
    
    std::vector<sf::Text*> borrowedListHeaders; 
    std::vector<BorrowedDetailItem*> borrowedItems; 

public:
    ManageReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Quan Ly Doc Gia");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900)); 

        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(400, 50),
                                     "Tim kiem doc gia...", font);
        searchButton = new Button(sf::Vector2f(700, 100), sf::Vector2f(150, 50),
                                 "Tim", font);
        viewAllButton = new Button(sf::Vector2f(870, 100), sf::Vector2f(180, 50),
                                  "Xem Tat Ca", font, 0, sf::Color(100, 150, 200));
        
        // Khởi tạo Modal chi tiết
        readerDetailModal = new Modal(font);
        readerDetailModal->getModalBox().setSize(sf::Vector2f(1000, 750)); 
        readerDetailModal->getModalBox().setPosition(200, 75);
        
        detailTitle.setFont(font);
        detailTitle.setCharacterSize(24);
        detailTitle.setFillColor(sf::Color(255, 193, 94));
        detailTitle.setPosition(230, 105);
        
        detailInfo.setFont(font);
        detailInfo.setCharacterSize(16);
        detailInfo.setFillColor(sf::Color::White);
        detailInfo.setPosition(230, 155);

        detailFine.setFont(font);
        detailFine.setCharacterSize(18);
        detailFine.setFillColor(sf::Color(255, 100, 100));
        detailFine.setPosition(230, 205);

        loadReaders(font);
    }

    ~ManageReadersScreen() {
        delete sidebar;
        delete scrollView;
        delete searchField;
        delete searchButton;
        delete viewAllButton;
        delete readerDetailModal;
        for (auto item : readerItems) delete item;
        for (auto text : borrowedListHeaders) delete text;
        for (auto item : borrowedItems) delete item;
    }

    void loadReaders(sf::Font& font) {
        loadReaders(font, "");
    }

    void loadReaders(sf::Font& font, const std::string& keyword) {
        for (auto item : readerItems) delete item;
        readerItems.clear();

        if (!libSystem) return;

        std::string keywordLower = keyword;
        std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

        NodeReader* current = libSystem->getDanhSachDocGia();
        float yPos = 180;

        while (current != nullptr) {
            Reader* docGia = current->data;
            std::string maID = docGia->getMaID();
            std::string hoTen = docGia->getHoTen();
            std::string email = docGia->getEmail();
            std::string sdt = docGia->getSDT();
            
            std::string infoForSearch = maID + hoTen + email + sdt;
            std::transform(infoForSearch.begin(), infoForSearch.end(), infoForSearch.begin(), ::tolower);
            
            if (keyword.empty() || infoForSearch.find(keywordLower) != std::string::npos) {
                
                ReaderDisplayItem* item = new ReaderDisplayItem();
                
                item->box.setSize(sf::Vector2f(1000, 90));
                item->box.setPosition(300, yPos);
                item->box.setFillColor(sf::Color(25, 28, 40));
                item->box.setOutlineThickness(1);
                item->box.setOutlineColor(sf::Color(40, 43, 55));

                item->idText.setFont(font);
                item->idText.setString(maID);
                item->idText.setCharacterSize(18);
                item->idText.setFillColor(sf::Color(255, 193, 94));
                item->idText.setPosition(320, yPos + 15);

                item->nameText.setFont(font);
                item->nameText.setString(hoTen);
                item->nameText.setCharacterSize(16);
                item->nameText.setFillColor(sf::Color::White);
                item->nameText.setPosition(420, yPos + 15);

                item->infoText.setFont(font);
                std::string info = "Email: " + email + " | SDT: " + sdt;
                item->infoText.setString(info);
                item->infoText.setCharacterSize(13);
                item->infoText.setFillColor(sf::Color(150, 150, 150));
                item->infoText.setPosition(420, yPos + 40);

                int daMuon = docGia->DemSachDaMuon();
                int quaHan = docGia->DemSachQuaHan();
                
                item->statusText.setFont(font);
                std::string status = "Dang muon: " + std::to_string(daMuon) + "/" + 
                                    std::to_string(docGia->getGioiHanMuon()) + " cuon";
                if (quaHan > 0) {
                    status += " | QUA HAN: " + std::to_string(quaHan);
                    item->statusText.setFillColor(sf::Color(255, 100, 100));
                } else {
                    item->statusText.setFillColor(sf::Color(100, 180, 100));
                }
                item->statusText.setString(status);
                item->statusText.setCharacterSize(14);
                item->statusText.setPosition(420, yPos + 62);

                readerItems.push_back(item);
                yPos += 100;
            }
            current = current->next;
        }

        scrollView->setMaxScroll(std::max(0.0f, yPos - 600.0f));
    }

    void showReaderDetailModal(sf::Font& font) {
        if (!selectedReader) return;
        
        // 1. Dọn dẹp danh sách cũ
        for (auto text : borrowedListHeaders) delete text;
        borrowedListHeaders.clear();
        for (auto item : borrowedItems) delete item;
        borrowedItems.clear();

        // 2. Cấu hình tiêu đề và thông tin cơ bản
        detailTitle.setString("Chi Tiet Doc Gia: " + selectedReader->getHoTen());
        detailInfo.setString("Ma ID: " + selectedReader->getMaID() + " | Username: " + selectedReader->getUsername() + 
                             "\nEmail: " + selectedReader->getEmail() + " | SDT: " + selectedReader->getSDT());

        // 3. Tính toán phạt
        long long tongNgayQuaHan = 0;
        NodeMuonSach* phieuNode = selectedReader->getDanhSachPhieuMuon();
        while (phieuNode != nullptr) {
            if (phieuNode->phieu->daQuaHan()) {
                // Đảm bảo số ngày quá hạn là số dương
                tongNgayQuaHan += std::abs(phieuNode->phieu->soNgayConLai()); 
            }
            phieuNode = phieuNode->next;
        }
        long long tongTienPhat = tongNgayQuaHan * 5000; // 5000 VND/ngày

        std::stringstream ss;
        ss.imbue(std::locale(""));
        ss << "Tong tien phat qua han: " << tongTienPhat << " VND"; 
        detailFine.setString(ss.str());

        // 4. Liệt kê sách đang mượn DẠNG BẢNG
        phieuNode = selectedReader->getDanhSachPhieuMuon();
        float itemY = 320; 
        const float ITEM_HEIGHT = 50.0f;
        const float PADDING = 10.0f;
        const float TEXT_OFFSET_Y = ITEM_HEIGHT/2 - 10;
        
        // 4.1 Thêm Header 
        sf::Text* headerLabel = new sf::Text("SACH DANG MUON:", font, 18);
        headerLabel->setFillColor(sf::Color(150, 150, 255));
        headerLabel->setPosition(230, itemY - 50);
        borrowedListHeaders.push_back(headerLabel);

        sf::Text* headerMa = new sf::Text(L"Mã", font, 14); 
        headerMa->setFillColor(sf::Color(150, 150, 150));
        headerMa->setPosition(240 + PADDING, itemY - 25); borrowedListHeaders.push_back(headerMa);
        
        sf::Text* headerTen = new sf::Text(L"Tên Sách", font, 14); 
        headerTen->setFillColor(sf::Color(150, 150, 150));
        headerTen->setPosition(330 + PADDING, itemY - 25); borrowedListHeaders.push_back(headerTen);
        
        sf::Text* headerNgayM = new sf::Text(L"Ngày Mượn", font, 14); 
        headerNgayM->setFillColor(sf::Color(150, 150, 150));
        headerNgayM->setPosition(580 + PADDING, itemY - 25); borrowedListHeaders.push_back(headerNgayM);
        
        sf::Text* headerHanT = new sf::Text(L"Hạn Trả", font, 14); 
        headerHanT->setFillColor(sf::Color(150, 150, 150));
        headerHanT->setPosition(730 + PADDING, itemY - 25); borrowedListHeaders.push_back(headerHanT);
        
        sf::Text* headerQH = new sf::Text(L"Tình Trạng", font, 14); 
        headerQH->setFillColor(sf::Color(150, 150, 150));
        headerQH->setPosition(880 + PADDING, itemY - 25); borrowedListHeaders.push_back(headerQH);
        
        itemY += 5; 

        // 4.2 Thêm các hàng dữ liệu sách
        if (selectedReader->DemSachDaMuon() == 0) {
            sf::Text* noBook = new sf::Text("Khong co sach nao dang muon.", font, 14);
            noBook->setFillColor(sf::Color(150, 150, 150));
            noBook->setPosition(230, itemY + PADDING);
            borrowedListHeaders.push_back(noBook); 
        }

        while (phieuNode != nullptr) {
            PhieuMuonSach* phieu = phieuNode->phieu;
            BorrowedDetailItem* item = new BorrowedDetailItem();
            
            // 1. Box bo góc
            item->box.setSize(sf::Vector2f(960, ITEM_HEIGHT));
            item->box.setCornerRadius(8.0f); 
            item->box.setPosition(220, itemY);
            item->box.setFillColor(sf::Color(40, 45, 60));
            
            // Xác định màu trạng thái
            sf::Color textColor;
            std::stringstream statusSS;
            if (phieu->daQuaHan()) {
                textColor = sf::Color(255, 100, 100); 
                // Hiển thị số ngày quá hạn (số dương)
                statusSS << std::abs(phieu->soNgayConLai()) << " ngay (QH)"; 
            } else if (phieu->soNgayConLai() <= 7) {
                textColor = sf::Color(255, 193, 94); 
                statusSS << phieu->soNgayConLai() << " Ngay (CL)";
            } else {
                textColor = sf::Color(100, 180, 100); 
                statusSS << phieu->soNgayConLai() << " ngay (CL)";
            }

            // 2. Text Mã Sách
            item->maSachText.setFont(font); item->maSachText.setString(phieu->sach->getMaSach());
            item->maSachText.setCharacterSize(14); item->maSachText.setFillColor(sf::Color(255, 193, 94));
            item->maSachText.setPosition(240 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // 3. Text Tên Sách
            std::string tenSach = phieu->sach->getTenSach();
            if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
            item->tenSachText.setFont(font); item->tenSachText.setString(tenSach);
            item->tenSachText.setCharacterSize(14); item->tenSachText.setFillColor(sf::Color::White);
            item->tenSachText.setPosition(330 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // 4. Text Ngày Mượn
            item->ngayMuonText.setFont(font); item->ngayMuonText.setString(timeString(phieu->ngayMuon));
            item->ngayMuonText.setCharacterSize(14); item->ngayMuonText.setFillColor(sf::Color(150, 150, 150));
            item->ngayMuonText.setPosition(580 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // 5. Text Ngày Hạn Trả
            item->ngayHetHanText.setFont(font); item->ngayHetHanText.setString(timeString(phieu->ngayHetHan));
            item->ngayHetHanText.setCharacterSize(14); item->ngayHetHanText.setFillColor(sf::Color(150, 150, 150));
            item->ngayHetHanText.setPosition(730 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // 6. Text Trạng Thái (Số ngày quá hạn/còn lại)
            item->trangThaiText.setFont(font); item->trangThaiText.setString(statusSS.str());
            item->trangThaiText.setCharacterSize(14); item->trangThaiText.setFillColor(textColor);
            item->trangThaiText.setPosition(880 + PADDING, itemY + TEXT_OFFSET_Y);

            borrowedItems.push_back(item);
            itemY += ITEM_HEIGHT + PADDING;
            phieuNode = phieuNode->next;
        }
        
        readerDetailModal->show();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchField->update();
        searchButton->update(mousePos);
        viewAllButton->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
        scrollView->handleScroll(event, mousePos);
    }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (readerDetailModal->isShown()) {
            if (readerDetailModal->handleClose(mousePos)) {
                readerDetailModal->hide();
                selectedReader = nullptr;
                // Giải phóng bộ nhớ của các đối tượng trong Modal
                for (auto text : borrowedListHeaders) delete text;
                borrowedListHeaders.clear();
                for (auto item : borrowedItems) delete item;
                borrowedItems.clear();
            }
            return;
        }

        if (searchButton->handleClick(mousePos)) {
            loadReaders(font, searchField->getText());
            return;
        }
        if (viewAllButton->handleClick(mousePos)) {
            searchField->clear();
            loadReaders(font); 
            return;
        }
        
        // Xử lý click vào item độc giả (ĐÃ SỬA LỖI SCROLL/CLICK)
        float scrollOffset = scrollView->getScrollOffset();
        // Cần dịch chuyển tọa độ chuột về gốc để so khớp với vị trí item
        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 

        for (size_t i = 0; i < readerItems.size(); ++i) {
            // Kiểm tra va chạm với adjustedMousePos
            if (readerItems[i]->box.getGlobalBounds().contains(adjustedMousePos)) {
                
                NodeReader* temp = libSystem->getDanhSachDocGia();
                // Di chuyển con trỏ temp đến đúng vị trí của item thứ i
                for (size_t j = 0; j < i && temp != nullptr; j++) {
                    temp = temp->next;
                }
                
                if (temp) {
                    selectedReader = temp->data;
                    libSystem->DocDanhSachMuonCuaDocGia(selectedReader); // Tải lại danh sách mượn
                    showReaderDetailModal(font);
                    break;
                }
            }
        }
    }
    
    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        searchField->draw(window);
        searchButton->draw(window);
        viewAllButton->draw(window);

        // Áp dụng View cuộn
        sf::View originalView = window.getView();
        sf::View scrolledView = originalView;
        scrolledView.move(0, -scrollView->getScrollOffset());
        window.setView(scrolledView);

        for (auto item : readerItems) {
            window.draw(item->box);
            window.draw(item->idText);
            window.draw(item->nameText);
            window.draw(item->infoText);
            window.draw(item->statusText);
        }
        
        // Khôi phục View
        window.setView(originalView);
        
        // Vẽ Modal Chi tiết Độc giả (luôn ở trên cùng)
        if (readerDetailModal->isShown()) {
            readerDetailModal->draw(window);
            window.draw(detailTitle);
            window.draw(detailInfo);
            window.draw(detailFine);
            
            // Vẽ Header của Bảng
            for (auto text : borrowedListHeaders) {
                window.draw(*text);
            }

            // Vẽ các Item trong Bảng
            for (auto item : borrowedItems) {
                window.draw(item->box);
                window.draw(item->maSachText);
                window.draw(item->tenSachText);
                window.draw(item->ngayMuonText);
                window.draw(item->ngayHetHanText);
                window.draw(item->trangThaiText);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif