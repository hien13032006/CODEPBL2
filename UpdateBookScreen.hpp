#ifndef UPDATE_BOOK_SCREEN_HPP
#define UPDATE_BOOK_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "LibrarySystem.h"
#include "Button.hpp"

class UpdateBookScreen {
private:
    sf::Font &font;
    LibrarySystem *L;

    bool closed;

    // UI
    sf::RectangleShape bg;

    sf::Text title;

    // Input mã sách
    sf::RectangleShape boxMa;
    sf::Text textMa;
    bool typingMa;

    Button btnFind;

    // Thông tin sách
    Sach* selectedBook;

    sf::Text infoText;

    // Các nút update
    Button btnTen;
    Button btnTacGia;
    Button btnTheLoai;
    Button btnNam;
    Button btnNXB;

    Button btnSave;
    Button btnExit;

    // Popup nhập dữ liệu mới
    bool popupOpen;
    sf::RectangleShape popupBox;
    sf::Text popupLabel;
    sf::RectangleShape popupInput;
    sf::Text popupText;
    bool typingPopup;
    int updateField;   // 1=ten,2=tacgia,3=theloai,4=nam,5=nxb

    // Thông báo
    sf::Text resultMsg;

public:
    UpdateBookScreen(sf::Font &f, LibrarySystem *lib)
        : font(f), L(lib),
          btnFind({680, 240}, {120, 40}, "Tim", f),
          btnTen({420, 520}, {150, 40}, "Ten", f),
          btnTacGia({580, 520}, {150, 40}, "Tac gia", f),
          btnTheLoai({740, 520}, {150, 40}, "The loai", f),
          btnNam({420, 570}, {150, 40}, "Nam XB", f),
          btnNXB({580, 570}, {150, 40}, "Nha XB", f),
          btnSave({740, 570}, {150, 40}, "Luu", f),
          btnExit({900, 570}, {150, 40}, "Thoat", f)
    {
        closed = false;
        typingMa = false;
        selectedBook = nullptr;

        // Background
        bg.setSize({800, 550});
        bg.setFillColor(sf::Color(245, 245, 245));
        bg.setOutlineThickness(2);
        bg.setOutlineColor(sf::Color(100, 100, 100));
        bg.setPosition(350, 150);

        // Title
        title.setFont(font);
        title.setString("CAP NHAT THONG TIN SACH");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::Black);
        title.setPosition(480, 180);

        // Input mã sách
        boxMa.setSize({250, 40});
        boxMa.setFillColor(sf::Color::White);
        boxMa.setOutlineThickness(2);
        boxMa.setOutlineColor(sf::Color(120, 120, 120));
        boxMa.setPosition(420, 240);

        textMa.setFont(font);
        textMa.setCharacterSize(22);
        textMa.setFillColor(sf::Color::Black);
        textMa.setPosition(425, 245);

        infoText.setFont(font);
        infoText.setCharacterSize(22);
        infoText.setFillColor(sf::Color::Black);
        infoText.setPosition(420, 300);

        // Popup
        popupOpen = false;
        typingPopup = false;

        popupBox.setSize({400, 215});
        popupBox.setFillColor(sf::Color(191, 219, 255));
        popupBox.setOutlineThickness(3);
        popupBox.setOutlineColor(sf::Color::Black);
        popupBox.setPosition(500, 250);

        popupLabel.setFont(font);
        popupLabel.setCharacterSize(24);
        popupLabel.setFillColor(sf::Color::Black);
        popupLabel.setPosition(520, 260);

        popupInput.setSize({360, 45});
        popupInput.setFillColor(sf::Color::White);
        popupInput.setOutlineThickness(2);
        popupInput.setOutlineColor(sf::Color::Black);
        popupInput.setPosition(520, 315);

        popupText.setFont(font);
        popupText.setCharacterSize(22);
        popupText.setFillColor(sf::Color::Black);
        popupText.setPosition(525, 322);

