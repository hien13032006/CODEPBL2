#ifndef UPDATE_READER_SCREEN_HPP
#define UPDATE_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "Reader.h"
#include "Button.hpp"

class UpdateReaderScreen {
private:
    sf::Font &font;
    Reader *reader;
    NodeReader* head;

    bool closed;

    // UI
    sf::RectangleShape bg;
    sf::Text title;

    // Thông tin cá nhân
    sf::Text infoText;

    // Nút cập nhật từng field
    Button btnName;
    Button btnPhone;
    Button btnEmail;
    Button btnUsername;
    Button btnPassword;
    Button btnExit;
    Button btnSave;

    // Popup nhập dữ liệu mới
    bool popupOpen;
    int activeField; // 1=name,2=phone,3=email,4=username,5=password

    sf::RectangleShape popupBox;
    sf::Text popupLabel;
    sf::RectangleShape popupInput;   // mật khẩu mới hoặc các field khác
    sf::Text popupText;
    sf::RectangleShape popupInput2;  // mật khẩu hiện tại
    sf::Text popupText2;
    int activePasswordInput; // 0=none, 1=current, 2=new

    // Thông báo kết quả
    sf::Text resultMsg;

public:
    UpdateReaderScreen(sf::Font &f, Reader *r, NodeReader* h)
        : font(f), reader(r), head(h),
          btnName({420, 450}, {150, 40}, "Ten", f),
          btnPhone({580, 450}, {150, 40}, "SDT", f),
          btnEmail({740, 450}, {150, 40}, "Email", f),
          btnUsername({900, 450}, {150, 40}, "Username", f),
          btnPassword({420, 510}, {150, 40}, "Password", f),
          btnSave({580, 510}, {150, 40}, "Luu", f),
          btnExit({740, 510}, {150, 40}, "Thoat", f)
    {
        closed = false;
        popupOpen = false;
        activePasswordInput = 0;
        updateInfoText();

        // Background
        bg.setSize({800, 500});
        bg.setFillColor(sf::Color(245, 245, 245));
        bg.setOutlineThickness(2);
        bg.setOutlineColor(sf::Color(100,100,100));
        bg.setPosition(350,150);

        // Title
        title.setFont(font);
        title.setString("CAP NHAT THONG TIN DOC GIA");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color::Black);
        title.setPosition(500,200);

        // Info text
        infoText.setFont(font);
        infoText.setCharacterSize(22);
        infoText.setFillColor(sf::Color::Black);
        infoText.setPosition(420,250);

        // Popup box
        popupBox.setSize({400,220});
        popupBox.setFillColor(sf::Color::White);
        popupBox.setOutlineThickness(3);
        popupBox.setOutlineColor(sf::Color::Black);
        popupBox.setPosition(500,250);

        popupLabel.setFont(font);
        popupLabel.setCharacterSize(24);
        popupLabel.setFillColor(sf::Color::Black);
        popupLabel.setPosition(520,260);

        popupInput.setSize({360,45});
        popupInput.setFillColor(sf::Color::White);
        popupInput.setOutlineThickness(2);
        popupInput.setOutlineColor(sf::Color::Black);
        popupInput.setPosition(520,315);

        popupText.setFont(font);
        popupText.setCharacterSize(22);
        popupText.setFillColor(sf::Color::Black);
        popupText.setPosition(525,322);

        popupInput2.setSize({360,45});
        popupInput2.setFillColor(sf::Color::White);
        popupInput2.setOutlineThickness(2);
        popupInput2.setOutlineColor(sf::Color::Black);
        popupInput2.setPosition(520,260);

        popupText2.setFont(font);
        popupText2.setCharacterSize(22);
        popupText2.setFillColor(sf::Color::Black);
        popupText2.setPosition(525,267);

