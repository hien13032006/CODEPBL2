#define NOMINMAX
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "ScreenBase.h"
#include "Theme.h"

// Các màn hình của bạn
#include "ScreenReaderLogin.h"
#include "ScreenReaderRegister.h"
#include "ScreenReaderMenu.h"
#include "ScreenLoginLibrarian.h"
#include "ScreenSearchBook.h"
#include "ScreenBorrowBook.h"
#include "ScreenReturnBook.h"
#include "ScreenRateBook.h"
#include "ScreenTopBook.h"
#include "ScreenHistory.h"
#include "ScreenViewBook.h"
#include "ScreenViewReader.h"
#include "ScreenLibAdd.h"
#include "ScreenLibDelete.h"
#include "ScreenLibUpdate.h"
#include "ScreenRole.h"
#include "ScreenWelcome.h"
#include "ScreenReaderChoice.h"

#include "LibrarySystem.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Thu Vien Bach Khoa");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        std::cerr << "Khong the tai font!\n";
        return -1;
    }

    // Load background
    sf::Texture bgTexture1, bgTexture2;
    sf::Sprite bgSprite1, bgSprite2;
    if (!bgTexture1.loadFromFile("pic1.png") || !bgTexture2.loadFromFile("pic2.png")) {
        std::cerr << "Khong the tai hinh nen!\n";
        return -1;
    }
    bgSprite1.setTexture(bgTexture1);
    bgSprite2.setTexture(bgTexture2);

    // Scale hình nền theo cửa sổ
    sf::Vector2u winSize = window.getSize();
    bgSprite1.setScale(
        (float)winSize.x / bgTexture1.getSize().x,
        (float)winSize.y / bgTexture1.getSize().y
    );
    bgSprite2.setScale(
        (float)winSize.x / bgTexture2.getSize().x,
        (float)winSize.y / bgTexture2.getSize().y
    );

    // Khởi tạo hệ thống thư viện
    LibrarySystem L;
    L.DocFileHeThong("DanhSachSach.txt");
    L.DocFileDocGia();
    L.XayDungTatCaCay();

    Reader* currentReader = nullptr;

    // --- Khởi tạo các màn hình ---
    AppState state = SCREEN_VISIT;

    ScreenWelcome* screenWelcome = new ScreenWelcom([&]() {
        state = SCREEN_ROLE;
    });
    screenWelcome->init(font);

    ScreenRole* screenRole = new ScreenRole(
        [&]() { state = SCREEN_READER_CHOICE },
        [&]() { state = SCREEN_LIB_LOGIN }
    );
    screenRole->init(font);

    ScreenReaderLogin scrRLogin(font, &L, &currentReader);
    ScreenReaderRegister scrRReg(font, &L);
    ScreenReaderMenu scrRMenu(font, &window);
    ScreenLoginLibrarian scrLibLogin(font, &L); // Màn hình đăng nhập Thủ thư

    // --- Pointer màn hình hiện tại ---
    ScreenBase* screen = nullptr;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện cho màn hình hiện tại
            switch (state) {
                case SCREEN_WELCOME: screenWelcome->handle(event, window); break;
                case SCREEN_ROLE:  screenRole->handle(event, window); break;
                case SCREEN_READER_LOGIN: scrRLogin.handleEvent(event, state, &window); break;
                case SCREEN_LIB_LOGIN:    scrLibLogin.handleEvent(event, state, &window); break;
                case SCREEN_READER_REGISTER: scrRReg.handleEvent(event, state, &window); break;
                case SCREEN_READER_MENU:  scrRMenu.handleEvent(event, state, &window); break;
                default: break;
            }
        }

        // --- Update màn hình hiện tại ---
        switch (state) {
            case SCREEN_WELCOME: screenWelcome->update(); break;
            case SCREEN_ROLE:  screenRole->update(); break;
            case SCREEN_READER_LOGIN: scrRLogin.update(); break;
            case SCREEN_LIB_LOGIN:    scrLibLogin.update(); break;
            case SCREEN_READER_REGISTER: scrRReg.update(); break;
            case SCREEN_READER_MENU:  scrRMenu.update(); break;
            default: break;
        }

        // --- Vẽ ---
        window.clear();
        if (state == SCREEN_WELCOME) window.draw(bgSprite1);
        else if (state == SCREEN_ROLE) window.draw(bgSprite2);

        switch (state) {
            case SCREEN_WELCOME:    screenWelcome->draw(window); break;
            case SCREEN_ROLE:    screenRole->draw(window); break;
            case SCREEN_READER_LOGIN:   scrRLogin.draw(window); break;
            case SCREEN_LIB_LOGIN:  scrLibLogin.draw(window); break;
            case SCREEN_READER_REGISTER: scrRReg.draw(window); break;
            case SCREEN_READER_MENU:     scrRMenu.draw(window); break;
            default: break;
        }

        window.display();
    }

    // --- Giải phóng bộ nhớ ---

    return 0;
}