        resultMsg.setFont(font);
        resultMsg.setCharacterSize(22);
        resultMsg.setFillColor(sf::Color::Blue);
        resultMsg.setPosition(615, 530);
    }

    bool isClosed() const { return closed; }

    // Hàm mở popup
    void openPopup(int field, const std::string &label) {
        updateField = field;
        popupLabel.setString(label);
        popupText.setString("");
        popupOpen = true;
        typingPopup = true;
    }

    void updateInfoText() {
        if (!selectedBook) {
            infoText.setString("Khong tim thay sach!");
            return;
        }

        std::string s =
            "Ma: " + selectedBook->getMaSach() +
            "\nTen: " + selectedBook->getTenSach() +
            "\nTac gia: " + selectedBook->getTacGia() +
            "\nThe loai: " + selectedBook->getTheLoai() +
            "\nNam XB: " + std::to_string(selectedBook->getNamXuatBan()) +
            "\nNha XB: " + selectedBook->getNhaXuatBan();

        infoText.setString(s);
    }

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
    // ---------------- Popup ----------------
    if (popupOpen) {
        // Nhập popup
        if (e.type == sf::Event::TextEntered && typingPopup) {
            if (e.text.unicode == 8) { // backspace
                std::string s = popupText.getString();
                if (!s.empty()) s.pop_back();
                popupText.setString(s);
            } else if (e.text.unicode >= 32 && e.text.unicode < 128) { // ký tự bình thường
                std::string s = popupText.getString();
                s.push_back((char)e.text.unicode);
                popupText.setString(s);
            }
        }

        // Nhấn Enter = lưu
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
            std::string val = popupText.getString();
            if (selectedBook) {
                switch(updateField) {
                    case 1: selectedBook->setTenSach(val); break;
                    case 2: selectedBook->setTacGia(val); break;
                    case 3: selectedBook->setTheLoai(val); break;
                    case 4: selectedBook->setNamXuatBan(std::stoi(val)); break;
                    case 5: selectedBook->setNhaXuatBan(val); break;
                }
            }
            updateInfoText();
            popupOpen = false;
            typingPopup = false;
        }

        return; // ưu tiên popup
    }

    // ---------------- Chọn input mã sách ----------------
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        if (boxMa.getGlobalBounds().contains(mouse)) {
            typingMa = true;
            boxMa.setOutlineColor(sf::Color::Blue);
        } else {
            typingMa = false;
            boxMa.setOutlineColor(sf::Color(120, 120, 120));
        }

        // Nút THOÁT
        if (btnExit.handleClick(mouse)) {
            closed = true;
        }

        // Nút TÌM
        if (btnFind.handleClick(mouse)) {
            selectedBook = L->TimSachTheoMa(textMa.getString());
            updateInfoText();
            resultMsg.setString("");
        }

        // Nếu đã chọn sách, xử lý các nút update và lưu
        if (selectedBook) {
            if (btnTen.handleClick(mouse)) openPopup(1, "Nhap ten moi:");
            if (btnTacGia.handleClick(mouse)) openPopup(2, "Nhap tac gia moi:");
            if (btnTheLoai.handleClick(mouse)) openPopup(3, "Nhap the loai moi:");
            if (btnNam.handleClick(mouse)) openPopup(4, "Nhap nam XB moi:");
            if (btnNXB.handleClick(mouse)) openPopup(5, "Nhap nha XB moi:");
            if (btnSave.handleClick(mouse)) {
                Sach::ghiFile("DanhSachSach.txt", L->getDanhSachSach());
                resultMsg.setString("Da luu thay doi vao file!");
            }
        }
    }

    // ---------------- Nhập mã sách ----------------
    if (typingMa && e.type == sf::Event::TextEntered) {
        if (e.text.unicode == 8) { // backspace
            std::string s = textMa.getString();
            if (!s.empty()) s.pop_back();
            textMa.setString(s);
        } else if (e.text.unicode >= 32 && e.text.unicode < 128) {
            std::string s = textMa.getString();
            s.push_back((char)e.text.unicode);
            textMa.setString(s);
        }
    }
}


    void render(sf::RenderWindow &window) {
        window.draw(bg);
        window.draw(title);

        window.draw(boxMa);
        window.draw(textMa);

        btnFind.draw(window);

        window.draw(infoText);

        if (selectedBook) {
            btnTen.draw(window);
            btnTacGia.draw(window);
            btnTheLoai.draw(window);
            btnNam.draw(window);
            btnNXB.draw(window);
            btnSave.draw(window);
        }

        btnExit.draw(window);
        window.draw(resultMsg);

        // Popup
        if (popupOpen) {
            window.draw(popupBox);
            window.draw(popupLabel);
            window.draw(popupInput);
            window.draw(popupText);
        }
    }
};

#endif
