#ifndef BOOK_DETAIL_SCREEN_HPP
#define BOOK_DETAIL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Modal.hpp"
#include "Button.hpp"
#include "Book.h"
#include "State.hpp"
#include "USER.h"
#include "Reader.h"
#include "Node.h" 
#include "LibrarySystem.h"

struct BorrowerItem {
    sf::Text nameText; sf::Text dateText; sf::Text statusText; sf::Color statusColor;
};

class BookDetailScreen {
private:
    Modal* modal;
    sf::RectangleShape coverBox; sf::Texture bookCoverTexture; sf::Sprite bookCoverSprite;   
    sf::Text titleText; sf::Text infoLabels; sf::Text infoValues;
    sf::Text borrowerTitle; sf::Text notBorrowedText;
    std::vector<BorrowerItem> borrowerItems; 
    Button* closeButton; Button* borrowButton;
    Sach* currentBook; UserRole userRole;

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // Các thông số vị trí được chỉnh để phù hợp Modal 900x600
        coverBox.setSize(sf::Vector2f(220, 330)); coverBox.setPosition(280, 180); // Ảnh lớn
        coverBox.setFillColor(sf::Color(60, 100, 180));

        titleText.setFont(font); titleText.setCharacterSize(28); // Tên sách to
        titleText.setFillColor(sf::Color(255, 193, 94)); titleText.setPosition(530, 180);

        infoLabels.setFont(font); infoLabels.setCharacterSize(18); // Label 18
        infoLabels.setFillColor(sf::Color(180, 180, 180)); infoLabels.setPosition(530, 230);
        infoLabels.setLineSpacing(1.5f);

        infoValues.setFont(font); infoValues.setCharacterSize(18); // Value 18
        infoValues.setFillColor(sf::Color::White); infoValues.setPosition(650, 230);
        infoValues.setLineSpacing(1.5f);
        
        borrowerTitle.setFont(font); borrowerTitle.setCharacterSize(20); borrowerTitle.setFillColor(sf::Color(150, 150, 255)); borrowerTitle.setPosition(530, 450);
        notBorrowedText.setFont(font); notBorrowedText.setCharacterSize(16); notBorrowedText.setFillColor(sf::Color(150, 150, 150)); notBorrowedText.setPosition(530, 480); notBorrowedText.setString("Chua co doc gia muon.");

        closeButton = new Button(sf::Vector2f(1000, 680), sf::Vector2f(120, 45), "Dong", font, 0, sf::Color(100, 100, 120));
        borrowButton = new Button(sf::Vector2f(850, 680), sf::Vector2f(120, 45), "Muon Sach", font, 0, sf::Color(76, 175, 80));
        userRole = UserRole::NONE;
    }

    ~BookDetailScreen() { delete closeButton; delete borrowButton; }

    void setBook(Sach* book, UserRole role, LibrarySystem* libSystem) { 
        currentBook = book; userRole = role; borrowerItems.clear();
        std::vector<sf::Color> colors = {sf::Color(200, 80, 60), sf::Color(60, 140, 200), sf::Color(100, 180, 100), sf::Color(200, 150, 60), sf::Color(140, 80, 180)};
        
        bookCoverTexture = sf::Texture(); 
        if (!book->getImagePath().empty() && bookCoverTexture.loadFromFile(book->getImagePath())) {
            bookCoverSprite.setTexture(bookCoverTexture);
            float scaleX = coverBox.getSize().x / bookCoverTexture.getSize().x;
            float scaleY = coverBox.getSize().y / bookCoverTexture.getSize().y;
            bookCoverSprite.setScale(scaleX, scaleY); bookCoverSprite.setPosition(coverBox.getPosition());
            coverBox.setFillColor(sf::Color::Transparent);
        } else { coverBox.setFillColor(colors[rand() % 5]); }

        std::string title = book->getTenSach(); if (title.length() > 40) title = title.substr(0, 37) + "...";
        titleText.setString(title);
        
        infoLabels.setString("Ma sach:\nTac gia:\nThe loai:\nNam XB:\nNha XB:\nSo luong:\nDanh gia:");
        char ratingStr[50]; sprintf(ratingStr, "%.1f/10 (%d luot)", book->getDiemTrungBinh(), book->getSoDanhGia());
        std::string values = book->getMaSach() + "\n" + book->getTacGia() + "\n" + book->getTheLoai() + "\n" +
                             std::to_string(book->getNamXuatBan()) + "\n" + book->getNhaXuatBan() + "\n" +
                             std::to_string(book->getSoLuong()) + " cuon\n" + std::string(ratingStr);
        infoValues.setString(values);

        if (role == UserRole::LIBRARIAN && libSystem) {
             std::vector<BorrowerInfo> results = libSystem->TimNguoiMuonSach(book->getMaSach());
             borrowerTitle.setString("Nguoi Dang Muon (" + std::to_string(results.size()) + "):");
             float itemY = 480; const float TEXT_SIZE = 16;
             for (const auto& res : results) {
                 BorrowerItem item; const sf::Font& f = closeButton->getFont();
                 item.nameText.setFont(f); item.dateText.setFont(f); item.statusText.setFont(f);
                 item.nameText.setString(res.maDocGia + " - " + res.tenDocGia);
                 item.dateText.setString(timeString(res.ngayHetHan)); // Chỉ hiện hạn trả cho gọn
                 item.statusText.setString(res.daQuaHan ? "QUA HAN" : "OK");
                 item.statusColor = res.daQuaHan ? sf::Color(255, 100, 100) : sf::Color(100, 200, 100);
                 
                 item.nameText.setCharacterSize(TEXT_SIZE); item.nameText.setPosition(530, itemY); item.nameText.setFillColor(sf::Color::White);
                 item.dateText.setCharacterSize(TEXT_SIZE); item.dateText.setPosition(800, itemY); item.dateText.setFillColor(sf::Color(180, 180, 180));
                 item.statusText.setCharacterSize(TEXT_SIZE); item.statusText.setPosition(950, itemY); item.statusText.setFillColor(item.statusColor);
                 borrowerItems.push_back(item); itemY += 25;
             }
        }
    }

    void update(sf::Vector2f mousePos) { closeButton->update(mousePos); if (userRole == UserRole::READER) borrowButton->update(mousePos); }

    int handleClick(sf::Vector2f mousePos) {
        if (closeButton->handleClick(mousePos)) return 0;
        if (userRole == UserRole::READER && borrowButton->handleClick(mousePos)) return 1;
        return -1;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            modal->draw(window); // Vẽ nền modal trước
            window.draw(coverBox); if (bookCoverTexture.getSize().x > 0) window.draw(bookCoverSprite);
            window.draw(titleText); window.draw(infoLabels); window.draw(infoValues);
            
            if (userRole == UserRole::LIBRARIAN) {
                window.draw(borrowerTitle);
                if (borrowerItems.empty()) window.draw(notBorrowedText);
                else for (const auto& item : borrowerItems) { window.draw(item.nameText); window.draw(item.dateText); window.draw(item.statusText); }
            } else if (userRole == UserRole::READER) {
                borrowButton->draw(window);
            }
            closeButton->draw(window);
        }
    }
    Sach* getCurrentBook() { return currentBook; }
};

#endif