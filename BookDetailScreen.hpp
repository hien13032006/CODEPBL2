#ifndef BOOK_DETAIL_SCREEN_HPP
#define BOOK_DETAIL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iomanip>
#include <algorithm> // Cần cho std::min
#include "Modal.hpp"
#include "Button.hpp"
#include "Book.h"
#include "Theme.hpp"
#include "LibrarySystem.h"
#include "ScrollView.hpp"
#include "Node.h" 

struct BorrowerItem { sf::Text nameText, dateText, statusText; sf::Color statusColor; };

class BookDetailScreen {
private:
    Modal* modal; 
    sf::RectangleShape detailPanel, coverBox; 
    sf::Texture bookCoverTexture; 
    sf::Sprite bookCoverSprite;   
    sf::Text titleText, infoLabels, infoValues, borrowerTitle, notBorrowedText;
    sf::Text headerName, headerDate, headerStatus; 
    
    std::vector<BorrowerItem> borrowerItems; 
    Button *closeButton, *borrowButton, *btnDelete, *btnEdit;
    Sach* currentBook; UserRole userRole;

    ScrollView* scrollView;
    
    const float LIST_START_X = 530.0f;
    const float LIST_START_Y = 460.0f; 
    const float LIST_VISIBLE_W = 600.0f; 
    const float LIST_VISIBLE_H = 140.0f; 

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // Panel chính
        detailPanel.setSize(sf::Vector2f(900, 650)); 
        detailPanel.setFillColor(sf::Color::White); 
        detailPanel.setOutlineThickness(2); 
        detailPanel.setOutlineColor(Theme::Primary); 
        detailPanel.setPosition(200, 35); 

        // Khung ảnh bìa (Kích thước cố định 240x360)
        coverBox.setSize(sf::Vector2f(240, 360)); 
        coverBox.setPosition(250, 70); 
        coverBox.setFillColor(sf::Color(200, 200, 200)); // Màu xám mặc định khi chưa có ảnh

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
        
        borrowerTitle.setFont(font); 
        borrowerTitle.setCharacterSize(20); 
        borrowerTitle.setFillColor(Theme::Primary); 
        borrowerTitle.setStyle(sf::Text::Bold);
        borrowerTitle.setPosition(530, 400);
        
        notBorrowedText.setFont(font); notBorrowedText.setCharacterSize(16); 
        notBorrowedText.setFillColor(Theme::TextLight); 
        notBorrowedText.setPosition(530, 440); 
        notBorrowedText.setString("(Hien chua co ai muon)");

        auto setupHeader = [&](sf::Text& t, string s, float x) {
            t.setFont(font); t.setString(s); t.setCharacterSize(14); 
            t.setFillColor(sf::Color(150, 150, 150)); t.setStyle(sf::Text::Bold);
            t.setPosition(x, 430); 
        };
        setupHeader(headerName, "DOC GIA", 530);
        setupHeader(headerDate, "HAN TRA", 780);
        setupHeader(headerStatus, "TRANG THAI", 920);

        closeButton = new Button(sf::Vector2f(930, 620), sf::Vector2f(120, 45), "Dong", font, 0, sf::Color(150, 150, 150));
        borrowButton = new Button(sf::Vector2f(780, 620), sf::Vector2f(120, 45), "Muon Sach", font, 1, Theme::Success);
        btnDelete = new Button(sf::Vector2f(630, 620), sf::Vector2f(120, 45), "Xoa", font, 2, Theme::Error);
        btnEdit = new Button(sf::Vector2f(480, 620), sf::Vector2f(120, 45), "Cap Nhat", font, 3, Theme::Secondary);
        userRole = UserRole::NONE;

        scrollView = new ScrollView(sf::FloatRect(LIST_START_X, LIST_START_Y, LIST_VISIBLE_W, LIST_VISIBLE_H));
    }

    ~BookDetailScreen() { 
        delete closeButton; delete borrowButton; delete btnDelete; delete btnEdit; 
        delete scrollView; 
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (modal && modal->isShown() && userRole == UserRole::LIBRARIAN) {
            sf::FloatRect eventArea(200, 400, 900, 250); 
            if(eventArea.contains(mousePos)) {
                sf::Vector2f fakePos(LIST_START_X + 10, LIST_START_Y + 10);
                scrollView->handleScroll(event, fakePos);
            }
        }
    }

