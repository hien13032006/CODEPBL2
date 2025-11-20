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
#include "LibrarySystem.h" // Cần include LibrarySystem

// NOTE: Hàm timeString(time_t) phải được định nghĩa toàn cục/inline trong một header file.

// Cấu trúc cho mỗi hàng người mượn (để hiển thị trong Modal)
struct BorrowerItem {
    sf::Text nameText;
    sf::Text dateText;
    sf::Text statusText;
    sf::Color statusColor;
};


class BookDetailScreen {
private:
    Modal* modal;
    sf::RectangleShape detailBox;
    sf::RectangleShape coverBox;
    sf::Texture bookCoverTexture; 
    sf::Sprite bookCoverSprite;   
    sf::Text titleText;
    sf::Text infoLabels;
    sf::Text infoValues;
    sf::Text borrowerTitle;
    sf::Text notBorrowedText;

    std::vector<BorrowerItem> borrowerItems; 

    Button* closeButton;
    Sach* currentBook;
    UserRole userRole;

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        detailBox.setSize(sf::Vector2f(1000, 750)); 
        detailBox.setPosition(200, 75);
        detailBox.setFillColor(sf::Color(30, 35, 50));

        coverBox.setSize(sf::Vector2f(250, 350));
        coverBox.setPosition(230, 120);
        coverBox.setFillColor(sf::Color(60, 100, 180));

        titleText.setFont(font); titleText.setCharacterSize(24); titleText.setFillColor(sf::Color(255, 193, 94));
        titleText.setPosition(520, 120);

        infoLabels.setFont(font); infoLabels.setCharacterSize(15); infoLabels.setFillColor(sf::Color(150, 150, 150));
        infoLabels.setPosition(520, 170);

        infoValues.setFont(font); infoValues.setCharacterSize(15); infoValues.setFillColor(sf::Color::White);
        infoValues.setPosition(650, 170);
        
        borrowerTitle.setFont(font); borrowerTitle.setCharacterSize(18); borrowerTitle.setFillColor(sf::Color(150, 150, 255));
        borrowerTitle.setPosition(520, 380);
        
        notBorrowedText.setFont(font); notBorrowedText.setCharacterSize(14); notBorrowedText.setFillColor(sf::Color(100, 100, 100));
        notBorrowedText.setPosition(520, 420); notBorrowedText.setString("Hien khong co doc gia nao dang muon cuon sach nay.");


        closeButton = new Button(sf::Vector2f(1000, 680), sf::Vector2f(150, 50),
                                "Dong", font, 0, sf::Color(100, 100, 120));
        
        // Cần đảm bảo các đối tượng Text trong BorrowerItem được khởi tạo với font
        const sf::Font& btnFont = closeButton->getFont();
        BorrowerItem tempItem; // Dùng tạm để khởi tạo font cho struct
        tempItem.nameText.setFont(btnFont);
        tempItem.dateText.setFont(btnFont);
        tempItem.statusText.setFont(btnFont);

