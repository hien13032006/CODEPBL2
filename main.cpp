// ======================= FULL main.cpp (UPDATED CLEAN LOGIN + STATE + BRIGHT THEME) (SFML) =======================
// Không dùng vector, đồng bộ Theme + Background + Popup
// Đã gắn toàn bộ màn: Title, RoleSelect, Login Reader/Librarian,
// Menu Reader/Librarian, BookList, Search, Add, Delete, Edit (chọn+sửa),
// Rate, Borrow/Return.

#include <SFML/Graphics.hpp>
#include <iostream>

// ==== Core UI helpers ====
#include "Theme.h"
#include "Background.h"
#include "Button.h"
#include "Popup.h"

// ==== Login screens (theme-based) ====
#include "ScreenLoginReader.h"
#include "ScreenLoginLibrarian.h"

// ==== Menus (đẹp, card panel) ====
#include "ScreenMenus.h"  // chứa ScreenMenuReader / ScreenMenuLibrarian

// ==== Data structures & loaders ====
#include "Node.h"
#include "Book.h"
#include "BookOps.h"
#include "ReaderLoader.h"
#include "ReaderLogin.h"
#include "LibrarianLoader.h"
#include "LibrarianLogin.h"

// ==== Functional book screens ====
#include "ScreenBookList.h"
#include "ScreenSearchBook.h"
#include "ScreenAddBook.h"
#include "ScreenDeleteBook.h"
#include "ScreenChooseEditBook.h"
#include "ScreenEditBook.h"
#include "ScreenRateBook.h"
#include "ScreenBorrowReturn.h"

// ================== Ứng dụng ==================

enum class AppState {
    Title,
    RoleSelect,
    LoginReader,
    LoginLibrarian,
    MenuReader,
    MenuLibrarian,
    BookList,
    BookSearch,
    AddBook,
    DeleteBook,
    EditBookChoose,
    EditBook,
    RateBook,
    BorrowReturn
};

// ----- Data heads -----
static NodeBook*      g_books    = nullptr;
static NodeReader*    g_readers  = nullptr;
static NodeLibrarian* g_libs     = nullptr;

// ----- Background & theme -----
static Background g_bg;
static sf::Font g_font;       // dùng cho title/labels nhỏ tại Title/Role

// ----- Title / Role UI -----
static sf::Text g_titleText, g_roleText;
static Button   g_btnStart, g_btnExit;
static Button   g_btnRoleReader, g_btnRoleLibrarian;

// ----- Menus -----
static ScreenMenuReader    g_menuReader;
static ScreenMenuLibrarian g_menuLib;

// ----- Login Screens -----
static ScreenLoginReader*    g_loginReader = nullptr;
static ScreenLoginLibrarian* g_loginLib    = nullptr;

// ----- Book Screens -----
static ScreenBookList*       g_scList     = nullptr;
static ScreenSearchBook*     g_scSearch   = nullptr;
static ScreenAddBook*        g_scAdd      = nullptr;
static ScreenDeleteBook*     g_scDel      = nullptr;
static ScreenChooseEditBook* g_scChooseEd = nullptr;
static ScreenEditBook*       g_scEdit     = nullptr;
static ScreenRateBook*       g_scRate     = nullptr;
static ScreenBorrowReturn*   g_scBR       = nullptr;

// ----- Popup toàn cục (nếu cần) -----
static Popup g_popup;

// ================== Khởi tạo UI đơn giản ==================
static void initTitleUI() {
    g_titleText.setFont(g_font);
    g_titleText.setString("THU VIEN BACH KHOA");
    g_titleText.setCharacterSize(46);
    g_titleText.setFillColor(Theme::Title);
    g_titleText.setStyle(sf::Text::Bold);
    g_titleText.setPosition(0, 110);
}

static void initTitleButtons(std::function<void(AppState)> setState) {
    g_btnStart = Button(g_font, "START", 520, 300);
    g_btnExit  = Button(g_font, "EXIT",  520, 370);
    g_btnStart.setCallback([&](){ setState(AppState::RoleSelect); });
    g_btnExit.setCallback ([](){ std::exit(0); });
}

