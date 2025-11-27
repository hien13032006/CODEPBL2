#ifndef MANAGE_BOOKS_SCREEN_HPP
#define MANAGE_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <cmath> 
#include <algorithm> 
#include "Sidebar.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"
#include "Node.h"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"
#include "Theme.hpp"

struct BookTableItem { 
    RoundedRectangleShape box; 
    sf::Texture coverTexture; 
    sf::Sprite coverSprite;   
    sf::Text maSachText, tenSachText, tacGiaText, theLoaiText, namXBText, soLuongText; 
};

enum class AddMode { SELECTION, MANUAL, AUTO_FILE };

class ManageBooksScreen {
private:
    sf::RenderWindow* windowRef; sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar; InputField* searchField; Button *searchButton, *addBookButton; 
    ScrollView* scrollView; std::vector<BookTableItem*> bookItems; std::vector<sf::Text*> tableHeaders;
    
    Modal* addBookModal; 
    sf::Text formTitle;
    AddMode currentMode; 

    Button *btnOptManual, *btnOptFile;
    InputField *nameField, *authorField, *categoryField, *yearField, *publisherField, *quantityField, *imagePathField; 
    Button *submitButton, *updateSubmitButton; 
    InputField *fileNameField;
    Button *btnExecFile; 
    Button *cancelButton; 

    LibrarySystem* libSystem; Sach* bookToEdit; bool isEditing;   
    sf::Font storedFont;

