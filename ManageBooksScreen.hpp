#ifndef MANAGE_BOOKS_SCREEN_HPP
#define MANAGE_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <cmath> 
#include "Sidebar.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"
#include "Node.h"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"

struct BookTableItem {
    RoundedRectangleShape box;
    sf::Text maSachText;
    sf::Text tenSachText;
    sf::Text tacGiaText;
    sf::Text theLoaiText;
    sf::Text namXBText;
    sf::Text soLuongText;
};

class ManageBooksScreen {
private:
    sf::RenderWindow* windowRef;
    sf::Font storedFont; 

    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    InputField* searchField;
    Button* searchButton;
    Button* addBookButton; 
    
    ScrollView* scrollView;
    std::vector<BookTableItem*> bookItems;
    std::vector<sf::Text*> tableHeaders;

    Modal* addBookModal;
    
    // Form fields
    InputField* nameField;
    InputField* authorField;
    InputField* categoryField;
    InputField* yearField;
    InputField* publisherField;
    InputField* quantityField;
    InputField* imagePathField; 
    
    Button* submitButton; 
    Button* updateSubmitButton; 
    Button* cancelButton;
    sf::Text formTitle;
    
    LibrarySystem* libSystem;
    Sach* bookToEdit; 
    bool isEditing;   

    // Helper để lấy View của danh sách (Dùng chung cho Render và Update chuột)
    sf::View getListView() {
        sf::View view = windowRef->getDefaultView();
        // Viewport: X=250, Y=180, W=1150, H=720
        view.setViewport(sf::FloatRect(250.f/1400.f, 180.f/900.f, 1150.f/1400.f, 720.f/900.f));
        view.setSize(1150, 720);
        view.setCenter(250 + 1150/2, 180 + 720/2 + scrollView->getScrollOffset());
        return view;
    }

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib), storedFont(font) {
        windowRef = nullptr;
        bookToEdit = nullptr;
        isEditing = false;
        
        // Background
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        // Title
        titleText.setFont(font);
        titleText.setString("Quan Ly Sach");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        // Vùng Scroll: Y=180, Cao=720
        scrollView = new ScrollView(sf::FloatRect(250, 180, 1150, 720));

        // Search & Buttons
        searchField = new InputField(sf::Vector2f(280, 80), sf::Vector2f(500, 50), "Tim kiem sach...", font);
        searchButton = new Button(sf::Vector2f(800, 80), sf::Vector2f(120, 50), "Tim", font, 0, sf::Color(60, 60, 70));

        addBookButton = new Button(sf::Vector2f(940, 75), sf::Vector2f(60, 60), "+", font, 0, sf::Color(76, 175, 80));
        addBookButton->getText().setCharacterSize(40); 

        // Modal Setup
        addBookModal = new Modal(font);
        formTitle.setFont(font); formTitle.setCharacterSize(24); formTitle.setFillColor(sf::Color(255, 193, 94)); formTitle.setPosition(470, 200);

        nameField = new InputField(sf::Vector2f(450, 270), sf::Vector2f(500, 45), "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, 330), sf::Vector2f(500, 45), "Tac gia", font);
        categoryField = new InputField(sf::Vector2f(450, 390), sf::Vector2f(500, 45), "The loai", font);
        yearField = new InputField(sf::Vector2f(450, 450), sf::Vector2f(240, 45), "Nam XB", font);
        publisherField = new InputField(sf::Vector2f(710, 450), sf::Vector2f(240, 45), "Nha XB", font);
        quantityField = new InputField(sf::Vector2f(450, 510), sf::Vector2f(240, 45), "So luong", font);
        imagePathField = new InputField(sf::Vector2f(450, 570), sf::Vector2f(500, 45), "Link anh bia", font); 

        submitButton = new Button(sf::Vector2f(450, 640), sf::Vector2f(240, 50), "Them Sach", font, 0, sf::Color(76, 175, 80));
        updateSubmitButton = new Button(sf::Vector2f(450, 640), sf::Vector2f(240, 50), "Luu Thay Doi", font, 0, sf::Color(255, 150, 0)); 
        cancelButton = new Button(sf::Vector2f(710, 640), sf::Vector2f(240, 50), "Huy", font, 0, sf::Color(120, 120, 120));

        loadBooksTable(font);
    }

    ~ManageBooksScreen() {
        delete sidebar; delete searchField; delete searchButton; delete addBookButton; delete scrollView;
        for (auto item : bookItems) delete item;
        for (auto header : tableHeaders) delete header;
        delete addBookModal; delete nameField; delete authorField; delete categoryField;
        delete yearField; delete publisherField; delete quantityField; delete imagePathField;
        delete submitButton; delete updateSubmitButton; delete cancelButton;
    }

    void setWindow(sf::RenderWindow* win) { windowRef = win; }
    void setBookToEdit(Sach* book) { bookToEdit = book; isEditing = true; }
    
    void showUpdateModal() {
        if (!bookToEdit) return;
        nameField->setText(bookToEdit->getTenSach()); authorField->setText(bookToEdit->getTacGia());
        categoryField->setText(bookToEdit->getTheLoai()); yearField->setText(std::to_string(bookToEdit->getNamXuatBan()));
        publisherField->setText(bookToEdit->getNhaXuatBan()); quantityField->setText(std::to_string(bookToEdit->getSoLuong()));
        imagePathField->setText(bookToEdit->getImagePath());
        formTitle.setString("Cap Nhat: " + bookToEdit->getMaSach());
        addBookModal->show(); isEditing = true;
    }

    void handleAddBook() {
        std::ofstream out("ThemSach.txt", std::ios::app);
        out << nameField->getText() << "|" << authorField->getText() << "|" << categoryField->getText() << "|"
            << yearField->getText() << "|" << publisherField->getText() << "|" << quantityField->getText() << "|"
            << imagePathField->getText() << "\n";
        out.close();
        libSystem->DocFileSach("ThemSach.txt"); libSystem->GhiFileHeThong("DanhSachSach.txt");
        loadBooksTable(storedFont); addBookModal->hide(); clearFields();
    }

    void handleUpdateBookLogic() {
        if (!bookToEdit) return;
        bookToEdit->setTenSach(nameField->getText()); bookToEdit->setTacGia(authorField->getText());
        bookToEdit->setTheLoai(categoryField->getText());
        try { bookToEdit->setNamXuatBan(std::stoi(yearField->getText())); } catch(...) {}
        bookToEdit->setNhaXuatBan(publisherField->getText());
        try { bookToEdit->setSoLuong(std::stoi(quantityField->getText())); } catch(...) {}
        bookToEdit->setImagePath(imagePathField->getText());
        libSystem->GhiFileHeThong("DanhSachSach.txt"); loadBooksTable(storedFont); addBookModal->hide(); clearFields();
    }

    void clearFields() {
        nameField->clear(); authorField->clear(); categoryField->clear(); yearField->clear(); 
        publisherField->clear(); quantityField->clear(); imagePathField->clear();
        bookToEdit = nullptr; isEditing = false;
    }

    void loadBooksTable(sf::Font& font) {
        for (auto item : bookItems) delete item; bookItems.clear();
        for (auto header : tableHeaders) delete header; tableHeaders.clear();

        if (!libSystem) return;

        // --- CẤU HÌNH KÍCH THƯỚC ---
        const float ITEM_HEIGHT = 80.0f; 
        const int FONT_SIZE = 18;
        float itemY = 180; 

        // Header
        std::vector<std::pair<std::string, float>> headers = {
            {"MA SACH", 280}, {"TEN SACH", 390}, {"TAC GIA", 600}, 
            {"THE LOAI", 780}, {"NAM XB", 920}, {"SL", 1000}
        };
        for (const auto& pair : headers) {
            sf::Text* headerText = new sf::Text(pair.first, font, 16);
            headerText->setFillColor(sf::Color(180, 180, 180));
            headerText->setPosition(pair.second, 150);
            tableHeaders.push_back(headerText);
        }

        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            Sach* book = current->data;
            BookTableItem* item = new BookTableItem();

            // Box
            item->box.setSize(sf::Vector2f(850, ITEM_HEIGHT - 10));
            item->box.setCornerRadius(8.0f);
            item->box.setPosition(270, itemY);
            item->box.setFillColor(sf::Color(35, 38, 50));

            // Helper
            auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = sf::Color::White) {
                txt.setFont(font); txt.setString(s); 
                txt.setCharacterSize(FONT_SIZE); 
                txt.setFillColor(c);
                txt.setPosition(std::floor(x), std::floor(itemY + (ITEM_HEIGHT/2) - 12)); 
            };

            setupText(item->maSachText, book->getMaSach(), 280, sf::Color(255, 193, 94));
            std::string tName = book->getTenSach(); if(tName.size()>28) tName = tName.substr(0,25)+"...";
            setupText(item->tenSachText, tName, 390);
            std::string tAuth = book->getTacGia(); if(tAuth.size()>20) tAuth = tAuth.substr(0,17)+"...";
            setupText(item->tacGiaText, tAuth, 600, sf::Color(220, 220, 220));
            setupText(item->theLoaiText, book->getTheLoai(), 780, sf::Color(220, 220, 220));
            setupText(item->namXBText, std::to_string(book->getNamXuatBan()), 920);
            sf::Color slColor = book->getSoLuong() > 0 ? sf::Color(100, 255, 100) : sf::Color(255, 80, 80);
            setupText(item->soLuongText, std::to_string(book->getSoLuong()), 1000, slColor);

            bookItems.push_back(item);
            itemY += ITEM_HEIGHT;
            current = current->next;
        }
        
        scrollView->setMaxScroll(itemY - 180);
        scrollView->reset();
    }

    void update() {
        if (!windowRef) return;
        sf::Vector2f mousePos = windowRef->mapPixelToCoords(sf::Mouse::getPosition(*windowRef));
        
        if (addBookModal->isShown()) {
            nameField->update(); authorField->update(); categoryField->update(); yearField->update(); 
            publisherField->update(); quantityField->update(); imagePathField->update();
            if(isEditing) updateSubmitButton->update(mousePos); else submitButton->update(mousePos);
            cancelButton->update(mousePos);
            return;
        }

        sidebar->update(mousePos); addBookButton->update(mousePos);
        searchField->update(); searchButton->update(mousePos);

        // --- SỬA LỖI LỆCH CHUỘT ---
        // 1. Lấy tọa độ pixel của chuột
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
        
        // 2. Chỉ xử lý hover nếu chuột nằm trong vùng danh sách (Y > 180)
        if (pixelMouse.y > 180) {
            // 3. Chuyển đổi pixel sang tọa độ thế giới (World Coords) dựa trên View Danh sách
            sf::View listView = getListView();
            sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView);

            for (auto item : bookItems) {
                // Kiểm tra va chạm với tọa độ đã được map chuẩn xác
                if (item->box.getGlobalBounds().contains(listMousePos)) {
                    item->box.setFillColor(sf::Color(50, 55, 75)); // Hover sáng
                } else {
                    item->box.setFillColor(sf::Color(35, 38, 50)); // Bình thường
                }
            }
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (addBookModal->isShown()) {
            nameField->handleEvent(event, mousePos); authorField->handleEvent(event, mousePos); categoryField->handleEvent(event, mousePos);
            yearField->handleEvent(event, mousePos); publisherField->handleEvent(event, mousePos); quantityField->handleEvent(event, mousePos);
            imagePathField->handleEvent(event, mousePos);
        } else {
            searchField->handleEvent(event, mousePos);
            scrollView->handleScroll(event, mousePos);
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) {
            if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) { addBookModal->hide(); clearFields(); return 0; }
            if (isEditing) { if(updateSubmitButton->handleClick(mousePos)) return 5; } else { if(submitButton->handleClick(mousePos)) return 1; }
        } else {
            if (addBookButton->handleClick(mousePos)) { addBookModal->show(); isEditing = false; clearFields(); return 2; }
            
            // --- SỬA LỖI CLICK ---
            // Lấy tọa độ pixel chuột (mousePos truyền vào là World Coords của Default View, tương đương Pixel Coords trong trường hợp này)
            sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
            
            if (pixelMouse.y > 180) { 
                // Map sang tọa độ danh sách
                sf::View listView = getListView();
                sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView);

                for (auto item : bookItems) {
                    if (item->box.getGlobalBounds().contains(listMousePos)) return 7; 
                }
            }
        }
        return 0;
    }

    std::string getClickedBookId(sf::Vector2f mousePos) {
        // Lại dùng logic map tọa độ
        sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef);
        sf::View listView = getListView();
        sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView);

        for (auto item : bookItems) {
            if (item->box.getGlobalBounds().contains(listMousePos)) return item->maSachText.getString();
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); }
    Sidebar* getSidebar() { return sidebar; }

    void render() {
        if (!windowRef) return;
        
        // 1. Vẽ Tĩnh (Header, Search, Sidebar)
        windowRef->setView(windowRef->getDefaultView());
        windowRef->draw(background); sidebar->draw(*windowRef); windowRef->draw(titleText);
        searchField->draw(*windowRef); searchButton->draw(*windowRef); addBookButton->draw(*windowRef);
        for (auto header : tableHeaders) windowRef->draw(*header);

        // 2. Vẽ Danh Sách Cuộn (Sử dụng Helper getListView để đồng bộ)
        windowRef->setView(getListView());

        for (auto item : bookItems) {
            windowRef->draw(item->box); windowRef->draw(item->maSachText); windowRef->draw(item->tenSachText);
            windowRef->draw(item->tacGiaText); windowRef->draw(item->theLoaiText); windowRef->draw(item->namXBText);
            windowRef->draw(item->soLuongText);
        }
        
        // 3. Modal (Vẽ đè lên cùng)
        windowRef->setView(windowRef->getDefaultView());
        if (addBookModal->isShown()) {
            addBookModal->draw(*windowRef); windowRef->draw(formTitle);
            nameField->draw(*windowRef); authorField->draw(*windowRef); categoryField->draw(*windowRef);
            yearField->draw(*windowRef); publisherField->draw(*windowRef); quantityField->draw(*windowRef);
            imagePathField->draw(*windowRef);
            if (isEditing) updateSubmitButton->draw(*windowRef); else submitButton->draw(*windowRef);
            cancelButton->draw(*windowRef);
        }
    }
};
#endif