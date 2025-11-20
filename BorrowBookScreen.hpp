#ifndef BORROW_BOOK_SCREEN_HPP
#define BORROW_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "Book.h"
#include "InputField.hpp"
#include <string>

class BorrowBookScreen {
private:
    Modal* modal;
    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text bookInfoText;
    sf::Text borrowMsg;

    InputField* maSachField;  // ô nhập maSach
    Button* borrowButton;
    Button* cancelButton;

    sf::Text errMaSach;        // text báo lỗi

    sf::Text borrowedInfoText; // thông tin phiếu mượn hiển thị bên dưới

    Sach* currentBook;

    // ANIMATION RUNG
    bool shaking = false;
    float shakeTime = 0.0f;
    const float shakeDuration = 0.4f;

public:
    BorrowBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        // Panel chính
        panel.setSize(sf::Vector2f(600, 400));
        panel.setFillColor(sf::Color(40, 50, 70, 220));
        panel.setPosition(400, 250);

        // Title
        titleText.setFont(font);
        titleText.setString("Muon Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(panel.getPosition().x + panel.getSize().x / 2,
                              panel.getPosition().y + 40);

        // Thông tin sách
        bookInfoText.setFont(font);
        bookInfoText.setCharacterSize(20);
        bookInfoText.setFillColor(sf::Color::White);
        bookInfoText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 100);

        // Thông báo mượn
        borrowMsg.setFont(font);
        borrowMsg.setCharacterSize(16);
        borrowMsg.setFillColor(sf::Color(255, 80, 80));
        borrowMsg.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 170);

        // Ô nhập maSachss
        maSachField = new InputField({panel.getPosition().x + 20, panel.getPosition().y + 100},
                                     {560, 50}, "Nhap ma sach...", font);

        // Text báo lỗi
        errMaSach.setFont(font);
        errMaSach.setCharacterSize(14);
        errMaSach.setFillColor(sf::Color(255, 80, 80));
        errMaSach.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 255);

        // Button
        borrowButton = new Button({panel.getPosition().x + 50, panel.getPosition().y + 340},
                                  {200, 40}, "Muon", font);

        cancelButton = new Button({panel.getPosition().x + 350, panel.getPosition().y + 340},
                                  {200, 40}, "Huy", font, 0, sf::Color(100, 100, 120));

        // Phiếu mượn hiển thị
        borrowedInfoText.setFont(font);
        borrowedInfoText.setCharacterSize(16);
        borrowedInfoText.setFillColor(sf::Color::White);
        borrowedInfoText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 200);
        borrowedInfoText.setString("");
    }

    ~BorrowBookScreen() {
        delete borrowButton;
        delete cancelButton;
        delete maSachField;
    }

    void setCurrentBook(Sach* book) {
        currentBook = book;
        if (currentBook) {
            std::string info = "Ten sach: " + currentBook->getTenSach() +
                               "\nMa sach: " + currentBook->getMaSach() +
                               "\nSo luong: " + std::to_string(currentBook->getSoLuong());
            bookInfoText.setString(info);
        }
    }

    Sach* getCurrentBook() const { return currentBook; }
    std::string getMaSachInput() const { return maSachField->getText(); }

    void setBorrowMessage(const std::string& msg) { borrowMsg.setString(msg); }

    void startShake() { shaking = true; shakeTime = 0; }

    // Validate nhập maSach
    bool validate() {
        errMaSach.setString("");
        if (getMaSachInput().empty()) {
            errMaSach.setString("Vui long nhap ma sach");
            startShake();
            return false;
        }
        return true;
    }

    void setBorrowedBookInfo(Sach& book, const std::string& borrowDate, const std::string& returnDate) {
        std::string info = "Ma sach: " + book.getMaSach() +
                           "\nTen sach: " + book.getTenSach() +
                           "\nTac gia: " + book.getTacGia() +
                           "\nNgay muon: " + borrowDate +
                           "\nNgay tra: " + returnDate;
        borrowedInfoText.setString(info);
    }

    void update(sf::Vector2f mousePos) {
        maSachField->update();
        borrowButton->update(mousePos);
        cancelButton->update(mousePos);

        // Animation rung
        if (shaking) {
            shakeTime += 0.02f;
            float offset = sin(shakeTime * 40) * 8;
            panel.setPosition(400 + offset, 250);
            if (shakeTime >= shakeDuration) {
                shaking = false;
                panel.setPosition(400, 250);
            }
        }
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        maSachField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (borrowButton->handleClick(mousePos)) return 1;
        if (cancelButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(panel);
            window.draw(titleText);
            window.draw(bookInfoText);
            window.draw(borrowMsg);
            maSachField->draw(window);
            window.draw(errMaSach);
            borrowButton->draw(window);
            cancelButton->draw(window);
            window.draw(borrowedInfoText); // hiển thị phiếu mượn
        }
    }

    void clearMessage() { borrowMsg.setString(""); }
    void clearInput() { maSachField->clear(); errMaSach.setString(""); borrowedInfoText.setString(""); }

    bool panelContains(sf::Vector2f pos) const { return panel.getGlobalBounds().contains(pos); }
    bool maSachFieldContains(sf::Vector2f pos) const { return maSachField->getGlobalBounds().contains(pos); }

};

#endif
