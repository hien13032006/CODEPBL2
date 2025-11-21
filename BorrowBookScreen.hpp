#ifndef BORROW_BOOK_SCREEN_HPP
#define BORROW_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Button.hpp"
#include "Modal.hpp"
#include "Book.h"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"

class BorrowBookScreen {
private:
    Modal* modal;
    RoundedRectangleShape panel;
    sf::Text titleText;
    sf::Text bookInfoText;
    sf::Text borrowMsg;

    InputField* maSachField;
    Button* borrowButton;
    Button* cancelButton;

    sf::Text errMaSach;
    sf::Text borrowedInfoText;
    Sach* currentBook;

    // Animation rung lắc khi lỗi
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    BorrowBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // 1. Panel Chính (Size 700x550)
        panel.setSize(sf::Vector2f(700, 550));
        panel.setCornerRadius(15.0f);
        panel.setFillColor(sf::Color(40, 45, 60)); // Nền tối
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(100, 100, 120));
        panel.setPosition(350, 175); // Canh giữa màn hình

        // 2. Title
        titleText.setFont(font);
        titleText.setString("XAC NHAN MUON SACH");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(panel.getPosition().x + 180, panel.getPosition().y + 30);

        // 3. Thông tin sách (Size 20)
        bookInfoText.setFont(font);
        bookInfoText.setCharacterSize(20);
        bookInfoText.setFillColor(sf::Color(220, 220, 220));
        bookInfoText.setPosition(panel.getPosition().x + 40, panel.getPosition().y + 100);
        bookInfoText.setLineSpacing(1.4f);

        // 4. Input (Size lớn)
        maSachField = new InputField(
            {panel.getPosition().x + 40, panel.getPosition().y + 220},
            {620, 55}, 
            "Nhap ma sach...", font
        );

        // 5. Thông báo lỗi
        errMaSach.setFont(font);
        errMaSach.setCharacterSize(16);
        errMaSach.setFillColor(sf::Color(255, 80, 80));
        errMaSach.setPosition(panel.getPosition().x + 40, panel.getPosition().y + 285);

        // 6. Thông báo thành công
        borrowMsg.setFont(font);
        borrowMsg.setCharacterSize(18);
        borrowMsg.setFillColor(sf::Color(100, 255, 100)); // Xanh lá
        borrowMsg.setPosition(panel.getPosition().x + 40, panel.getPosition().y + 315);

        borrowedInfoText.setFont(font);
        borrowedInfoText.setCharacterSize(18);
        borrowedInfoText.setFillColor(sf::Color::White);
        borrowedInfoText.setPosition(panel.getPosition().x + 40, panel.getPosition().y + 350);

        // 7. Buttons
        borrowButton = new Button(
            {panel.getPosition().x + 80, panel.getPosition().y + 450},
            {200, 55}, "Xac Nhan", font, 0, sf::Color(76, 175, 80)
        );

        cancelButton = new Button(
            {panel.getPosition().x + 420, panel.getPosition().y + 450},
            {200, 55}, "Huy", font, 0, sf::Color(180, 80, 80)
        );
    }

    ~BorrowBookScreen() {
        delete borrowButton;
        delete cancelButton;
        delete maSachField;
    }

    void setCurrentBook(Sach* book) {
        currentBook = book;
        if (currentBook) {
            // Tự động điền mã sách nếu chọn từ màn hình chi tiết
            maSachField->setText(currentBook->getMaSach());
            
            std::string info = "Ten sach:  " + currentBook->getTenSach() + 
                               "\nTac gia:   " + currentBook->getTacGia() +
                               "\nCon lai:   " + std::to_string(currentBook->getSoLuong()) + " cuon";
            bookInfoText.setString(info);
        } else {
            maSachField->clear();
            bookInfoText.setString("Vui long nhap ma sach de muon.");
        }
    }

    std::string getMaSachInput() const { return maSachField->getText(); }
    void setBorrowMessage(const std::string& msg) { borrowMsg.setString(msg); }
    
    void startShake() { shaking = true; shakeTime = 0; }

    bool validate() {
        errMaSach.setString("");
        if (getMaSachInput().empty()) {
            errMaSach.setString("Vui long nhap ma sach!");
            startShake();
            return false;
        }
        return true;
    }

    void update(sf::Vector2f mousePos) {
        maSachField->update();
        borrowButton->update(mousePos);
        cancelButton->update(mousePos);

        // Hiệu ứng rung khi lỗi
        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 50) * 10;
            panel.setPosition(350 + offset, 175);
            if (shakeTime >= shakeDuration) {
                shaking = false;
                panel.setPosition(350, 175);
            }
        }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        maSachField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (borrowButton->handleClick(mousePos)) return 1; // Mượn
        if (cancelButton->handleClick(mousePos)) return 2; // Hủy
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(panel);
            window.draw(titleText);
            window.draw(bookInfoText);
            maSachField->draw(window);
            window.draw(errMaSach);
            window.draw(borrowMsg);
            window.draw(borrowedInfoText);
            borrowButton->draw(window);
            cancelButton->draw(window);
        }
    }
    
    void clearInput() { 
        // Không clear mã sách ở đây vì cần giữ lại để hiển thị
        errMaSach.setString(""); 
        borrowedInfoText.setString(""); 
    }
    
    void clearMessage() { borrowMsg.setString(""); }
};

#endif