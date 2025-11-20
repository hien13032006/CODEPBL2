#ifndef SCREEN_READER_BORROWED_BOOKS_H
#define SCREEN_READER_BORROWED_BOOKS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Reader.h"
#include "Button.hpp"
#include "Utils.hpp"

struct BorrowedBookItem {
    sf::RectangleShape box;
    sf::Text title;
    sf::Text author;
    sf::Text dateBorrowed;
    sf::Text dateReturn;
    sf::Text remainingDays;
};

class ScreenReaderBorrowedBooks {
private:
    sf::Font &font;
    Reader* reader;

    sf::RectangleShape bg;
    sf::Text header;

    // Tiêu đề cột
    sf::Text colTitle, colAuthor, colDateBorrowed, colDateReturn, colRemaining;

    std::vector<BorrowedBookItem> items;

    Button btnClose;
    bool closed;

    sf::Text warningText;

public:
    ScreenReaderBorrowedBooks(sf::Font &f, Reader* r);
    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos);
    void render(sf::RenderWindow &window);
    bool wasClosed() const { return closed; }
};

ScreenReaderBorrowedBooks::ScreenReaderBorrowedBooks(sf::Font &f, Reader* r)
    : font(f), reader(r), btnClose({1160, 150}, {40, 40}, "X", f), closed(false)
{
    // Background
    bg.setSize({900, 600});
    bg.setFillColor(sf::Color(245,245,245,255));
    bg.setOutlineColor(sf::Color(100,100,100));
    bg.setOutlineThickness(2);
    bg.setPosition(300, 150);

    // Header
    header.setFont(font);
    header.setCharacterSize(36);
    header.setString("SACH DANG MUON");
    header.setFillColor(sf::Color::Black);
    header.setPosition(550, 175);

    btnClose.setColor(sf::Color(230, 90, 90));

    // Tiêu đề cột
    colTitle.setFont(font); colTitle.setCharacterSize(20); colTitle.setString("Ten sach");
    colTitle.setFillColor(sf::Color::Black); colTitle.setPosition(370, 265);

    colAuthor.setFont(font); colAuthor.setCharacterSize(20); colAuthor.setString("Tac gia");
    colAuthor.setFillColor(sf::Color::Black); colAuthor.setPosition(570, 265);

    colDateBorrowed.setFont(font); colDateBorrowed.setCharacterSize(20); colDateBorrowed.setString("Ngay muon");
    colDateBorrowed.setFillColor(sf::Color::Black); colDateBorrowed.setPosition(725, 265);

    colDateReturn.setFont(font); colDateReturn.setCharacterSize(20); colDateReturn.setString("Ngay tra");
    colDateReturn.setFillColor(sf::Color::Black); colDateReturn.setPosition(870, 265);

    colRemaining.setFont(font); colRemaining.setCharacterSize(20); colRemaining.setString("Con lai");
    colRemaining.setFillColor(sf::Color::Black); colRemaining.setPosition(980, 265);

    // ====== TẠO DÒNG CẢNH BÁO ======
    warningText.setFont(font);
    warningText.setCharacterSize(18);
    warningText.setPosition(350, 230);

    int soQuaHan = reader->DemSachQuaHan();
    if (soQuaHan > 0) {
        warningText.setString("Canh bao: Ban co " + std::to_string(soQuaHan) + " cuon sach qua han!");
        warningText.setFillColor(sf::Color::Red);
    } else {
        warningText.setString("");
    }

    // ======= TẠO LIST ITEMS =======
    NodeMuonSach* current = reader->getDanhSachPhieuMuon();
    float y = 295.f;
    bool alternate = false;

    while (current) {
        BorrowedBookItem item;

        item.box.setSize({785, 40});
        item.box.setFillColor(alternate ? sf::Color(255,255,255,200) : sf::Color(173, 216, 230));
        item.box.setOutlineColor(sf::Color(150,150,150));
        item.box.setOutlineThickness(1);
        item.box.setPosition(345, y);
        alternate = !alternate;

        // Tên sách
        item.title.setFont(font);
        item.title.setCharacterSize(18);
        item.title.setString(current->phieu->sach->getTenSach());
        item.title.setFillColor(sf::Color::Black);
        item.title.setPosition(360, y+10);

        // Tác giả
        item.author.setFont(font);
        item.author.setCharacterSize(18);
        item.author.setString(current->phieu->sach->getTacGia());
        item.author.setFillColor(sf::Color::Black);
        item.author.setPosition(570, y+10);

        // Ngày mượn
        item.dateBorrowed.setFont(font);
        item.dateBorrowed.setCharacterSize(18);
        item.dateBorrowed.setString(timeString(current->phieu->ngayMuon));
        item.dateBorrowed.setFillColor(sf::Color::Black);
        item.dateBorrowed.setPosition(730, y+10);

        // Ngày trả
        item.dateReturn.setFont(font);
        item.dateReturn.setCharacterSize(18);
        item.dateReturn.setString(timeString(current->phieu->ngayHetHan));
        item.dateReturn.setFillColor(sf::Color::Black);
        item.dateReturn.setPosition(870, y+10);

        // ====== Còn lại + kiểm tra quá hạn ======
        int conLai = current->phieu->soNgayConLai();

        item.remainingDays.setFont(font);
        item.remainingDays.setCharacterSize(18);

        if (conLai < 0) {
            item.remainingDays.setString("Qua han");
            item.remainingDays.setFillColor(sf::Color::Red);
        } else {
            item.remainingDays.setString(std::to_string(conLai));
            item.remainingDays.setFillColor(sf::Color::Black);
        }

        item.remainingDays.setPosition(1000, y+10);

        items.push_back(item);
        y += 50;
        current = current->next;
    }
}

void ScreenReaderBorrowedBooks::handleEvent(sf::Event &e, const sf::Vector2f &mousePos) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (btnClose.handleClick(mousePos)) {
            closed = true;
        }
    }
}

void ScreenReaderBorrowedBooks::render(sf::RenderWindow &window) {
    window.draw(bg);
    window.draw(header);

    // Cảnh báo
    window.draw(warningText);

    // Cột
    window.draw(colTitle);
    window.draw(colAuthor);
    window.draw(colDateBorrowed);
    window.draw(colDateReturn);
    window.draw(colRemaining);

    btnClose.draw(window);

    // Items
    for (auto &item : items) {
        window.draw(item.box);
        window.draw(item.title);
        window.draw(item.author);
        window.draw(item.dateBorrowed);
        window.draw(item.dateReturn);
        window.draw(item.remainingDays);
    }
}

#endif
