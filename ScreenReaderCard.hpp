#ifndef SCREEN_READER_CARD_H
#define SCREEN_READER_CARD_H

#include <SFML/Graphics.hpp>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"

class ScreenReaderCard {
private:
    sf::Font &font; 
    Reader *reader;
    sf::Text title; 
    sf::Text lbMaID, lbHoTen, lbEmail, lbSDT, lbNgayDK;
    bool closed; 
    RoundedRectangleShape panel; 
    Button btnClose;
    
    // Nút cập nhật
    Button* btnUpdate;
    bool updateRequested;

public:
    ScreenReaderCard(sf::Font &f, Reader *r) 
        : font(f), reader(r), 
          btnClose({960, 200}, {40, 40}, "X", f, 0, sf::Color(230, 90, 90)) 
    {
        closed = false;
        updateRequested = false;

        // Panel (600x450) - Nền trắng cho giống thẻ thật
        panel.setSize({600, 450}); 
        panel.setCornerRadius(20.0f);
        panel.setFillColor(sf::Color(255, 255, 255)); 
        panel.setPosition(400, 200);

        // Tiêu đề
        title.setFont(font); 
        title.setString("THE BAN DOC");
        title.setCharacterSize(36); 
        title.setFillColor(sf::Color(40, 45, 60)); // Chữ màu tối
        title.setPosition(570, 230);

        // Helper định dạng text
        auto setTxt = [&](sf::Text& t, std::string s, float y) {
            t.setFont(font); t.setString(s); 
            t.setCharacterSize(22); 
            t.setFillColor(sf::Color(60, 60, 60));
            t.setPosition(450, y);
        };

        // Lấy chuỗi ngày đăng ký (Giả sử reader có hàm getNgayDangKy dạng string hoặc convert)
        // Ở đây dùng tạm chuỗi mẫu nếu chưa có hàm convert
        char dateBuf[20];
        time_t t = reader->getNgayDangKy();
        strftime(dateBuf, 20, "%d/%m/%Y", localtime(&t));

        setTxt(lbMaID,   "ID: " + reader->getMaID(), 300);
        setTxt(lbHoTen,  "Ten: " + reader->getHoTen(), 340);
        setTxt(lbEmail,  "Email: " + reader->getEmail(), 380);
        setTxt(lbSDT,    "SDT: " + reader->getSDT(), 420);
        setTxt(lbNgayDK, "Ngay DK: " + std::string(dateBuf), 460);

        // Nút Cập nhật thông tin
        btnUpdate = new Button(
            {550, 530}, {300, 50}, "Cap Nhat Thong Tin", 
            font, 1, sf::Color(60, 100, 180)
        );
    }
    
    ~ScreenReaderCard() { delete btnUpdate; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { 
        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnClose.handleClick(mousePos)) closed = true;
            if (btnUpdate->handleClick(mousePos)) updateRequested = true;
        }
    }

    void render(sf::RenderWindow &window) { 
        window.draw(panel); 
        window.draw(title); 
        window.draw(lbMaID); window.draw(lbHoTen); 
        window.draw(lbEmail); window.draw(lbSDT); window.draw(lbNgayDK);
        btnClose.draw(window);
        btnUpdate->draw(window);
    }

    bool wasClosed() const { return closed; }
    
    // Kiểm tra xem có bấm nút Update không để App chuyển màn hình
    bool isUpdateRequested() { 
        if (updateRequested) { updateRequested = false; return true; }
        return false;
    }
};
#endif