    sf::View getListView() {
        if (!windowRef) return sf::View(); sf::View view = windowRef->getDefaultView();
        view.setViewport(sf::FloatRect(250.f/1300.f, 180.f/720.f, 1050.f/1300.f, 540.f/720.f)); 
        view.setSize(1050, 540); 
        view.setCenter(250 + 1050/2, 180 + 540/2 + scrollView->getScrollOffset()); return view;
    }

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib), storedFont(font) {
        windowRef = nullptr; bookToEdit = nullptr; isEditing = false;
        currentMode = AddMode::SELECTION; 

        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("QUAN LY SACH"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        scrollView = new ScrollView(sf::FloatRect(250, 180, 1050, 540)); 
        sidebar = new Sidebar(font);
        searchField = new InputField(sf::Vector2f(280, 80), sf::Vector2f(500, 50), "Tim sach, tac gia...", font);
        searchButton = new Button(sf::Vector2f(800, 80), sf::Vector2f(120, 50), "Tim", font, 0, Theme::Primary);
        addBookButton = new Button(sf::Vector2f(940, 75), sf::Vector2f(60, 60), "+", font, 0, Theme::Primary); addBookButton->getText().setCharacterSize(40); 
        
        addBookModal = new Modal(font); 
        formTitle.setFont(font); formTitle.setCharacterSize(24); formTitle.setFillColor(Theme::Primary); formTitle.setPosition(470, 150);

        btnOptManual = new Button({450, 250}, {240, 150}, "NHAP THU CONG", font, 10, Theme::Secondary);
        btnOptFile = new Button({710, 250}, {240, 150}, "THEM TU FILE", font, 11, Theme::Primary);
        
        float formY = 200; 
        nameField = new InputField(sf::Vector2f(450, formY), sf::Vector2f(500, 45), "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, formY + 60), sf::Vector2f(500, 45), "Tac gia", font); 
        categoryField = new InputField(sf::Vector2f(450, formY + 120), sf::Vector2f(500, 45), "The loai", font);
        yearField = new InputField(sf::Vector2f(450, formY + 180), sf::Vector2f(240, 45), "Nam XB", font); 
        publisherField = new InputField(sf::Vector2f(710, formY + 180), sf::Vector2f(240, 45), "Nha XB", font);
        quantityField = new InputField(sf::Vector2f(450, formY + 240), sf::Vector2f(240, 45), "So luong", font); 
        imagePathField = new InputField(sf::Vector2f(450, formY + 300), sf::Vector2f(500, 45), "Link anh bia", font); 
        
        submitButton = new Button(sf::Vector2f(450, formY + 380), sf::Vector2f(240, 50), "Luu Sach", font, 0, Theme::Success);
        updateSubmitButton = new Button(sf::Vector2f(450, formY + 380), sf::Vector2f(240, 50), "Cap Nhat", font, 0, Theme::Secondary); 
        
        fileNameField = new InputField({450, 280}, {500, 50}, "Nhap ten file...", font);
        fileNameField->setText("ThemSach.txt"); 
        btnExecFile = new Button({450, 350}, {240, 50}, "Nap Du Lieu", font, 12, Theme::Primary);

        cancelButton = new Button(sf::Vector2f(710, formY + 380), sf::Vector2f(240, 50), "Huy", font, 0, sf::Color(150, 150, 150));

        loadBooksTable(font);
    }

    ~ManageBooksScreen() { 
        delete sidebar; delete searchField; delete searchButton; delete addBookButton; delete scrollView; 
        for (auto item : bookItems) delete item; for (auto header : tableHeaders) delete header; 
        delete addBookModal; 
        delete nameField; delete authorField; delete categoryField; delete yearField; delete publisherField; delete quantityField; delete imagePathField; 
        delete fileNameField;
        delete submitButton; delete updateSubmitButton; delete cancelButton; delete btnExecFile; delete btnOptManual; delete btnOptFile;
    }

    void setWindow(sf::RenderWindow* win) { windowRef = win; }
    
    void setBookToEdit(Sach* book) { 
        bookToEdit = book; 
        if (book) {
            isEditing = true;
            currentMode = AddMode::MANUAL; 
        }
    }

    void handleRowClick(const std::string& bookID) { 
        NodeBook* cur = libSystem->getDanhSachSach(); 
        while(cur) { if(cur->data->getMaSach() == bookID) { bookToEdit = cur->data; break; } cur = cur->next; } 
        showUpdateModal(); 
    }

    void showUpdateModal() { 
        if (!bookToEdit) return; 
        isEditing = true; 
        currentMode = AddMode::MANUAL; 
        formTitle.setString("CAP NHAT SACH: " + bookToEdit->getMaSach()); 
        
        nameField->setText(bookToEdit->getTenSach()); 
        authorField->setText(bookToEdit->getTacGia()); 
        categoryField->setText(bookToEdit->getTheLoai()); 
        yearField->setText(std::to_string(bookToEdit->getNamXuatBan())); 
        publisherField->setText(bookToEdit->getNhaXuatBan()); 
        quantityField->setText(std::to_string(bookToEdit->getSoLuong())); 
        imagePathField->setText(bookToEdit->getImagePath()); 
        
        addBookModal->show(); 
    }

    void handleAddManual() { 
        std::ofstream out("TempManualBook.txt"); 
        if(out.is_open()){
            out << nameField->getText() << "|" << authorField->getText() << "|" << categoryField->getText() << "|" << yearField->getText() << "|" << publisherField->getText() << "|" << quantityField->getText() << "|" << imagePathField->getText() << "\n"; 
            out.close(); 
            libSystem->DocFileSach("TempManualBook.txt"); 
            libSystem->GhiFileHeThong("DanhSachSach.txt"); 
            loadBooksTable(storedFont); 
            addBookModal->hide(); 
            clearFields(); 
        }
    }

    void handleAddFromFile() {
        std::string fname = fileNameField->getText();
        if(fname.empty()) fname = "ThemSach.txt";
        libSystem->DocFileSach(fname);
        libSystem->GhiFileHeThong("DanhSachSach.txt");
        loadBooksTable(storedFont);
        addBookModal->hide();
    }

    void handleUpdateBookLogic() { 
        if (!bookToEdit) return; 
        bookToEdit->setTenSach(nameField->getText()); 
        bookToEdit->setTacGia(authorField->getText()); 
        bookToEdit->setTheLoai(categoryField->getText()); 
        try { bookToEdit->setNamXuatBan(std::stoi(yearField->getText())); } catch(...) {} 
        bookToEdit->setNhaXuatBan(publisherField->getText()); 
        try { bookToEdit->setSoLuong(std::stoi(quantityField->getText())); } catch(...) {} 
        bookToEdit->setImagePath(imagePathField->getText()); 
        libSystem->GhiFileHeThong("DanhSachSach.txt"); 
        loadBooksTable(storedFont); 
        addBookModal->hide(); 
        clearFields(); 
        isEditing = false; bookToEdit = nullptr; 
    }

    void clearFields() { 
        nameField->clear(); authorField->clear(); categoryField->clear(); yearField->clear(); publisherField->clear(); quantityField->clear(); imagePathField->clear(); fileNameField->setText("ThemSach.txt");
        bookToEdit = nullptr; isEditing = false; 
    }

    void loadBooksTable(sf::Font& font, std::string keyword = "") {
        // [MỚI] Tải lại toàn bộ dữ liệu mượn để đảm bảo tính toán chính xác
        if(libSystem) {
            libSystem->DocTatCaDanhSachMuon();
        }

        for (auto item : bookItems) delete item; bookItems.clear(); for (auto header : tableHeaders) delete header; tableHeaders.clear(); if (!libSystem) return;
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower); const float ITEM_HEIGHT = 80.0f; const int FONT_SIZE = 18; float itemY = 180; 
        
        // [CẬP NHẬT TỌA ĐỘ] Căn chỉnh lại để tránh đè chữ
        std::vector<std::pair<std::string, float>> headers = { {"MA SACH", 340}, {"TEN SACH", 440}, {"TAC GIA", 740}, {"THE LOAI", 960}, {"NAM XB", 1110}, {"SL", 1210} };
        for (const auto& pair : headers) { sf::Text* headerText = new sf::Text(pair.first, font, 16); headerText->setFillColor(Theme::TextLight); headerText->setPosition(pair.second, 150); tableHeaders.push_back(headerText); }
        
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            Sach* book = current->data; std::string ten = book->getTenSach(), tg = book->getTacGia(), tl = book->getTheLoai(), ma = book->getMaSach(); std::string sLower = ten + tg + tl + ma; std::transform(sLower.begin(), sLower.end(), sLower.begin(), ::tolower);
            if (keyword.empty() || sLower.find(keyword) != std::string::npos) {
                BookTableItem* item = new BookTableItem(); 
                item->box.setSize(sf::Vector2f(1030, ITEM_HEIGHT - 10)); 
                item->box.setCornerRadius(8.0f); item->box.setPosition(270, itemY); item->box.setFillColor(sf::Color::White); item->box.setOutlineThickness(1); item->box.setOutlineColor(Theme::Border);

                if (!book->getImagePath().empty() && item->coverTexture.loadFromFile(book->getImagePath())) {
                    item->coverSprite.setTexture(item->coverTexture);
                    float imgW = 50.0f; float imgH = 70.0f;
                    item->coverSprite.setScale(imgW / item->coverTexture.getSize().x, imgH / item->coverTexture.getSize().y);
                    item->coverSprite.setPosition(280, itemY); 
                }

                auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = Theme::TextDark) { txt.setFont(font); txt.setString(s); txt.setCharacterSize(FONT_SIZE); txt.setFillColor(c); txt.setPosition(std::floor(x), std::floor(itemY + (ITEM_HEIGHT/2) - 12)); };
                
                setupText(item->maSachText, book->getMaSach(), 340, Theme::Primary); 
                
                std::string tName = book->getTenSach(); if(tName.size() > 28) tName = tName.substr(0, 25) + "..."; 
                setupText(item->tenSachText, tName, 440); 
                
                // Cắt tên tác giả ngắn lại để không đè lên Thể loại
                std::string tAuth = book->getTacGia(); if(tAuth.size() > 22) tAuth = tAuth.substr(0, 19) + "..."; 
                setupText(item->tacGiaText, tAuth, 740, Theme::TextLight); 
                
                setupText(item->theLoaiText, book->getTheLoai(), 960, Theme::TextLight); 
                setupText(item->namXBText, std::to_string(book->getNamXuatBan()), 1110); 
                
                // [LOGIC HIỂN THỊ SỐ LƯỢNG]
                int conLai = book->getSoLuong();
                int dangMuon = libSystem->DemTongSachDangMuon(book->getMaSach());
                int tongSo = conLai + dangMuon;
                
                std::string slStr = std::to_string(conLai) + "/" + std::to_string(tongSo);
                sf::Color slColor = conLai > 0 ? Theme::Success : Theme::Error; 
                setupText(item->soLuongText, slStr, 1210, slColor); 
                
                bookItems.push_back(item); itemY += ITEM_HEIGHT;
            } current = current->next;
        }
        scrollView->setMaxScroll(std::max(0.0f, itemY - 180.0f + 600.0f)); scrollView->reset();
    }

    void update() { 
        if (!windowRef) return; 
        sf::Vector2f mousePos = windowRef->mapPixelToCoords(sf::Mouse::getPosition(*windowRef));

        if (addBookModal->isShown()) { 
            if (currentMode == AddMode::SELECTION) {
                btnOptManual->update(mousePos); 
                btnOptFile->update(mousePos);
            }
            else if (currentMode == AddMode::MANUAL) {
                nameField->update(); authorField->update(); categoryField->update(); yearField->update(); publisherField->update(); quantityField->update(); imagePathField->update(); 
                if(isEditing) updateSubmitButton->update(mousePos); else submitButton->update(mousePos); 
            }
            else if (currentMode == AddMode::AUTO_FILE) {
                fileNameField->update();
                btnExecFile->update(mousePos);
            }
            cancelButton->update(mousePos);
            return; 
        } 
        
        sidebar->update(sf::Vector2f(0,0)); 
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); 
        if (pixelMouse.y > 180) { 
            sf::View listView = getListView(); 
            sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); 
            for (auto item : bookItems) { 
                if (item->box.getGlobalBounds().contains(listMousePos)) { item->box.setFillColor(sf::Color(255, 245, 250)); } else { item->box.setFillColor(sf::Color::White); } 
            } 
        } 
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { 
        if (addBookModal->isShown()) { 
            if (currentMode == AddMode::MANUAL) {
                nameField->handleEvent(event, mousePos); authorField->handleEvent(event, mousePos); categoryField->handleEvent(event, mousePos); yearField->handleEvent(event, mousePos); publisherField->handleEvent(event, mousePos); quantityField->handleEvent(event, mousePos); imagePathField->handleEvent(event, mousePos); 
            } else if (currentMode == AddMode::AUTO_FILE) {
                fileNameField->handleEvent(event, mousePos);
            }
        } else { 
            searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); 
        } 
    }

    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) { 
            if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) { 
                addBookModal->hide(); clearFields(); 
                return 0; 
            } 
            if (currentMode == AddMode::SELECTION) {
                if (btnOptManual->handleClick(mousePos)) {
                    currentMode = AddMode::MANUAL;
                    formTitle.setString("THEM SACH THU CONG");
                    cancelButton->setPosition({710, 580}); 
                }
                else if (btnOptFile->handleClick(mousePos)) {
                    currentMode = AddMode::AUTO_FILE;
                    formTitle.setString("THEM TU FILE");
                    cancelButton->setPosition({710, 350});
                }
            }
            else if (currentMode == AddMode::MANUAL) {
                if (isEditing) { 
                    if(updateSubmitButton->handleClick(mousePos)) { handleUpdateBookLogic(); return 5; } 
                } else { 
                    if(submitButton->handleClick(mousePos)) { handleAddManual(); return 1; } 
                } 
            }
            else if (currentMode == AddMode::AUTO_FILE) {
                if (btnExecFile->handleClick(mousePos)) { handleAddFromFile(); return 1; }
            }
        } 
        else { 
            if (addBookButton->handleClick(mousePos)) { 
                addBookModal->show(); 
                isEditing = false; 
                clearFields(); 
                currentMode = AddMode::SELECTION; 
                formTitle.setString("CHON PHUONG THUC THEM");
                return 2; 
            } 
            if (searchButton->handleClick(mousePos)) { loadBooksTable(storedFont, searchField->getText()); return 3; } 
            
            sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); 
            if (pixelMouse.y > 180) { 
                sf::View listView = getListView(); 
                sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); 
                for (auto item : bookItems) { 
                    if (item->box.getGlobalBounds().contains(listMousePos)) { 
                        handleRowClick(item->maSachText.getString()); return 7; 
                    } 
                } 
            } 
        } 
        return 0;
    }

    std::string getClickedBookId(sf::Vector2f mousePos) { sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : bookItems) { if (item->box.getGlobalBounds().contains(listMousePos)) return item->maSachText.getString(); } return ""; }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    
    void render() { 
        if (!windowRef) return; 
        windowRef->setView(windowRef->getDefaultView()); 
        windowRef->draw(background); sidebar->draw(*windowRef); windowRef->draw(titleText); searchField->draw(*windowRef); searchButton->draw(*windowRef); addBookButton->draw(*windowRef); 
        for (auto header : tableHeaders) windowRef->draw(*header); 
        
        windowRef->setView(getListView()); 
        for (auto item : bookItems) { 
            windowRef->draw(item->box); 
            if (item->coverTexture.getSize().x > 0) windowRef->draw(item->coverSprite);
            windowRef->draw(item->maSachText); windowRef->draw(item->tenSachText); windowRef->draw(item->tacGiaText); windowRef->draw(item->theLoaiText); windowRef->draw(item->namXBText); windowRef->draw(item->soLuongText); 
        } 
        windowRef->setView(windowRef->getDefaultView()); 
        
        if (addBookModal->isShown()) { 
            addBookModal->draw(*windowRef); 
            sf::RectangleShape formBg({600, 500}); formBg.setPosition(400, 130); formBg.setFillColor(sf::Color::White); formBg.setOutlineColor(Theme::Primary); formBg.setOutlineThickness(2); windowRef->draw(formBg); 
            windowRef->draw(formTitle); 

            if (currentMode == AddMode::SELECTION) {
                btnOptManual->draw(*windowRef);
                btnOptFile->draw(*windowRef);
            }
            else if (currentMode == AddMode::MANUAL) {
                nameField->draw(*windowRef); authorField->draw(*windowRef); categoryField->draw(*windowRef); yearField->draw(*windowRef); publisherField->draw(*windowRef); quantityField->draw(*windowRef); imagePathField->draw(*windowRef); 
                if (isEditing) updateSubmitButton->draw(*windowRef); else submitButton->draw(*windowRef); 
                cancelButton->draw(*windowRef);
            }
            else if (currentMode == AddMode::AUTO_FILE) {
                fileNameField->draw(*windowRef);
                btnExecFile->draw(*windowRef);
                cancelButton->draw(*windowRef);
            }
        } 
    }
};
#endif