        userRole = UserRole::NONE;
    }

    ~BookDetailScreen() { delete closeButton; }

    // FIX: Thêm LibrarySystem* vào tham số
    void setBook(Sach* book, UserRole role, LibrarySystem* libSystem) { 
        currentBook = book;
        userRole = role;
        borrowerItems.clear();

        // 1. Logic tải ảnh bìa (giữ nguyên)
        std::vector<sf::Color> colors = {sf::Color(200, 80, 60), sf::Color(60, 140, 200), sf::Color(100, 180, 100), sf::Color(200, 150, 60), sf::Color(140, 80, 180)};
        bookCoverTexture = sf::Texture(); 
        if (!book->getImagePath().empty()) {
            if (bookCoverTexture.loadFromFile(book->getImagePath())) {
                bookCoverSprite.setTexture(bookCoverTexture);
                float scaleX = coverBox.getSize().x / bookCoverTexture.getSize().x;
                float scaleY = coverBox.getSize().y / bookCoverTexture.getSize().y;
                bookCoverSprite.setScale(scaleX, scaleY);
                bookCoverSprite.setPosition(coverBox.getPosition());
                coverBox.setFillColor(sf::Color::Transparent);
            } else {
                coverBox.setFillColor(colors[rand() % 5]); 
            }
        } else {
            coverBox.setFillColor(colors[rand() % 5]);
        }


        // 2. Cập nhật thông tin sách cơ bản (giữ nguyên)
        std::string title = book->getTenSach();
        if (title.length() > 30) title = title.substr(0, 30) + "...";
        titleText.setString(title);
        
        std::string labels = "Ma sach:\nTac gia:\nThe loai:\nNam XB:\nNha XB:\nSo luong:\nDanh gia:";
        infoLabels.setString(labels);
        char ratingStr[50];
        sprintf(ratingStr, "%.1f/10 (%d luot)", book->getDiemTrungBinh(), book->getSoDanhGia());
        std::string values = book->getMaSach() + "\n" + book->getTacGia() + "\n" + book->getTheLoai() + "\n" +
                             std::to_string(book->getNamXuatBan()) + "\n" + book->getNhaXuatBan() + "\n" +
                             std::to_string(book->getSoLuong()) + " cuon\n" + std::string(ratingStr);
        infoValues.setString(values);


        // 3. Logic liệt kê người đang mượn (Dùng dữ liệu thực tế)
        if (role == UserRole::LIBRARIAN && libSystem != nullptr) {
             // NOTE: TimNguoiMuonSach cần được định nghĩa trong LibrarySystem.cpp
             std::vector<BorrowerInfo> results = libSystem->TimNguoiMuonSach(book->getMaSach());
             
             borrowerTitle.setString("Nguoi Dang Muon Sach (" + std::to_string(results.size()) + "):");
             
             // 4. Xử lý kết quả tra cứu và hiển thị
             for (const auto& result : results) {
                 BorrowerItem item;
                 const sf::Font& btnFont = closeButton->getFont();

                 // Set Font cho các Text trong item
                 item.nameText.setFont(btnFont); 
                 item.dateText.setFont(btnFont);
                 item.statusText.setFont(btnFont);
                 
                 item.nameText.setString(result.maDocGia + " - " + result.tenDocGia);
                 item.dateText.setString("Muon: " + timeString(result.ngayMuon) + " | Han: " + timeString(result.ngayHetHan));
                 item.statusText.setString(result.trangThai);
                 item.statusColor = result.daQuaHan ? sf::Color(255, 100, 100) : sf::Color(100, 180, 100);
                 
                 borrowerItems.push_back(item);
             }
        }
        
        // 5. Căn chỉnh vị trí các item người mượn
        float itemY = 420;
        const float TEXT_SIZE = 14;
        for (auto& item : borrowerItems) {
            item.nameText.setCharacterSize(TEXT_SIZE); item.nameText.setPosition(530, itemY); item.nameText.setFillColor(sf::Color::White);
            item.dateText.setCharacterSize(12); item.dateText.setPosition(530, itemY + 20); item.dateText.setFillColor(sf::Color(150, 150, 150));
            item.statusText.setCharacterSize(TEXT_SIZE); item.statusText.setPosition(800, itemY + 10); item.statusText.setFillColor(item.statusColor);
            itemY += 50;
        }
    }

    void update(sf::Vector2f mousePos) {
        closeButton->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (closeButton->handleClick(mousePos)) { return 0; } // Đóng Modal
        return -1;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(detailBox);
            window.draw(coverBox);
            
            if (bookCoverTexture.getSize().x > 0) { window.draw(bookCoverSprite); }
            
            window.draw(titleText);
            window.draw(infoLabels);
            window.draw(infoValues);
            
            if (userRole == UserRole::LIBRARIAN) {
                window.draw(borrowerTitle);
                
                if (borrowerItems.empty()) {
                    window.draw(notBorrowedText);
                } else {
                    for (const auto& item : borrowerItems) {
                        window.draw(item.nameText);
                        window.draw(item.dateText);
                        window.draw(item.statusText);
                    }
                }
            }
            
            closeButton->draw(window);
        }
    }

    Sach* getCurrentBook() { return currentBook; }
};

#endif