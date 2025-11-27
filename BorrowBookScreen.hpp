#ifndef BORROW_BOOK_SCREEN_HPP
#define BORROW_BOOK_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Button.hpp"
#include "Modal.hpp"
#include "Book.h"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class BorrowBookScreen {
private:
    Modal* modal; RoundedRectangleShape panel; sf::Text titleText, bookInfoText, borrowMsg, errMaSach; InputField* maSachField; Button *borrowButton, *cancelButton; Sach* currentBook; bool shaking = false; float shakeTime = 0.0f;
public:
    BorrowBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        panel.setSize(sf::Vector2f(700, 500)); panel.setCornerRadius(15.0f); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(300, 110);
        titleText.setFont(font); titleText.setString("XAC NHAN MUON SACH"); titleText.setCharacterSize(32); titleText.setFillColor(Theme::Primary); titleText.setPosition(480, 140);
        bookInfoText.setFont(font); bookInfoText.setCharacterSize(20); bookInfoText.setFillColor(Theme::TextDark); bookInfoText.setPosition(340, 200); bookInfoText.setLineSpacing(1.4f);
        maSachField = new InputField({340, 320}, {620, 55}, "Nhap ma sach...", font);
        errMaSach.setFont(font); errMaSach.setCharacterSize(16); errMaSach.setFillColor(Theme::Error); errMaSach.setPosition(340, 385);
        
        // borrowMsg dùng chung cho cả lỗi và thành công
        borrowMsg.setFont(font); borrowMsg.setCharacterSize(18); borrowMsg.setPosition(340, 415);
        
        borrowButton = new Button({380, 530}, {200, 55}, "Xac Nhan", font, 0, Theme::Success); cancelButton = new Button({720, 530}, {200, 55}, "Huy", font, 0, sf::Color(150, 150, 150));
    }
    ~BorrowBookScreen() { delete borrowButton; delete cancelButton; delete maSachField; }
    
    void setCurrentBook(Sach* book) {
        currentBook = book; if (currentBook) { maSachField->setText(currentBook->getMaSach()); std::string info = "Ten sach:  " + currentBook->getTenSach() + "\nTac gia:   " + currentBook->getTacGia() + "\nCon lai:   " + std::to_string(currentBook->getSoLuong()) + " cuon"; bookInfoText.setString(info); } 
        else { maSachField->clear(); bookInfoText.setString("Vui long nhap ma sach de muon."); }
    }
    
    std::string getMaSachInput() const { return maSachField->getText(); } 
    
    // [MỚI] Hàm hiển thị lỗi màu ĐỎ + Rung
    void setErrorMessage(const std::string& msg) {
        borrowMsg.setString(msg);
        borrowMsg.setFillColor(Theme::Error);
        startShake();
    }

    // [MỚI] Hàm hiển thị thành công màu XANH
    void setSuccessMessage(const std::string& msg) {
        borrowMsg.setString(msg);
        borrowMsg.setFillColor(Theme::Success);
    }

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
    
    void update(sf::Vector2f mousePos) { maSachField->update(); borrowButton->update(mousePos); cancelButton->update(mousePos); if (shaking) { shakeTime += 0.02f; float offset = sin(shakeTime * 50) * 10; panel.setPosition(300 + offset, 110); if (shakeTime >= 0.4f) { shaking = false; panel.setPosition(300, 110); } } }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { maSachField->handleEvent(event, mousePos); }
    int handleClick(sf::Vector2f mousePos) { if (borrowButton->handleClick(mousePos)) return 1; if (cancelButton->handleClick(mousePos)) return 2; return 0; }
    void render(sf::RenderWindow& window) { if (modal && modal->isShown()) { window.draw(panel); window.draw(titleText); window.draw(bookInfoText); maSachField->draw(window); window.draw(errMaSach); window.draw(borrowMsg); borrowButton->draw(window); cancelButton->draw(window); } }
    void clearInput() { errMaSach.setString(""); } 
    void clearMessage() { borrowMsg.setString(""); }
};
#endif