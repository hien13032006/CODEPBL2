#ifndef BOOK_DETAIL_SCREEN_HPP
#define BOOK_DETAIL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iomanip>
#include "Modal.hpp"
#include "Button.hpp"
#include "Book.h"
#include "Theme.hpp"
#include "LibrarySystem.h"

struct BorrowerItem { sf::Text nameText, dateText, statusText; sf::Color statusColor; };

class BookDetailScreen {
private:
    Modal* modal; sf::RectangleShape detailPanel, coverBox; sf::Texture bookCoverTexture; sf::Sprite bookCoverSprite;   
    sf::Text titleText, infoLabels, infoValues, borrowerTitle, notBorrowedText;
    // Thêm header cho danh sách mượn để rõ ràng hơn
    sf::Text headerName, headerDate, headerStatus; 
    
    std::vector<BorrowerItem> borrowerItems; 
    Button *closeButton, *borrowButton, *btnDelete, *btnEdit;
    Sach* currentBook; UserRole userRole;

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // Panel chính
        detailPanel.setSize(sf::Vector2f(900, 650)); // Tăng chiều cao lên xíu
        detailPanel.setFillColor(sf::Color::White); 
        detailPanel.setOutlineThickness(2); 
        detailPanel.setOutlineColor(Theme::Primary); 
        detailPanel.setPosition(200, 35); // Đẩy lên cao chút cho cân đối

        // Ảnh bìa
        coverBox.setSize(sf::Vector2f(240, 360)); // Ảnh to hơn chút
        coverBox.setPosition(250, 100); 
        coverBox.setFillColor(sf::Color(200, 200, 200));

        // Tên sách
        titleText.setFont(font); titleText.setCharacterSize(32); // Chữ to hơn
        titleText.setFillColor(Theme::TextDark); 
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(530, 100);

        // Labels (Ma sach, Tac gia...)
        infoLabels.setFont(font); infoLabels.setCharacterSize(18); 
        infoLabels.setFillColor(Theme::TextLight); 
        infoLabels.setPosition(530, 160); 
        infoLabels.setLineSpacing(1.8f); // Giãn dòng rộng ra (1.8)

        // Values (Gia tri)
        infoValues.setFont(font); infoValues.setCharacterSize(18); 
        infoValues.setFillColor(Theme::TextDark); 
        infoValues.setPosition(650, 160); // Cách label 120px
        infoValues.setLineSpacing(1.8f);
        
        // Tiêu đề danh sách mượn (Đẩy xuống Y=430 để tránh đè)
        borrowerTitle.setFont(font); 
        borrowerTitle.setCharacterSize(22); 
        borrowerTitle.setFillColor(Theme::Primary); 
        borrowerTitle.setStyle(sf::Text::Bold);
        borrowerTitle.setPosition(530, 430);
        
        notBorrowedText.setFont(font); notBorrowedText.setCharacterSize(16); 
        notBorrowedText.setFillColor(Theme::TextLight); 
        notBorrowedText.setPosition(530, 470); 
        notBorrowedText.setString("(Hien chua co ai muon)");

        // Headers bảng mượn (Dành cho Thủ thư)
        auto setupHeader = [&](sf::Text& t, string s, float x) {
            t.setFont(font); t.setString(s); t.setCharacterSize(14); 
            t.setFillColor(sf::Color(150, 150, 150)); t.setStyle(sf::Text::Bold);
            t.setPosition(x, 470); // Y=470
        };
        setupHeader(headerName, "DOC GIA", 530);
        setupHeader(headerDate, "HAN TRA", 800);
        setupHeader(headerStatus, "TRANG THAI", 950);

        // Buttons (Đẩy xuống đáy Y=620)
        closeButton = new Button(sf::Vector2f(930, 620), sf::Vector2f(120, 45), "Dong", font, 0, sf::Color(150, 150, 150));
        borrowButton = new Button(sf::Vector2f(780, 620), sf::Vector2f(120, 45), "Muon Sach", font, 1, Theme::Success);
        btnDelete = new Button(sf::Vector2f(630, 620), sf::Vector2f(120, 45), "Xoa", font, 2, Theme::Error);
        btnEdit = new Button(sf::Vector2f(480, 620), sf::Vector2f(120, 45), "Cap Nhat", font, 3, Theme::Secondary);
        userRole = UserRole::NONE;
    }
    ~BookDetailScreen() { delete closeButton; delete borrowButton; delete btnDelete; delete btnEdit; }

    void setBook(Sach* book, UserRole role, LibrarySystem* libSystem) { 
        currentBook = book; userRole = role; borrowerItems.clear();
        
        bookCoverTexture = sf::Texture(); 
        if (!book->getImagePath().empty() && bookCoverTexture.loadFromFile(book->getImagePath())) { 
            bookCoverSprite.setTexture(bookCoverTexture); 
            // Scale ảnh giữ tỉ lệ khung
            float scaleX = coverBox.getSize().x / bookCoverTexture.getSize().x;
            float scaleY = coverBox.getSize().y / bookCoverTexture.getSize().y;
            bookCoverSprite.setScale(scaleX, scaleY); 
            bookCoverSprite.setPosition(coverBox.getPosition()); 
            coverBox.setFillColor(sf::Color::White); // Xóa nền xám nếu có ảnh
        } else { coverBox.setFillColor(Theme::Secondary); }

        // Tên sách: Cắt ngắn nếu quá dài
        std::string title = book->getTenSach(); 
        if (title.length() > 30) title = title.substr(0, 27) + "..."; 
        titleText.setString(title);
        
        infoLabels.setString("Ma sach:\nTac gia:\nThe loai:\nNam XB:\nNha XB:\nSo luong:\nDanh gia:");
        char ratingStr[50]; sprintf(ratingStr, "%.1f/10 (%d luot)", book->getDiemTrungBinh(), book->getSoDanhGia());
        std::string values = book->getMaSach() + "\n" + book->getTacGia() + "\n" + book->getTheLoai() + "\n" + std::to_string(book->getNamXuatBan()) + "\n" + book->getNhaXuatBan() + "\n" + std::to_string(book->getSoLuong()) + " cuon\n" + std::string(ratingStr);
        infoValues.setString(values);

        if (role == UserRole::LIBRARIAN && libSystem) {
             std::vector<BorrowerInfo> results = libSystem->TimNguoiMuonSach(book->getMaSach());
             borrowerTitle.setString("Nguoi Dang Muon (" + std::to_string(results.size()) + "):"); 
             
             float itemY = 500; // Bắt đầu list từ Y=500
             const float TEXT_SIZE = 16;
             
             for (const auto& res : results) {
                 BorrowerItem item; const sf::Font& f = closeButton->getFont();
                 item.nameText.setFont(f); item.dateText.setFont(f); item.statusText.setFont(f);
                 
                 // Tên: Mã + Tên (Cắt ngắn nếu dài)
                 std::string nameStr = res.maDocGia + " - " + res.tenDocGia;
                 if (nameStr.length() > 25) nameStr = nameStr.substr(0, 22) + "...";
                 item.nameText.setString(nameStr);
                 
                 item.dateText.setString(timeString(res.ngayHetHan)); 
                 item.statusText.setString(res.daQuaHan ? "QUA HAN" : "OK");
                 item.statusColor = res.daQuaHan ? Theme::Error : Theme::Success;
                 
                 // Căn chỉnh tọa độ cột
                 item.nameText.setCharacterSize(TEXT_SIZE); item.nameText.setPosition(530, itemY); item.nameText.setFillColor(Theme::TextDark);
                 item.dateText.setCharacterSize(TEXT_SIZE); item.dateText.setPosition(800, itemY); item.dateText.setFillColor(Theme::TextLight);
                 item.statusText.setCharacterSize(TEXT_SIZE); item.statusText.setPosition(950, itemY); item.statusText.setFillColor(item.statusColor);
                 
                 borrowerItems.push_back(item); itemY += 30; // Giãn dòng item 30px
             }
        }
    }
    void update(sf::Vector2f mousePos) { closeButton->update(mousePos); if (userRole == UserRole::READER) { borrowButton->update(mousePos); } else if (userRole == UserRole::LIBRARIAN) { btnDelete->update(mousePos); btnEdit->update(mousePos); } }
    int handleClick(sf::Vector2f mousePos) { if (closeButton->handleClick(mousePos)) return 0; if (userRole == UserRole::READER) { if (borrowButton->handleClick(mousePos)) return 1; } else if (userRole == UserRole::LIBRARIAN) { if (btnDelete->handleClick(mousePos)) return 2; if (btnEdit->handleClick(mousePos)) return 3; } return -1; }
    
    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            modal->draw(window); 
            window.draw(detailPanel); 
            window.draw(coverBox); if (bookCoverTexture.getSize().x > 0) window.draw(bookCoverSprite);
            window.draw(titleText); window.draw(infoLabels); window.draw(infoValues);
            
            if (userRole == UserRole::LIBRARIAN) { 
                window.draw(borrowerTitle); 
                if (borrowerItems.empty()) {
                    window.draw(notBorrowedText); 
                } else { 
                    // Vẽ headers
                    window.draw(headerName); window.draw(headerDate); window.draw(headerStatus);
                    // Vẽ list items
                    for (const auto& item : borrowerItems) { 
                        window.draw(item.nameText); window.draw(item.dateText); window.draw(item.statusText); 
                    } 
                }
                btnDelete->draw(window); btnEdit->draw(window); 
            } else if (userRole == UserRole::READER) { 
                borrowButton->draw(window); 
            }
            closeButton->draw(window);
        }
    }
    Sach* getCurrentBook() { return currentBook; }
};
#endif