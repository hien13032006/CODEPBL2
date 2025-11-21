#ifndef UPDATE_LIBRARIAN_SCREEN_HPP
#define UPDATE_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "Node.h"
#include "Librarian.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "InputField.hpp"

class UpdateLibrarianScreen {
private:
    sf::Font &font; Librarian *librarian; 
    bool closed;
    RoundedRectangleShape bg; sf::Text title;
    
    // 5 trường thông tin cần sửa
    InputField *inName, *inPhone, *inEmail, *inUser, *inPass;
    Button *btnSave, *btnCancel;
    sf::Text msg;

public:
    UpdateLibrarianScreen(sf::Font &f, Librarian *l) : font(f), librarian(l) {
        closed = false;

        // Background (600x650)
        bg.setSize({600, 650});
        bg.setCornerRadius(15.0f);
        bg.setFillColor(sf::Color(40, 45, 60)); // Nền tối
        bg.setPosition(400, 100);

        title.setFont(font); title.setString("CAP NHAT THONG TIN");
        title.setCharacterSize(32); title.setFillColor(sf::Color::White);
        title.setPosition(520, 130);

        // Init Fields
        float startY = 200; float gap = 70;
        inName = new InputField({450, startY}, {500, 50}, "Ho ten", font); 
        inName->setText(l->getHoTen());

        inPhone = new InputField({450, startY+gap}, {500, 50}, "So dien thoai", font);
        inPhone->setText(l->getSDT());

        inEmail = new InputField({450, startY+gap*2}, {500, 50}, "Email", font);
        inEmail->setText(l->getEmail());

        inUser = new InputField({450, startY+gap*3}, {500, 50}, "Username", font);
        inUser->setText(l->getUsername());

        inPass = new InputField({450, startY+gap*4}, {500, 50}, "Password (moi)", font, true);
        inPass->setText(l->getPassword());

        btnSave = new Button({450, 600}, {200, 55}, "Luu", font, 0, sf::Color(76, 175, 80));
        btnCancel = new Button({750, 600}, {200, 55}, "Huy", font, 0, sf::Color(180, 80, 80));

        msg.setFont(font); msg.setCharacterSize(18); msg.setPosition(450, 560);
    }

    ~UpdateLibrarianScreen() {
        delete inName; delete inPhone; delete inEmail; delete inUser; delete inPass;
        delete btnSave; delete btnCancel;
    }

    bool isClosed() const { return closed; }

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse) {
        inName->handleEvent(e, mouse); inPhone->handleEvent(e, mouse);
        inEmail->handleEvent(e, mouse); inUser->handleEvent(e, mouse); inPass->handleEvent(e, mouse);

        if (e.type == sf::Event::MouseButtonPressed) {
            if (btnSave->handleClick(mouse)) {
                // Cập nhật object
                librarian->setHoTen(inName->getText());
                librarian->setSDT(inPhone->getText());
                librarian->setEmail(inEmail->getText());
                librarian->setUsername(inUser->getText());
                librarian->setPassword(inPass->getText());
                
                // Lưu file (Logic đơn giản: Ghi đè file ThuThu.txt)
                // Lưu ý: Trong thực tế cần ghi lại toàn bộ danh sách, ở đây giả sử chỉ có 1 thủ thư hoặc xử lý bên ngoài
                // Để an toàn, ta chỉ báo thành công và đóng, việc ghi file nên để App xử lý hoặc LibrarySystem
                msg.setString("Cap nhat thanh cong!");
                msg.setFillColor(sf::Color(100, 255, 100));
                closed = true; 
            }
            if (btnCancel->handleClick(mouse)) closed = true;
        }
    }

    void update(sf::Vector2f mouse) {
        inName->update(); inPhone->update(); inEmail->update(); inUser->update(); inPass->update();
        btnSave->update(mouse); btnCancel->update(mouse);
    }

    void render(sf::RenderWindow &window) {
        window.draw(bg); window.draw(title);
        inName->draw(window); inPhone->draw(window); inEmail->draw(window);
        inUser->draw(window); inPass->draw(window);
        btnSave->draw(window); btnCancel->draw(window);
        window.draw(msg);
    }
};
#endif