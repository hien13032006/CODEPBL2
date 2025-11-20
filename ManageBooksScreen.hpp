#ifndef MANAGE_BOOKS_SCREEN_HPP
#define MANAGE_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"
#include "Node.h"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"

// Cấu trúc cho mỗi hàng sách trong bảng
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
    sf::Font storedFont; // FIX: Lưu trữ font để dùng trong loadBooksTable

    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    InputField* searchField;
    Button* searchButton;
    Button* addBookButton; 
    ScrollView* scrollView;

    // Bảng sách
    std::vector<BookTableItem*> bookItems;
    std::vector<sf::Text*> tableHeaders;

    Modal* addBookModal;
    
    // Form thêm sách (giữ nguyên các InputFields)
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

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib), storedFont(font) {
        windowRef = nullptr;
        bookToEdit = nullptr;
        isEditing = false;
        
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Quan Ly Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));

        searchField = new InputField(sf::Vector2f(280, 80), sf::Vector2f(500, 45),
                                     "Tim kiem sach theo ma, ten, tac gia...", font);
        searchButton = new Button(sf::Vector2f(800, 80), sf::Vector2f(100, 45),
                                 "Tim", font, 0, sf::Color(60, 60, 70));

        addBookButton = new Button(sf::Vector2f(920, 75), sf::Vector2f(60, 60),
                                  "+", font, 0, sf::Color(76, 175, 80));
        addBookButton->getText().setCharacterSize(36); 
        addBookButton->getText().setStyle(sf::Text::Bold);

        // Modal thêm sách
        addBookModal = new Modal(font);
        formTitle.setFont(font);
        formTitle.setCharacterSize(24);
        formTitle.setFillColor(sf::Color(30, 30, 30));
        formTitle.setPosition(470, 200);

        // Form Fields
        nameField = new InputField(sf::Vector2f(450, 270), sf::Vector2f(500, 45), "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, 330), sf::Vector2f(500, 45), "Tac gia", font);
        categoryField = new InputField(sf::Vector2f(450, 390), sf::Vector2f(500, 45), "The loai", font);
        yearField = new InputField(sf::Vector2f(450, 450), sf::Vector2f(240, 45), "Nam xuat ban", font);
        publisherField = new InputField(sf::Vector2f(710, 450), sf::Vector2f(240, 45), "Nha xuat ban", font);
        quantityField = new InputField(sf::Vector2f(450, 510), sf::Vector2f(240, 45), "So luong", font);
        imagePathField = new InputField(sf::Vector2f(450, 570), sf::Vector2f(500, 45), "Duong dan anh (VD: images/book.png)", font); 

        submitButton = new Button(sf::Vector2f(450, 640), sf::Vector2f(240, 50), "Them Vao Thu Vien", font, 0, sf::Color(76, 175, 80));
        updateSubmitButton = new Button(sf::Vector2f(450, 640), sf::Vector2f(240, 50), "Cap Nhat Sach", font, 0, sf::Color(255, 150, 0)); 
        cancelButton = new Button(sf::Vector2f(710, 640), sf::Vector2f(240, 50), "Huy", font, 0, sf::Color(120, 120, 120));

        loadBooksTable(font);
    }

    ~ManageBooksScreen() {
        delete sidebar;
        delete searchField;
        delete searchButton;
        delete addBookButton;
        delete scrollView;
        
        for (auto item : bookItems) delete item;
        for (auto header : tableHeaders) delete header;

        delete addBookModal;
        delete nameField;
        delete authorField;
        delete categoryField;
        delete yearField;
        delete publisherField;
        delete quantityField;
        delete imagePathField;
        delete submitButton;
        delete updateSubmitButton;
        delete cancelButton;
    }

    void setWindow(sf::RenderWindow* win) {
        windowRef = win;
    }
    
    void setBookToEdit(Sach* book) { 
        bookToEdit = book;
        isEditing = true;
    }
    
    void showUpdateModal() {
        if (!bookToEdit) return;
        
        // FIX: Sử dụng setText và setPlaceholder đã được sửa
        nameField->setText(bookToEdit->getTenSach());
        authorField->setText(bookToEdit->getTacGia());
        categoryField->setText(bookToEdit->getTheLoai());
        yearField->setText(std::to_string(bookToEdit->getNamXuatBan()));
        publisherField->setText(bookToEdit->getNhaXuatBan());
        quantityField->setText(std::to_string(bookToEdit->getSoLuong()));
        
        // FIX: Đặt placeholder dựa trên trạng thái (sử dụng hàm đã thêm vào InputField)
        if (bookToEdit->getSoLuong() == 0) {
            quantityField->setPlaceholder("Hết hàng"); 
        } else {
            quantityField->setPlaceholder("Số lượng"); 
        }

        imagePathField->setText(bookToEdit->getImagePath());
        
        formTitle.setString("Cap Nhat Thong Tin Sach: " + bookToEdit->getMaSach());
        addBookModal->show();
        isEditing = true;
    }

    void loadBooksTable(sf::Font& font) {
        for (auto item : bookItems) delete item;
        bookItems.clear();
        for (auto header : tableHeaders) delete header;
        tableHeaders.clear();

        if (!libSystem) return;

        // 1. Tạo Header Bảng
        std::vector<std::pair<std::string, float>> headers = {
            {"Ma Sach", 280}, {"Ten Sach", 380}, {"Tac Gia", 550}, 
            {"The Loai", 700}, {"Nam XB", 850}, {"SL", 950}
        };
        for (const auto& pair : headers) {
            sf::Text* headerText = new sf::Text(pair.first, font, 14);
            headerText->setFillColor(sf::Color(150, 150, 150));
            headerText->setPosition(pair.second, 150);
            tableHeaders.push_back(headerText);
        }

        // 2. Tải và tạo các hàng dữ liệu
        NodeBook* current = libSystem->getDanhSachSach();
        float itemY = 190;
        const float ITEM_HEIGHT = 45.0f;
        const float PADDING = 5.0f;
        const float TEXT_OFFSET_Y = ITEM_HEIGHT / 2 - 10;

        while (current != nullptr) {
            Sach* book = current->data;
            BookTableItem* item = new BookTableItem();

            // Box bo góc
            item->box.setSize(sf::Vector2f(720, ITEM_HEIGHT));
            item->box.setCornerRadius(8.0f);
            item->box.setPosition(270, itemY);
            item->box.setFillColor(sf::Color(25, 28, 40));

            // Ma Sach
            item->maSachText.setFont(font); item->maSachText.setString(book->getMaSach());
            item->maSachText.setCharacterSize(14); item->maSachText.setFillColor(sf::Color(255, 193, 94));
            item->maSachText.setPosition(280 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // Ten Sach
            std::string tenSach = book->getTenSach();
            if (tenSach.length() > 20) tenSach = tenSach.substr(0, 17) + "...";
            item->tenSachText.setFont(font); item->tenSachText.setString(tenSach);
            item->tenSachText.setCharacterSize(14); item->tenSachText.setFillColor(sf::Color::White);
            item->tenSachText.setPosition(380 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // Tac Gia
            std::string tacGia = book->getTacGia();
            if (tacGia.length() > 15) tacGia = tacGia.substr(0, 12) + "...";
            item->tacGiaText.setFont(font); item->tacGiaText.setString(tacGia);
            item->tacGiaText.setCharacterSize(14); item->tacGiaText.setFillColor(sf::Color(150, 150, 150));
            item->tacGiaText.setPosition(550 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // The Loai
            item->theLoaiText.setFont(font); item->theLoaiText.setString(book->getTheLoai());
            item->theLoaiText.setCharacterSize(14); item->theLoaiText.setFillColor(sf::Color(150, 150, 150));
            item->theLoaiText.setPosition(700 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // Nam XB
            item->namXBText.setFont(font); item->namXBText.setString(std::to_string(book->getNamXuatBan()));
            item->namXBText.setCharacterSize(14); item->namXBText.setFillColor(sf::Color(150, 150, 150));
            item->namXBText.setPosition(850 + PADDING, itemY + TEXT_OFFSET_Y);
            
            // So Luong
            item->soLuongText.setFont(font); item->soLuongText.setString(std::to_string(book->getSoLuong()));
            item->soLuongText.setCharacterSize(14); 
            item->soLuongText.setFillColor(book->getSoLuong() > 0 ? sf::Color(100, 180, 100) : sf::Color(200, 80, 80));
            item->soLuongText.setPosition(950 + PADDING, itemY + TEXT_OFFSET_Y);

            bookItems.push_back(item);
            itemY += ITEM_HEIGHT + PADDING;
            current = current->next;
        }
        
        scrollView->setMaxScroll(std::max(0.0f, itemY - 900 + 150)); 
        scrollView->reset();
    }

    void handleUpdateBookLogic() {
        if (!bookToEdit) return;

        std::string name = nameField->getText();
        std::string author = authorField->getText();
        std::string category = categoryField->getText();
        
        int year = 0;
        try { year = std::stoi(yearField->getText()); } catch (...) { year = bookToEdit->getNamXuatBan(); }
        int quantity = 0;
        try { quantity = std::stoi(quantityField->getText()); } catch (...) { quantity = bookToEdit->getSoLuong(); }
        
        std::string publisher = publisherField->getText();
        std::string imagePath = imagePathField->getText();

        // Cập nhật các trường dữ liệu
        bookToEdit->setTenSach(name);
        bookToEdit->setTacGia(author);
        bookToEdit->setTheLoai(category);
        bookToEdit->setNamXuatBan(year);
        bookToEdit->setNhaXuatBan(publisher);
        bookToEdit->setSoLuong(quantity);
        bookToEdit->setImagePath(imagePath);

        libSystem->GhiFileHeThong("DanhSachSach.txt");
        // FIX: Dùng storedFont
        loadBooksTable(storedFont); 
        std::cout << "Da cap nhat sach: " << bookToEdit->getMaSach() << std::endl;

        addBookModal->hide();
        clearFields();
    }

    void handleAddBook() {
        std::string name = nameField->getText();
        std::string author = authorField->getText();
        std::string category = categoryField->getText();
        std::string yearStr = yearField->getText();
        std::string publisher = publisherField->getText();
        std::string quantityStr = quantityField->getText();
        std::string imagePath = imagePathField->getText();
        
        if (name.empty() || author.empty() || category.empty() || 
            yearStr.empty() || publisher.empty() || quantityStr.empty()) {
            std::cout << "Loi: Vui long dien day du thong tin!" << std::endl;
            return;
        }
        
        std::ofstream out("ThemSach.txt", std::ios::app);
        if (out.is_open()) {
            out << name << "|" << author << "|" << category << "|"
                << yearStr << "|" << publisher << "|" << quantityStr << "|"
                << imagePath << "\n";
            out.close();
        }
        
        libSystem->DocFileSach("ThemSach.txt");
        libSystem->GhiFileHeThong("DanhSachSach.txt");
        
        // FIX: Dùng storedFont
        loadBooksTable(storedFont); 
        std::cout << "Da them sach: " << name << std::endl;
        
        addBookModal->hide();
        clearFields();
    }

    void clearFields() {
        nameField->clear(); authorField->clear(); categoryField->clear(); yearField->clear(); 
        publisherField->clear(); quantityField->clear(); imagePathField->clear();
        bookToEdit = nullptr; isEditing = false; formTitle.setString("Them Sach Moi");
        // Đặt lại placeholder mặc định khi xóa
        quantityField->setPlaceholder("Số lượng"); 
    }

    void update() {
        if (!windowRef) return;
        sf::Vector2f mousePos = windowRef->mapPixelToCoords(sf::Mouse::getPosition(*windowRef));
        
        sidebar->update(mousePos); addBookButton->update(mousePos);
        searchField->update(); searchButton->update(mousePos);

        // Áp dụng scroll offset cho các item trong bảng khi hover
        float scrollOffset = scrollView->getScrollOffset();
        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);

        for (auto item : bookItems) {
             item->box.getGlobalBounds().contains(adjustedMousePos) ? 
             item->box.setFillColor(sf::Color(40, 50, 70)) : 
             item->box.setFillColor(sf::Color(25, 28, 40));
        }

        if (addBookModal->isShown()) {
            nameField->update(); authorField->update(); categoryField->update(); yearField->update(); 
            publisherField->update(); quantityField->update(); imagePathField->update();
            if (isEditing) { updateSubmitButton->update(mousePos); } else { submitButton->update(mousePos); }
            cancelButton->update(mousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }
    
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
        scrollView->handleScroll(event, mousePos);

        if (addBookModal->isShown()) {
            nameField->handleEvent(event, mousePos); authorField->handleEvent(event, mousePos);
            categoryField->handleEvent(event, mousePos); yearField->handleEvent(event, mousePos);
            publisherField->handleEvent(event, mousePos); quantityField->handleEvent(event, mousePos);
            imagePathField->handleEvent(event, mousePos);
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) {
            if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) { addBookModal->hide(); clearFields(); return 0; }
            if (isEditing) { if (updateSubmitButton->handleClick(mousePos)) return 5; } 
            else { if (submitButton->handleClick(mousePos)) return 1; }
        } else {
            if (addBookButton->handleClick(mousePos)) { addBookModal->show(); isEditing = false; clearFields(); return 2; }
            if (searchButton->handleClick(mousePos)) { /* Thực hiện tìm kiếm */ return 4; }
            
            // Xử lý click vào hàng sách
            float scrollOffset = scrollView->getScrollOffset();
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
            
            for (auto item : bookItems) {
                if (item->box.getGlobalBounds().contains(adjustedMousePos)) return 7; // Xem chi tiết sách
            }
        }
        return 0;
    }

    std::string getClickedBookId(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset); 
        for (auto item : bookItems) {
            if (item->box.getGlobalBounds().contains(adjustedMousePos)) return item->maSachText.getString();
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }
    Sidebar* getSidebar() { return sidebar; }
    void render() {
        if (!windowRef) return;
        windowRef->draw(background);
        sidebar->draw(*windowRef);
        windowRef->draw(titleText);
        searchField->draw(*windowRef);
        searchButton->draw(*windowRef);
        addBookButton->draw(*windowRef);

        // Áp dụng View cuộn cho bảng
        sf::View originalView = windowRef->getView();
        sf::View scrolledView = originalView;
        scrolledView.move(0, -scrollView->getScrollOffset());
        windowRef->setView(scrolledView);

        // Vẽ Header
        for (auto header : tableHeaders) windowRef->draw(*header);

        // Vẽ các Item trong bảng
        for (auto item : bookItems) {
            windowRef->draw(item->box);
            windowRef->draw(item->maSachText);
            windowRef->draw(item->tenSachText);
            windowRef->draw(item->tacGiaText);
            windowRef->draw(item->theLoaiText);
            windowRef->draw(item->namXBText);
            windowRef->draw(item->soLuongText);
        }
        
        windowRef->setView(originalView); // Khôi phục View

        // Vẽ Modal
        if (addBookModal->isShown()) {
            addBookModal->draw(*windowRef);
            windowRef->draw(formTitle);
            nameField->draw(*windowRef); authorField->draw(*windowRef); categoryField->draw(*windowRef);
            yearField->draw(*windowRef); publisherField->draw(*windowRef); quantityField->draw(*windowRef);
            imagePathField->draw(*windowRef);

            if (isEditing) { updateSubmitButton->draw(*windowRef); } else { submitButton->draw(*windowRef); }
            cancelButton->draw(*windowRef);
        }
    }
};

#endif