        // Result message
        resultMsg.setFont(font);
        resultMsg.setCharacterSize(22);
        resultMsg.setFillColor(sf::Color::Blue);
        resultMsg.setPosition(420,410);
    }

    bool isClosed() const { return closed; }

    void updateInfoText() {
        if (!reader) return;
        auto maskPassword = [](const std::string &pass){
            int len = pass.length();
            if(len <=2) return std::string(len,'*');
            return std::string(len-2,'*') + pass.substr(len-2);
        };
        std::string s = 
            "Ten: " + reader->getHoTen() +
            "\nSDT: " + reader->getSDT() +
            "\nEmail: " + reader->getEmail() +
            "\nUsername: " + reader->getUsername() +
            "\nPassword: " + maskPassword(reader->getPassword());
        infoText.setString(s);
    }

    void openPopup(int field, const std::string &label){
        activeField = field;
        popupOpen = true;
        popupLabel.setString(label);
        popupText.setString("");
        popupText2.setString("");
        activePasswordInput = 0;
    }

    void handleEvent(sf::Event &e, const sf::Vector2f &mouse){
        if(popupOpen){
            // Click vào popup để active input
            if(e.type==sf::Event::MouseButtonPressed && e.mouseButton.button==sf::Mouse::Left){
                if(activeField==5){ // password
                    if(popupInput2.getGlobalBounds().contains(mouse)) activePasswordInput=1; // current
                    else if(popupInput.getGlobalBounds().contains(mouse)) activePasswordInput=2; // new
                    else activePasswordInput=0;
                } else {
                    if(popupInput.getGlobalBounds().contains(mouse)) activePasswordInput=2; // normal field
                    else activePasswordInput=0;
                }
            }

            // Nhập text
            if(e.type==sf::Event::TextEntered){
                if(activeField==5){ // password
                    if(activePasswordInput==1){ // current
                        std::string s = popupText2.getString();
                        if(e.text.unicode==8 && !s.empty()) s.pop_back();
                        else if(e.text.unicode>=32 && e.text.unicode<128) s.push_back((char)e.text.unicode);
                        popupText2.setString(s);
                    } else if(activePasswordInput==2){ // new
                        std::string s = popupText.getString();
                        if(e.text.unicode==8 && !s.empty()) s.pop_back();
                        else if(e.text.unicode>=32 && e.text.unicode<128) s.push_back((char)e.text.unicode);
                        popupText.setString(s);
                    }
                } else if(activePasswordInput==2){ // normal field
                    std::string s = popupText.getString();
                    if(e.text.unicode==8 && !s.empty()) s.pop_back();
                    else if(e.text.unicode>=32 && e.text.unicode<128) s.push_back((char)e.text.unicode);
                    popupText.setString(s);
                }
            }

            // Enter lưu dữ liệu
            if(e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Enter){
                std::string val = popupText.getString();
                if(reader){
                    if(activeField==1) reader->setHoTen(val);
                    else if(activeField==2) reader->setSDT(val);
                    else if(activeField==3) reader->setEmail(val);
                    else if(activeField==4) reader->setUsername(val);
                    else if(activeField==5){
                        std::string oldPass = popupText2.getString();
                        if(oldPass==reader->getPassword()){
                            reader->setPassword(val);
                            resultMsg.setString("Da doi mat khau!");
                        } else {
                            resultMsg.setString("Mat khau hien tai khong dung!");
                        }
                    }
                }
                popupOpen = false;
                activePasswordInput = 0;
                updateInfoText();
            }

            return; // không xử lý nút khi popup mở
        }

        // Click nút
        if(e.type==sf::Event::MouseButtonPressed && e.mouseButton.button==sf::Mouse::Left){
            if(btnExit.handleClick(mouse)) closed = true;
            if(btnSave.handleClick(mouse)){
                std::ofstream out("DocGia.txt", std::ios::trunc);
                for(NodeReader* p=head; p!=nullptr; p=p->next)
                    out << p->data->toCSV() << "\n";
                out.close();
                resultMsg.setString("Cap nhat thong tin thanh cong!");
            }
            if(btnName.handleClick(mouse)) openPopup(1,"Nhap ten moi:");
            if(btnPhone.handleClick(mouse)) openPopup(2,"Nhap SDT moi:");
            if(btnEmail.handleClick(mouse)) openPopup(3,"Nhap Email moi:");
            if(btnUsername.handleClick(mouse)) openPopup(4,"Nhap Username moi:");
            if(btnPassword.handleClick(mouse)) openPopup(5,"Nhap Password moi:");
        }
    }

    void render(sf::RenderWindow &window){
        window.draw(bg);
        window.draw(title);
        window.draw(infoText);

        btnName.draw(window);
        btnPhone.draw(window);
        btnEmail.draw(window);
        btnUsername.draw(window);
        btnPassword.draw(window);
        btnSave.draw(window);
        btnExit.draw(window);

        window.draw(resultMsg);

        if(popupOpen){
            window.draw(popupBox);
            window.draw(popupLabel);
            window.draw(popupInput);
            window.draw(popupText);
            if(activeField==5){
                window.draw(popupInput2);
                window.draw(popupText2);
            }
        }
    }
};

#endif
