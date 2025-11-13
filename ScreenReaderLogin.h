#ifndef SCREEN_READER_LOGIN_H
#define SCREEN_READER_LOGIN_H

#include <SFML/Graphics.hpp>
#include "TextBox.h"
#include "Button.h"
#include "Reader.h"
#include "LibrarySystem.h"
#include "ScreenBase.h"

class ScreenReaderLogin : public ScreenBase {
private:
    sf::Font &font;
    LibrarySystem *library;
    Reader **currentReader;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Text lbUser;
    sf::Text lbPass;
    sf::Text lbError;

public:
    TextBox boxUser;
    TextBox boxPass;
    Button btnOK;
    Button btnBack;

    ScreenReaderLogin(sf::Font &f, LibrarySystem *lib = nullptr, Reader **curReader = nullptr)
        : font(f), library(lib), currentReader(curReader)
    {
        // Texts
        lbUser.setFont(font);
        lbUser.setString("Username:");
        lbUser.setCharacterSize(24);
        lbUser.setFillColor(sf::Color::Black);

        lbPass.setFont(font);
        lbPass.setString("Password:");
        lbPass.setCharacterSize(24);
        lbPass.setFillColor(sf::Color::Black);
    }

    void init(sf::RenderWindow &window) {
        // Hình nền full screen
        if(!bgTexture.loadFromFile("picLogin.png")) {
            // xử lý lỗi nếu file không tồn tại
        }

        bgSprite.setTexture(bgTexture);
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgTexture.getSize().x,
            float(winSize.y) / bgTexture.getSize().y
        );

        // --- Vị trí căn giữa các ô ---
        float centerX = winSize.x * 0.5f;
        float boxWidth = 320.f;
        float boxHeight = 45.f;

        
        float boxX = winSize.x * 0.6f;
        float boxYUser = winSize.y * 0.40f;
        float boxYPass = winSize.y * 0.50f;

        float labelOffsetX = 50.f;  // khoảng cách giữa nhãn và textbox
        float labelOffsetY = 5.f;   // nhãn nằm cao hơn textbox

        // Đặt nhãn sát bên trái textbox
        lbUser.setPosition(boxX - labelOffsetX - 100.f, boxYUser + labelOffsetY);
        lbPass.setPosition(boxX - labelOffsetX - 100.f, boxYPass + labelOffsetY);

        // Khởi tạo textbox
        boxUser = TextBox(font, {boxX, boxYUser}, {350, 55});
        boxPass = TextBox(font, {boxX, boxYPass}, {350, 55});
        boxPass.setPassword(true);

        // Nút Login/Back bên phải
        btnOK   = Button("Login", font, {winSize.x * 0.57f, winSize.y * 0.60f}, {200, 60});
        btnBack = Button("Back",  font, {winSize.x * 0.70f, winSize.y * 0.60f}, {200, 60});
    }

    void handleEvent(sf::Event &e, AppState &current, sf::RenderWindow *window) override {
        if(!window) return;

        boxUser.handleEvent(e);
        boxPass.handleEvent(e);
        btnOK.handleEvent(e, *window);
        btnBack.handleEvent(e, *window);

        // Khi nhấn nút Login
        btnOK.setCallback([this, &current]() {
            std::string username = boxUser.get();
            std::string password = boxPass.get();

            if (library) {
                Reader *r = library->DangNhapDocGia(username, password);
                if (r) {
                    if (currentReader) *currentReader = r;
                    lbError.setString("");
                    current = SCREEN_READER_MENU; // tạm dùng màn hình lựa chọn Reader
                } else {
                    lbError.setString("Sai username hoặc password!");
                }
            } else {
                lbError.setString("Loi he thong thu vien!");
            }
        });
        // Khi nhấn nút Back
        btnBack.setCallback([&]() {
            current = SCREEN_WELCOME;
        });
    }

    void update() override {
        btnOK.update();
        btnBack.update();
    }

    void draw(sf::RenderWindow &w) override {
        w.draw(bgSprite);
        w.draw(lbUser);
        w.draw(lbPass);
        boxUser.draw(w);
        boxPass.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif
