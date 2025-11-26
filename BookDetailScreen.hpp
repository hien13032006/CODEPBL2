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
#include "ScrollView.hpp"

struct BorrowerItem { sf::Text nameText, dateText, statusText; sf::Color statusColor; };

class BookDetailScreen {
private:
    Modal* modal; sf::RectangleShape detailPanel, coverBox; sf::Texture bookCoverTexture; sf::Sprite bookCoverSprite;   
    sf::Text titleText, infoLabels, infoValues, borrowerTitle, notBorrowedText;
    sf::Text headerName, headerDate, headerStatus; 
    
    std::vector<BorrowerItem> borrowerItems; 
    Button *closeButton, *borrowButton, *btnDelete, *btnEdit;
    Sach* currentBook; UserRole userRole;

    ScrollView* scrollView;
    
    // --- CẤU HÌNH VÙNG DANH SÁCH ---
    // Vị trí bắt đầu vẽ danh sách (Dưới header một chút)
    const float LIST_START_X = 530.0f;
    const float LIST_START_Y = 460.0f; 
    
    // Kích thước vùng nhìn thấy được (Viewport)
    const float LIST_VISIBLE_W = 600.0f; // Rộng
    const float LIST_VISIBLE_H = 140.0f; // Cao (Chỉ cho phép cao 140px để không đè nút)

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // Panel chính
        detailPanel.setSize(sf::Vector2f(900, 650)); 
        detailPanel.setFillColor(sf::Color::White); 
        detailPanel.setOutlineThickness(2); 
        detailPanel.setOutlineColor(Theme::Primary); 
        detailPanel.setPosition(200, 35); 

        // Ảnh bìa
        coverBox.setSize(sf::Vector2f(240, 360)); 
        coverBox.setPosition(250, 70); 
        coverBox.setFillColor(sf::Color(200, 200, 200));

        // Tên sách
        titleText.setFont(font); titleText.setCharacterSize(32); 
        titleText.setFillColor(Theme::TextDark); 
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(530, 70);

        // Thông tin chi tiết
        infoLabels.setFont(font); infoLabels.setCharacterSize(18); 
        infoLabels.setFillColor(Theme::TextLight); 
        infoLabels.setPosition(530, 130); 
        infoLabels.setLineSpacing(1.6f); 

        infoValues.setFont(font); infoValues.setCharacterSize(18); 
        infoValues.setFillColor(Theme::TextDark); 
        infoValues.setPosition(650, 130); 
        infoValues.setLineSpacing(1.6f);
        
        // Tiêu đề "Người Đang Mượn"
        borrowerTitle.setFont(font); 
        borrowerTitle.setCharacterSize(20); 
        borrowerTitle.setFillColor(Theme::Primary); 
        borrowerTitle.setStyle(sf::Text::Bold);
        borrowerTitle.setPosition(530, 400);
        
        notBorrowedText.setFont(font); notBorrowedText.setCharacterSize(16); 
        notBorrowedText.setFillColor(Theme::TextLight); 
        notBorrowedText.setPosition(530, 440); 
        notBorrowedText.setString("(Hien chua co ai muon)");

        // Headers bảng (Cố định, không trượt)
        auto setupHeader = [&](sf::Text& t, string s, float x) {
            t.setFont(font); t.setString(s); t.setCharacterSize(14); 
            t.setFillColor(sf::Color(150, 150, 150)); t.setStyle(sf::Text::Bold);
            t.setPosition(x, 430); 
        };
        setupHeader(headerName, "DOC GIA", 530);
        setupHeader(headerDate, "HAN TRA", 780);
        setupHeader(headerStatus, "TRANG THAI", 920);

        // Các nút chức năng (Nằm ở đáy Y=620)
        closeButton = new Button(sf::Vector2f(930, 620), sf::Vector2f(120, 45), "Dong", font, 0, sf::Color(150, 150, 150));
        borrowButton = new Button(sf::Vector2f(780, 620), sf::Vector2f(120, 45), "Muon Sach", font, 1, Theme::Success);
        btnDelete = new Button(sf::Vector2f(630, 620), sf::Vector2f(120, 45), "Xoa", font, 2, Theme::Error);
        btnEdit = new Button(sf::Vector2f(480, 620), sf::Vector2f(120, 45), "Cap Nhat", font, 3, Theme::Secondary);
        userRole = UserRole::NONE;

