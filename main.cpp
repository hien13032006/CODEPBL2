#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "ScreenBase.h"
#include "Theme.h"

// SCREEN imports
#include "ScreenWelcome.h"
#include "ScreenRole.h"

#include "ScreenReaderLogin.h"
#include "ScreenReaderRegister.h"
#include "ScreenReaderMenu.h"

#include "ScreenSearchBook.h"
#include "ScreenViewBook.h"
#include "ScreenBorrowBook.h"
#include "ScreenReturnBook.h"
#include "ScreenRateBook.h"
#include "ScreenTopBook.h"
#include "ScreenHistory.h"

#include "ScreenLibLogin.h"
#include "ScreenLibMenu.h"
#include "ScreenLibAdd.h"
#include "ScreenLibDelete.h"
#include "ScreenLibUpdate.h"

#include "ScreenViewReader.h"

#include "LibrarySystem.h"

int main() {
    // Tạo cửa sổ full màn hình
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Library",
        sf::Style::Fullscreen
    );
    window.setFramerateLimit(60);

    // ==== Tải font ====
    sf::Font font;
    if (!font.loadFromFile("assets/DejaVuSans.ttf")) {
        printf("Khong tim thay font!\n");
        return 0;
    }

    // ==== Khởi tạo hệ thống ====
    LibrarySystem L;
    L.DocFileHeThong("assets/DanhSachSach.txt");
    L.DocFileDocGia();
    L.XayDungTatCaCay();

    Reader *currentReader = nullptr;
    USER   *currentUser   = nullptr;     // dành cho thư thư

    // ==== Tạo màn hình ====
    ScreenWelcome        scrWelcome(font);
    ScreenRole           scrRole(font);

    ScreenReaderLogin    scrRLogin(font, &L, &currentReader);
    ScreenReaderRegister scrRReg(font, &L);
    ScreenReaderMenu     scrRMenu(font);

    ScreenSearchBook     scrSearch(font, &L);
    ScreenViewBook       scrViewBook(font, &L);
    ScreenBorrowBook     scrBorrow(font, &L, &currentReader);
    ScreenReturnBook     scrReturn(font, &L, &currentReader);
    ScreenRateBook       scrRate(font, &L, &currentReader);
    ScreenTopBook        scrTop(font, &L);
    ScreenHistory        scrHis(font, &L, &currentReader);

    ScreenLibLogin       scrLLogin(font, &L, &currentUser);
    ScreenLibMenu        scrLMenu(font);
    ScreenLibAdd         scrAdd(font, &L);
    ScreenLibDelete      scrDel(font, &L);
    ScreenLibUpdate      scrUpd(font, &L);

    ScreenViewReader     scrViewReader(font, &L);

    // ==== App state ====
    AppState current = SCREEN_WELCOME;
    ScreenBase *screen = &scrWelcome;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            // ====== Xác định màn hình hiện tại ======
            switch (current) {
            case SCREEN_WELCOME:         screen = &scrWelcome; break;
            case SCREEN_ROLE:            screen = &scrRole; break;

            case SCREEN_READER_LOGIN:    screen = &scrRLogin; break;
            case SCREEN_READER_REGISTER: screen = &scrRReg; break;
            case SCREEN_READER_MENU:     screen = &scrRMenu; break;

            case SCREEN_SEARCH_BOOK:     screen = &scrSearch; break;
            case SCREEN_VIEW_BOOK:       screen = &scrViewBook; break;
            case SCREEN_BORROW_BOOK:     screen = &scrBorrow; break;
            case SCREEN_RETURN_BOOK:     screen = &scrReturn; break;
            case SCREEN_RATE_BOOK:       screen = &scrRate; break;
            case SCREEN_TOP_BOOK:        screen = &scrTop; break;
            case SCREEN_HISTORY:         screen = &scrHis; break;

            case SCREEN_LIB_LOGIN:       screen = &scrLLogin; break;
            case SCREEN_LIB_MENU:        screen = &scrLMenu; break;
            case SCREEN_LIB_ADD:         screen = &scrAdd; break;
            case SCREEN_LIB_DELETE:      screen = &scrDel; break;
            case SCREEN_LIB_UPDATE:      screen = &scrUpd; break;

            case SCREEN_VIEW_READER:     screen = &scrViewReader; break;
            }

            // ==== Gửi event cho màn hình hiện tại ====
            if (screen)
                screen->handleEvent(e, current);
        }

        // ==== Update logic ====
        if (screen) screen->update();

        // ==== Vẽ ====
        window.clear(Theme::BG);
        if (screen) screen->draw(window);
        window.display();
    }

    return 0;
}