static void initRoleUI(std::function<void(AppState)> setState) {
    g_roleText.setFont(g_font);
    g_roleText.setString("CHON PHAN QUYEN");
    g_roleText.setCharacterSize(34);
    g_roleText.setFillColor(Theme::Title);
    g_roleText.setStyle(sf::Text::Bold);
    g_roleText.setPosition(0, 80);

    g_btnRoleReader    = Button(g_font, "READER",  520, 220);
    g_btnRoleLibrarian = Button(g_font, "THU THU", 520, 290);

    g_btnRoleReader.setCallback([&](){    setState(AppState::LoginReader);    });
    g_btnRoleLibrarian.setCallback([&](){ setState(AppState::LoginLibrarian); });
}

// ================== Main ==================
int main() {
    sf::RenderWindow win(sf::VideoMode(1200, 700), "Library SFML");
    win.setFramerateLimit(60);

    // ---- Font + Background ----
    if (!g_font.loadFromFile("assets/DejaVuSans.ttf")) {
        std::cout << "Khong load duoc font assets/DejaVuSans.ttf";
    }
    g_bg.load("assets/bg.jpg");
    g_bg.resizeToCover(win.getSize());

    // ---- Load Data ----
    // Sách: ThemSach.txt (định dạng bạn đã dùng trong Book::docFileInput)
    Sach::docFileInput("ThemSach.txt", g_books);

    // Reader & Librarian (định dạng: id|name|phone|email|user|pass)
    g_readers = loadReaders("Reader.txt");
    g_libs    = loadLibrarians("ThuThu.txt");

    // ---- State ----
    AppState state = AppState::Title;
    auto setState = [&](AppState s){ state = s; };

    // ---- Title/Role ----
    initTitleUI();
    initTitleButtons(setState);
    initRoleUI(setState);

    // ---- Menus ----
    g_menuLib.init(
        [&](){ setState(AppState::BookList); },
        [&](){ setState(AppState::BookSearch); },
        [&](){ setState(AppState::AddBook); },
        [&](){ setState(AppState::DeleteBook); },
        [&](){ setState(AppState::EditBookChoose); },
        [&](){ setState(AppState::RateBook); },
        [&](){ setState(AppState::BorrowReturn); },
        [&](){ setState(AppState::RoleSelect); }
    );

    g_menuReader.init(
        [&](){ setState(AppState::BookList); },
        [&](){ setState(AppState::BookSearch); },
        [&](){ setState(AppState::RateBook); },
        [&](){ setState(AppState::BorrowReturn); },
        [&](){ setState(AppState::RoleSelect); }
    );

    // ---- Login Screens ----
    g_loginReader = new ScreenLoginReader(
        g_readers,
        [&](){ setState(AppState::MenuReader); },
        [&](){ setState(AppState::RoleSelect); }
    );
    g_loginReader->init();

    g_loginLib = new ScreenLoginLibrarian(
        g_libs,
        [&](){ setState(AppState::MenuLibrarian); },
        [&](){ setState(AppState::RoleSelect); }
    );
    g_loginLib->init();

    // ---- Book Screens ----
    g_scList   = new ScreenBookList(  g_books, [&](){ setState(AppState::MenuLibrarian); } );
    g_scSearch = new ScreenSearchBook(g_books, [&](){ setState(AppState::MenuLibrarian); } );
    g_scAdd    = new ScreenAddBook(   g_books, [&](){ setState(AppState::MenuLibrarian); } );
    g_scDel    = new ScreenDeleteBook(g_books, [&](){ setState(AppState::MenuLibrarian); } );
    g_scEdit   = new ScreenEditBook(  g_books, [&](){ setState(AppState::MenuLibrarian); } );

    g_scChooseEd = new ScreenChooseEditBook(
        g_books,
        [&](Sach* s){ g_scEdit->setBook(s); setState(AppState::EditBook); },
        [&](){ setState(AppState::MenuLibrarian); }
    );

    g_scRate = new ScreenRateBook(
        g_books,
        [&](){ setState(AppState::MenuLibrarian); }
    );

    g_scBR = new ScreenBorrowReturn(
        g_books,
        "LichSuMuonTra.txt",
        [&](){ setState(AppState::MenuReader); }
    );

    g_scList->init();   g_scSearch->init(); g_scAdd->init();
    g_scDel->init();    g_scEdit->init();   g_scChooseEd->init();
    g_scRate->init();   g_scBR->init();

    // ================== Vòng lặp chính ==================
    while (win.isOpen()) {
        sf::Event evt;
        while (win.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) win.close();
            if (evt.type == sf::Event::Resized) {
                g_bg.resizeToCover(win.getSize());
            }

            switch (state) {
                case AppState::Title:
                    g_btnStart.handle(evt);
                    g_btnExit.handle(evt);
                    break;

                case AppState::RoleSelect:
                    g_btnRoleReader.handle(evt);
                    g_btnRoleLibrarian.handle(evt);
                    break;

                case AppState::LoginReader:
                    g_loginReader->handle(win, evt);
                    break;

                case AppState::LoginLibrarian:
                    g_loginLib->handle(win, evt);
                    break;

                case AppState::MenuReader:
                    g_menuReader.handle(evt);
                    break;

                case AppState::MenuLibrarian:
                    g_menuLib.handle(evt);
                    break;

                case AppState::BookList:       g_scList->handle(evt);     break;
                case AppState::BookSearch:     g_scSearch->handle(evt);   break;
                case AppState::AddBook:        g_scAdd->handle(evt);      break;
                case AppState::DeleteBook:     g_scDel->handle(evt);      break;
                case AppState::EditBookChoose: g_scChooseEd->handle(evt); break;
                case AppState::EditBook:       g_scEdit->handle(evt);     break;
                case AppState::RateBook:       g_scRate->handle(evt);     break;
                case AppState::BorrowReturn:   g_scBR->handle(evt);       break;
            }
        }

        // -------- update --------
        switch (state) {
            case AppState::Title:
                g_btnStart.update(win);
                g_btnExit.update(win);
                break;

            case AppState::RoleSelect:
                g_btnRoleReader.update(win);
                g_btnRoleLibrarian.update(win);
                break;

            case AppState::LoginReader:    g_loginReader->update(win); break;
            case AppState::LoginLibrarian: g_loginLib->update(win);    break;

            case AppState::MenuReader:     g_menuReader.update(win);   break;
            case AppState::MenuLibrarian:  g_menuLib.update(win);      break;

            case AppState::BookList:       g_scList->update(win);      break;
            case AppState::BookSearch:     g_scSearch->update(win);    break;
            case AppState::AddBook:        g_scAdd->update(win);       break;
            case AppState::DeleteBook:     g_scDel->update(win);       break;
            case AppState::EditBookChoose: g_scChooseEd->update(win);  break;
            case AppState::EditBook:       g_scEdit->update(win);      break;
            case AppState::RateBook:       g_scRate->update(win);      break;
            case AppState::BorrowReturn:   g_scBR->update(win);        break;
        }

        // Popup toàn cục (nếu dùng) cũng có thể update ở đây
        g_popup.update(win);

        // -------- draw --------
        win.clear();
        g_bg.draw(win);

        switch (state) {
            case AppState::Title: {
                Theme::centerTextX(g_titleText, win.getSize().x);
                win.draw(g_titleText);
                g_btnStart.draw(win);
                g_btnExit.draw(win);
            } break;

            case AppState::RoleSelect: {
                Theme::centerTextX(g_roleText, win.getSize().x);
                win.draw(g_roleText);
                g_btnRoleReader.draw(win);
                g_btnRoleLibrarian.draw(win);
            } break;

            case AppState::LoginReader:    g_loginReader->draw(win);    break;
            case AppState::LoginLibrarian: g_loginLib->draw(win);       break;
            case AppState::MenuReader:     g_menuReader.draw(win, (float)win.getSize().x);  break;
            case AppState::MenuLibrarian:  g_menuLib.draw(win, (float)win.getSize().x);     break;
            case AppState::BookList:       g_scList->draw(win);         break;
            case AppState::BookSearch:     g_scSearch->draw(win);       break;
            case AppState::AddBook:        g_scAdd->draw(win);          break;
            case AppState::DeleteBook:     g_scDel->draw(win);          break;
            case AppState::EditBookChoose: g_scChooseEd->draw(win);     break;
            case AppState::EditBook:       g_scEdit->draw(win);         break;
            case AppState::RateBook:       g_scRate->draw(win);         break;
            case AppState::BorrowReturn:   g_scBR->draw(win);           break;
        }

        // Vẽ popup toàn cục nếu muốn
        g_popup.draw(win);

        win.display();
    }

    return 0;
}