        // Init ScrollView với kích thước vùng nhìn thấy
        scrollView = new ScrollView(sf::FloatRect(LIST_START_X, LIST_START_Y, LIST_VISIBLE_W, LIST_VISIBLE_H));
    }

    ~BookDetailScreen() { 
        delete closeButton; delete borrowButton; delete btnDelete; delete btnEdit; 
        delete scrollView; 
    }

    // Xử lý sự kiện lăn chuột
    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (modal && modal->isShown() && userRole == UserRole::LIBRARIAN) {
            // Cho phép lăn chuột ngay cả khi chuột ở trên vùng info một chút
            // Mở rộng vùng bắt sự kiện Y từ 400 đến 600
            sf::FloatRect eventArea(200, 400, 900, 250); 
            if(eventArea.contains(mousePos)) {
                // Fake mousePos để ép ScrollView nhận sự kiện (vì ScrollView check bound strict quá)
                sf::Vector2f fakePos(LIST_START_X + 10, LIST_START_Y + 10);
                scrollView->handleScroll(event, fakePos);
            }
        }
    }

    void setBook(Sach* book, UserRole role, LibrarySystem* libSystem) { 
        currentBook = book; userRole = role; borrowerItems.clear();
        
        bookCoverTexture = sf::Texture(); 
        if (!book->getImagePath().empty() && bookCoverTexture.loadFromFile(book->getImagePath())) { 
            bookCoverSprite.setTexture(bookCoverTexture); 
            float scaleX = coverBox.getSize().x / bookCoverTexture.getSize().x;
            float scaleY = coverBox.getSize().y / bookCoverTexture.getSize().y;
            bookCoverSprite.setScale(scaleX, scaleY); bookCoverSprite.setPosition(coverBox.getPosition()); coverBox.setFillColor(sf::Color::White); 
        } else { coverBox.setFillColor(Theme::Secondary); }

        std::string title = book->getTenSach(); 
        if (title.length() > 30) title = title.substr(0, 27) + "..."; 
        titleText.setString(title);
        
        infoLabels.setString("Ma sach:\nTac gia:\nThe loai:\nNam XB:\nNha XB:\nSo luong:\nDanh gia:");
        char ratingStr[50]; sprintf(ratingStr, "%.1f/10 (%d luot)", book->getDiemTrungBinh() > 10 ? 10 : book->getDiemTrungBinh(), book->getSoDanhGia());
        std::string values = book->getMaSach() + "\n" + book->getTacGia() + "\n" + book->getTheLoai() + "\n" + std::to_string(book->getNamXuatBan()) + "\n" + book->getNhaXuatBan() + "\n" + std::to_string(book->getSoLuong()) + " cuon\n" + std::string(ratingStr);
        infoValues.setString(values);

        if (role == UserRole::LIBRARIAN && libSystem) {
             std::vector<BorrowerInfo> results = libSystem->TimNguoiMuonSach(book->getMaSach());
             borrowerTitle.setString("Nguoi Dang Muon (" + std::to_string(results.size()) + "):"); 
             
             float itemY = LIST_START_Y; 
             const float TEXT_SIZE = 16;
             const float ROW_H = 35.0f; // Tăng chiều cao dòng cho thoáng
             
             for (const auto& res : results) {
                 BorrowerItem item; const sf::Font& f = closeButton->getFont();
                 item.nameText.setFont(f); item.dateText.setFont(f); item.statusText.setFont(f);
                 
                 std::string nameStr = res.maDocGia + " - " + res.tenDocGia;
                 if (nameStr.length() > 22) nameStr = nameStr.substr(0, 19) + "...";
                 item.nameText.setString(nameStr);
                 item.dateText.setString(timeString(res.ngayHetHan)); 
                 item.statusText.setString(res.daQuaHan ? "QUA HAN" : "OK");
                 item.statusColor = res.daQuaHan ? Theme::Error : Theme::Success;
                 
                 item.nameText.setCharacterSize(TEXT_SIZE); item.nameText.setPosition(530, itemY); item.nameText.setFillColor(Theme::TextDark);
                 item.dateText.setCharacterSize(TEXT_SIZE); item.dateText.setPosition(780, itemY); item.dateText.setFillColor(Theme::TextLight);
                 item.statusText.setCharacterSize(TEXT_SIZE); item.statusText.setPosition(920, itemY); item.statusText.setFillColor(item.statusColor);
                 
                 borrowerItems.push_back(item); 
                 itemY += ROW_H; 
             }

             // Tính toán scroll max, cộng thêm padding 200px để trượt dư
             float totalH = borrowerItems.size() * ROW_H;
             scrollView->setMaxScroll(std::max(0.0f, totalH - LIST_VISIBLE_H + 150.0f)); 
             scrollView->reset();
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
                    // Vẽ Header cố định
                    window.draw(headerName); window.draw(headerDate); window.draw(headerStatus);
                    
                    // [QUAN TRỌNG] Vẽ danh sách trong Viewport cắt (Clipping)
                    sf::View listView = window.getDefaultView();
                    // Tỉ lệ Viewport dựa trên màn hình 1300x720
                    float vpX = LIST_START_X / 1300.0f;
                    float vpY = LIST_START_Y / 720.0f;
                    float vpW = LIST_VISIBLE_W / 1300.0f;
                    float vpH = LIST_VISIBLE_H / 720.0f;

                    listView.setViewport(sf::FloatRect(vpX, vpY, vpW, vpH));
                    listView.setSize(LIST_VISIBLE_W, LIST_VISIBLE_H);
                    // Center di chuyển theo scroll
                    listView.setCenter(LIST_START_X + LIST_VISIBLE_W/2.0f, LIST_START_Y + LIST_VISIBLE_H/2.0f + scrollView->getScrollOffset());
                    
                    window.setView(listView);
                    for (const auto& item : borrowerItems) { 
                        window.draw(item.nameText); window.draw(item.dateText); window.draw(item.statusText); 
                    } 
                    window.setView(window.getDefaultView()); // Reset về view thường để vẽ nút
                }
                // Vẽ nút (Nằm ngoài vùng view list nên không bị che)
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