// [HÀM setBook ĐÃ SỬA LỖI ĐÈ CHỮ]
    void setBook(Sach* book, UserRole role, LibrarySystem* libSystem) { 
        currentBook = book; 
        userRole = role; 
        borrowerItems.clear();
        
        // ---------------------------------------------------------
        // 1. XỬ LÝ ẢNH BÌA (Giữ nguyên)
        // ---------------------------------------------------------
        bookCoverTexture = sf::Texture(); 
        bookCoverSprite.setScale(1, 1);
        bookCoverSprite.setOrigin(0, 0);
        bookCoverSprite.setPosition(coverBox.getPosition());

        float targetW = 240.0f; 
        float targetH = 360.0f; 

        if (!book->getImagePath().empty() && bookCoverTexture.loadFromFile(book->getImagePath())) { 
            bookCoverTexture.setSmooth(true);
            bookCoverSprite.setTexture(bookCoverTexture, true);
            sf::Vector2u imgSize = bookCoverTexture.getSize();
            float scaleX = targetW / (float)imgSize.x;
            float scaleY = targetH / (float)imgSize.y;
            float scale = std::min(scaleX, scaleY); 
            bookCoverSprite.setScale(scale, scale);
            float finalW = imgSize.x * scale;
            float finalH = imgSize.y * scale;
            float offsetX = (targetW - finalW) / 2.0f;
            float offsetY = (targetH - finalH) / 2.0f;
            bookCoverSprite.setPosition(coverBox.getPosition().x + offsetX, coverBox.getPosition().y + offsetY);
            coverBox.setFillColor(sf::Color::White); 
        } else { 
            coverBox.setFillColor(sf::Color(200, 200, 200)); 
        }
        // A. Tên sách (Xuống dòng nếu dài)
        std::string title = book->getTenSach(); 
        if (title.length() > 28) {
            size_t splitPos = title.find_last_of(' ', 28);
            if (splitPos != std::string::npos) title.insert(splitPos + 1, "\n");
        }
        titleText.setString(title);

        // B. Tính vị trí bắt đầu của Thông tin chi tiết
        sf::FloatRect titleBounds = titleText.getGlobalBounds(); 
        float infoY = titleBounds.top + titleBounds.height + 25.0f;
        
        // Set vị trí cho 2 cột thông tin
        infoLabels.setPosition(530, infoY);
        infoValues.setPosition(650, infoY);

        // C. Điền dữ liệu vào trước để tính chiều cao thực tế
        infoLabels.setString("Ma sach:\nTac gia:\nThe loai:\nNam XB:\nNha XB:\nSo luong:\nDanh gia:");
        
        char ratingStr[50]; 
        sprintf(ratingStr, "%.1f/10 (%d luot)", book->getDiemTrungBinh() > 10 ? 10 : book->getDiemTrungBinh(), book->getSoDanhGia());
        
        std::string values = book->getMaSach() + "\n" + 
                             book->getTacGia() + "\n" + 
                             book->getTheLoai() + "\n" + 
                             std::to_string(book->getNamXuatBan()) + "\n" + 
                             book->getNhaXuatBan() + "\n" + 
                             std::to_string(book->getSoLuong()) + " cuon\n" + 
                             std::string(ratingStr);
        infoValues.setString(values);

        // [QUAN TRỌNG] Tính vị trí phần "Người Đang Mượn" dựa trên đáy của phần Thông tin
        sf::FloatRect infoBounds = infoValues.getGlobalBounds();
        
        // Vị trí tiêu đề "Người Đang Mượn" = Đáy thông tin + 30px đệm
        float borrowerTitleY = infoBounds.top + infoBounds.height + 30.0f;
        borrowerTitle.setPosition(530, borrowerTitleY);
        
        // Vị trí dòng Header bảng (Doc gia, Han tra...) = Tiêu đề + 30px
        float headerY = borrowerTitleY + 30.0f;
        
        // Cập nhật vị trí các Header
        headerName.setPosition(530, headerY);
        headerDate.setPosition(780, headerY);
        headerStatus.setPosition(920, headerY);
        notBorrowedText.setPosition(530, headerY); // Dòng "Chưa có ai mượn" cũng nằm đây

        // Vị trí bắt đầu danh sách item = Header + 30px
        float listStartY = headerY + 30.0f;
        if (role == UserRole::LIBRARIAN && libSystem) {
             NodeBorrowerInfo* headList = libSystem->TimNguoiMuonSach(book->getMaSach());
             
             int count = 0;
             NodeBorrowerInfo* p = headList;
             while(p != nullptr) { count++; p = p->next; }
             
             borrowerTitle.setString("Nguoi Dang Muon (" + std::to_string(count) + "):"); 
             
             float currentItemY = listStartY; // Sử dụng vị trí động đã tính
             const float TEXT_SIZE = 16;
             const float ROW_H = 35.0f;
             
             p = headList;
             while (p != nullptr) {
                 const BorrowerInfo& res = p->data; 
                 BorrowerItem item; 
                 const sf::Font& f = closeButton->getFont();
                 
                 item.nameText.setFont(f); item.dateText.setFont(f); item.statusText.setFont(f);
                 
                 std::string nameStr = res.maDocGia + " - " + res.tenDocGia;
                 if (nameStr.length() > 22) nameStr = nameStr.substr(0, 19) + "...";
                 
                 item.nameText.setString(nameStr);
                 item.dateText.setString(timeString(res.ngayHetHan)); 
                 item.statusText.setString(res.daQuaHan ? "QUA HAN" : "OK");
                 item.statusColor = res.daQuaHan ? Theme::Error : Theme::Success;
                 
                 // Set vị trí dựa trên currentItemY động
                 item.nameText.setCharacterSize(TEXT_SIZE); 
                 item.nameText.setPosition(530, currentItemY); 
                 item.nameText.setFillColor(Theme::TextDark);
                 
                 item.dateText.setCharacterSize(TEXT_SIZE); 
                 item.dateText.setPosition(780, currentItemY); 
                 item.dateText.setFillColor(Theme::TextLight);
                 
                 item.statusText.setCharacterSize(TEXT_SIZE); 
                 item.statusText.setPosition(920, currentItemY); 
                 item.statusText.setFillColor(item.statusColor);
                 
                 borrowerItems.push_back(item); 
                 currentItemY += ROW_H; 
                 p = p->next; 
             }

             while (headList != nullptr) {
                 NodeBorrowerInfo* temp = headList;
                 headList = headList->next;
                 delete temp;
             }
             float visibleH = 650.0f - (listStartY - 35.0f) - 60.0f; 
             if (visibleH < 100) visibleH = 100; // Tối thiểu

             float totalContentH = borrowerItems.size() * ROW_H;
             scrollView->setMaxScroll(std::max(0.0f, totalContentH - visibleH)); 
             scrollView->reset();
        }
    }
 void update(sf::Vector2f mousePos) { 
        closeButton->update(mousePos); 
        if (userRole == UserRole::LIBRARIAN) { btnDelete->update(mousePos); btnEdit->update(mousePos); } 
    }

    int handleClick(sf::Vector2f mousePos) { 
        if (closeButton->handleClick(mousePos)) return 0; 
        if (userRole == UserRole::LIBRARIAN) { 
            if (btnDelete->handleClick(mousePos)) return 2; 
            if (btnEdit->handleClick(mousePos)) return 3; 
        } 
        return -1; 
    }
    
    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            modal->draw(window); 
            window.draw(detailPanel); 
            
            // Vẽ hộp nền trước
            window.draw(coverBox); 
            
            // Nếu có texture hợp lệ thì vẽ sprite lên trên
            if (bookCoverTexture.getSize().x > 0) {
                window.draw(bookCoverSprite);
            }

            window.draw(titleText); window.draw(infoLabels); window.draw(infoValues);
            
            if (userRole == UserRole::LIBRARIAN) { 
                window.draw(borrowerTitle); 
                if (borrowerItems.empty()) {
                    window.draw(notBorrowedText); 
                } else { 
                    window.draw(headerName); window.draw(headerDate); window.draw(headerStatus);
                    
                    sf::View listView = window.getDefaultView();
                    float vpX = LIST_START_X / 1300.0f;
                    float vpY = LIST_START_Y / 720.0f;
                    float vpW = LIST_VISIBLE_W / 1300.0f;
                    float vpH = LIST_VISIBLE_H / 720.0f;

                    listView.setViewport(sf::FloatRect(vpX, vpY, vpW, vpH));
                    listView.setSize(LIST_VISIBLE_W, LIST_VISIBLE_H);
                    listView.setCenter(LIST_START_X + LIST_VISIBLE_W/2.0f, LIST_START_Y + LIST_VISIBLE_H/2.0f + scrollView->getScrollOffset());
                    
                    window.setView(listView);
                    for (const auto& item : borrowerItems) { 
                        window.draw(item.nameText); window.draw(item.dateText); window.draw(item.statusText); 
                    } 
                    window.setView(window.getDefaultView()); 
                }
                btnDelete->draw(window); btnEdit->draw(window); 
            }
            closeButton->draw(window);
        }
    }
    Sach* getCurrentBook() { return currentBook; }
};

#endif