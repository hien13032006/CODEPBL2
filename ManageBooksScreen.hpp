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

struct BookTableItem { RoundedRectangleShape box; sf::Text maSachText, tenSachText, tacGiaText, theLoaiText, namXBText, soLuongText; };

class ManageBooksScreen {
private:
    sf::RenderWindow* windowRef; sf::RectangleShape background; sf::Text titleText; Sidebar* sidebar; InputField* searchField; Button *searchButton, *addBookButton; 
    ScrollView* scrollView; std::vector<BookTableItem*> bookItems; std::vector<sf::Text*> tableHeaders;
    Modal* addBookModal; InputField *nameField, *authorField, *categoryField, *yearField, *publisherField, *quantityField, *imagePathField; 
    Button *submitButton, *updateSubmitButton, *cancelButton; sf::Text formTitle;
    LibrarySystem* libSystem; Sach* bookToEdit; bool isEditing;   
    sf::Font storedFont;

    sf::View getListView() {
        if (!windowRef) return sf::View(); sf::View view = windowRef->getDefaultView();
        view.setViewport(sf::FloatRect(250.f/1300.f, 180.f/720.f, 1030.f/1300.f, 540.f/720.f)); view.setSize(1030, 540); view.setCenter(250 + 1030/2, 180 + 540/2 + scrollView->getScrollOffset()); return view;
    }

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib), storedFont(font) {
        windowRef = nullptr; bookToEdit = nullptr; isEditing = false;
        background.setSize(sf::Vector2f(1300, 720)); background.setPosition(250, 0); background.setFillColor(Theme::Background);
        titleText.setFont(font); titleText.setString("QUAN LY SACH"); titleText.setCharacterSize(30); titleText.setFillColor(Theme::TextDark); titleText.setPosition(280, 30);
        sidebar = new Sidebar(font); scrollView = new ScrollView(sf::FloatRect(250, 180, 1030, 540));
        searchField = new InputField(sf::Vector2f(280, 80), sf::Vector2f(500, 50), "Tim sach, tac gia...", font);
        searchButton = new Button(sf::Vector2f(800, 80), sf::Vector2f(120, 50), "Tim", font, 0, Theme::Primary);
        addBookButton = new Button(sf::Vector2f(940, 75), sf::Vector2f(60, 60), "+", font, 0, Theme::Primary); addBookButton->getText().setCharacterSize(40); 
        addBookModal = new Modal(font); formTitle.setFont(font); formTitle.setCharacterSize(24); formTitle.setFillColor(Theme::Primary); formTitle.setPosition(470, 150);
        float formY = 200; nameField = new InputField(sf::Vector2f(450, formY), sf::Vector2f(500, 45), "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, formY + 60), sf::Vector2f(500, 45), "Tac gia", font); categoryField = new InputField(sf::Vector2f(450, formY + 120), sf::Vector2f(500, 45), "The loai", font);
        yearField = new InputField(sf::Vector2f(450, formY + 180), sf::Vector2f(240, 45), "Nam XB", font); publisherField = new InputField(sf::Vector2f(710, formY + 180), sf::Vector2f(240, 45), "Nha XB", font);
        quantityField = new InputField(sf::Vector2f(450, formY + 240), sf::Vector2f(240, 45), "So luong", font); imagePathField = new InputField(sf::Vector2f(450, formY + 300), sf::Vector2f(500, 45), "Link anh bia", font); 
        submitButton = new Button(sf::Vector2f(450, formY + 380), sf::Vector2f(240, 50), "Them Sach", font, 0, Theme::Primary);
        updateSubmitButton = new Button(sf::Vector2f(450, formY + 380), sf::Vector2f(240, 50), "Luu Thay Doi", font, 0, Theme::Secondary); 
        cancelButton = new Button(sf::Vector2f(710, formY + 380), sf::Vector2f(240, 50), "Huy", font, 0, sf::Color(150, 150, 150));
        loadBooksTable(font);
    }
    ~ManageBooksScreen() { delete sidebar; delete searchField; delete searchButton; delete addBookButton; delete scrollView; for (auto item : bookItems) delete item; for (auto header : tableHeaders) delete header; delete addBookModal; delete nameField; delete authorField; delete categoryField; delete yearField; delete publisherField; delete quantityField; delete imagePathField; delete submitButton; delete updateSubmitButton; delete cancelButton; }
    void setWindow(sf::RenderWindow* win) { windowRef = win; }
    void setBookToEdit(Sach* book) { bookToEdit = book; isEditing = true; }
    void showUpdateModal() { if (!bookToEdit) return; isEditing = true; formTitle.setString("CAP NHAT SACH: " + bookToEdit->getMaSach()); nameField->setText(bookToEdit->getTenSach()); authorField->setText(bookToEdit->getTacGia()); categoryField->setText(bookToEdit->getTheLoai()); yearField->setText(std::to_string(bookToEdit->getNamXuatBan())); publisherField->setText(bookToEdit->getNhaXuatBan()); quantityField->setText(std::to_string(bookToEdit->getSoLuong())); imagePathField->setText(bookToEdit->getImagePath()); addBookModal->show(); }
    void handleRowClick(const std::string& bookID) { NodeBook* cur = libSystem->getDanhSachSach(); while(cur) { if(cur->data->getMaSach() == bookID) { bookToEdit = cur->data; break; } cur = cur->next; } showUpdateModal(); }
    void handleAddBook() { std::ofstream out("ThemSach.txt", std::ios::app); out << nameField->getText() << "|" << authorField->getText() << "|" << categoryField->getText() << "|" << yearField->getText() << "|" << publisherField->getText() << "|" << quantityField->getText() << "|" << imagePathField->getText() << "\n"; out.close(); libSystem->DocFileSach("ThemSach.txt"); libSystem->GhiFileHeThong("DanhSachSach.txt"); loadBooksTable(storedFont); addBookModal->hide(); clearFields(); }
    void handleUpdateBookLogic() { if (!bookToEdit) return; bookToEdit->setTenSach(nameField->getText()); bookToEdit->setTacGia(authorField->getText()); bookToEdit->setTheLoai(categoryField->getText()); try { bookToEdit->setNamXuatBan(std::stoi(yearField->getText())); } catch(...) {} bookToEdit->setNhaXuatBan(publisherField->getText()); try { bookToEdit->setSoLuong(std::stoi(quantityField->getText())); } catch(...) {} bookToEdit->setImagePath(imagePathField->getText()); libSystem->GhiFileHeThong("DanhSachSach.txt"); loadBooksTable(storedFont); addBookModal->hide(); clearFields(); isEditing = false; bookToEdit = nullptr; }
    void clearFields() { nameField->clear(); authorField->clear(); categoryField->clear(); yearField->clear(); publisherField->clear(); quantityField->clear(); imagePathField->clear(); bookToEdit = nullptr; isEditing = false; }

    void loadBooksTable(sf::Font& font, std::string keyword = "") {
        for (auto item : bookItems) delete item; bookItems.clear(); for (auto header : tableHeaders) delete header; tableHeaders.clear(); if (!libSystem) return;
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower); const float ITEM_HEIGHT = 80.0f; const int FONT_SIZE = 18; float itemY = 180; 
        std::vector<std::pair<std::string, float>> headers = { {"MA SACH", 280}, {"TEN SACH", 400}, {"TAC GIA", 700}, {"THE LOAI", 900}, {"NAM XB", 1060}, {"SL", 1170} };
        for (const auto& pair : headers) { sf::Text* headerText = new sf::Text(pair.first, font, 16); headerText->setFillColor(Theme::TextLight); headerText->setPosition(pair.second, 150); tableHeaders.push_back(headerText); }
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            Sach* book = current->data; std::string ten = book->getTenSach(), tg = book->getTacGia(), tl = book->getTheLoai(), ma = book->getMaSach(); std::string sLower = ten + tg + tl + ma; std::transform(sLower.begin(), sLower.end(), sLower.begin(), ::tolower);
            if (keyword.empty() || sLower.find(keyword) != std::string::npos) {
                BookTableItem* item = new BookTableItem(); item->box.setSize(sf::Vector2f(980, ITEM_HEIGHT - 10)); item->box.setCornerRadius(8.0f); item->box.setPosition(270, itemY); item->box.setFillColor(sf::Color::White); item->box.setOutlineThickness(1); item->box.setOutlineColor(Theme::Border);
                auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = Theme::TextDark) { txt.setFont(font); txt.setString(s); txt.setCharacterSize(FONT_SIZE); txt.setFillColor(c); txt.setPosition(std::floor(x), std::floor(itemY + (ITEM_HEIGHT/2) - 12)); };
                setupText(item->maSachText, book->getMaSach(), 280, Theme::Primary); std::string tName = book->getTenSach(); if(tName.size() > 35) tName = tName.substr(0, 32) + "..."; setupText(item->tenSachText, tName, 400);
                std::string tAuth = book->getTacGia(); if(tAuth.size() > 25) tAuth = tAuth.substr(0, 22) + "..."; setupText(item->tacGiaText, tAuth, 700, Theme::TextLight); setupText(item->theLoaiText, book->getTheLoai(), 900, Theme::TextLight); setupText(item->namXBText, std::to_string(book->getNamXuatBan()), 1060); sf::Color slColor = book->getSoLuong() > 0 ? Theme::Success : Theme::Error; setupText(item->soLuongText, std::to_string(book->getSoLuong()), 1170, slColor);
                bookItems.push_back(item); itemY += ITEM_HEIGHT;
            } current = current->next;
        }
        // FIX: Padding 600.0f
        scrollView->setMaxScroll(std::max(0.0f, itemY - 180.0f + 600.0f)); scrollView->reset();
    }

    void update() { if (!windowRef) return; if (addBookModal->isShown()) { nameField->update(); authorField->update(); categoryField->update(); yearField->update(); publisherField->update(); quantityField->update(); imagePathField->update(); if(isEditing) updateSubmitButton->update(sf::Vector2f(0,0)); else submitButton->update(sf::Vector2f(0,0)); cancelButton->update(sf::Vector2f(0,0)); return; } sidebar->update(sf::Vector2f(0,0)); sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); if (pixelMouse.y > 180) { sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : bookItems) { if (item->box.getGlobalBounds().contains(listMousePos)) { item->box.setFillColor(sf::Color(255, 245, 250)); } else { item->box.setFillColor(sf::Color::White); } } } }
    int handleSidebarClick(sf::Vector2f mousePos) { return sidebar->handleClick(mousePos); }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { if (addBookModal->isShown()) { nameField->handleEvent(event, mousePos); authorField->handleEvent(event, mousePos); categoryField->handleEvent(event, mousePos); yearField->handleEvent(event, mousePos); publisherField->handleEvent(event, mousePos); quantityField->handleEvent(event, mousePos); imagePathField->handleEvent(event, mousePos); } else { searchField->handleEvent(event, mousePos); scrollView->handleScroll(event, mousePos); } }
    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) { if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) { addBookModal->hide(); clearFields(); return 0; } if (isEditing) { if(updateSubmitButton->handleClick(mousePos)) { handleUpdateBookLogic(); return 5; } } else { if(submitButton->handleClick(mousePos)) { handleAddBook(); return 1; } } } 
        else { if (addBookButton->handleClick(mousePos)) { addBookModal->show(); isEditing = false; clearFields(); formTitle.setString("THEM SACH MOI"); return 2; } if (searchButton->handleClick(mousePos)) { loadBooksTable(storedFont, searchField->getText()); return 3; } sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); if (pixelMouse.y > 180) { sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : bookItems) { if (item->box.getGlobalBounds().contains(listMousePos)) { handleRowClick(item->maSachText.getString()); return 7; } } } } return 0;
    }
    std::string getClickedBookId(sf::Vector2f mousePos) { sf::Vector2i pixelMouse = sf::Mouse::getPosition(*windowRef); sf::View listView = getListView(); sf::Vector2f listMousePos = windowRef->mapPixelToCoords(pixelMouse, listView); for (auto item : bookItems) { if (item->box.getGlobalBounds().contains(listMousePos)) return item->maSachText.getString(); } return ""; }
    void setUserRole(UserRole role, sf::Font& font) { sidebar->setUserRole(role, font); } Sidebar* getSidebar() { return sidebar; }
    void render() { if (!windowRef) return; windowRef->setView(windowRef->getDefaultView()); windowRef->draw(background); sidebar->draw(*windowRef); windowRef->draw(titleText); searchField->draw(*windowRef); searchButton->draw(*windowRef); addBookButton->draw(*windowRef); for (auto header : tableHeaders) windowRef->draw(*header); windowRef->setView(getListView()); for (auto item : bookItems) { windowRef->draw(item->box); windowRef->draw(item->maSachText); windowRef->draw(item->tenSachText); windowRef->draw(item->tacGiaText); windowRef->draw(item->theLoaiText); windowRef->draw(item->namXBText); windowRef->draw(item->soLuongText); } windowRef->setView(windowRef->getDefaultView()); if (addBookModal->isShown()) { addBookModal->draw(*windowRef); sf::RectangleShape formBg({600, 500}); formBg.setPosition(400, 130); formBg.setFillColor(sf::Color::White); formBg.setOutlineColor(Theme::Primary); formBg.setOutlineThickness(2); windowRef->draw(formBg); windowRef->draw(formTitle); nameField->draw(*windowRef); authorField->draw(*windowRef); categoryField->draw(*windowRef); yearField->draw(*windowRef); publisherField->draw(*windowRef); quantityField->draw(*windowRef); imagePathField->draw(*windowRef); if (isEditing) updateSubmitButton->draw(*windowRef); else submitButton->draw(*windowRef); cancelButton->draw(*windowRef); } }
};
#endif