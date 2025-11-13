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

int main(){
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Library",sf::Style::Fullscreen);
    LibrarySystem L;
    L.DocFileHeThong("DanhSachSach.txt");
    L.DocFileDocGia();
    L.XayDungTatCaCay();

    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("DejaVuSans.ttf");

    Reader *currentReader = nullptr;

    AppState current = SCREEN_WELCOME;

    // Screens
    ScreenWelcome       scrWelcome(font);
    ScreenRole          scrRole(font, window);
    scrWelcome.init(window);
    scrRole.init(window);
    scrRole.onReader = [&current]() { current = SCREEN_READER_CHOICE; };
    scrRole.onLibrarian = [&current]() { current = SCREEN_LIB_LOGIN; };

    ScreenReaderChoice  scrRChoice(font);
    ScreenReaderLogin   scrRLogin(font,&L,&currentReader);
    scrRLogin.init(window);
    ScreenReaderRegister scrRReg(font,&L);
    ScreenReaderMenu    scrRMenu(font,&window);

    ScreenLoginLibrarian scrLibLogin(font,&L);

    ScreenSearchBook    scrSearch(font,&L);
    ScreenViewBook      scrViewBook(font,&L);
    ScreenBorrowBook    scrBorrow(font,&L,&currentReader);
    ScreenReturnBook    scrReturn(font,&L,&currentReader);
    ScreenRateBook      scrRate(font,&L,&currentReader);
    ScreenTopBook       scrTop(font,&L);
    ScreenHistory       scrHist(font,&L,&currentReader);

    ScreenViewReader    scrViewReader(font,&L);
    ScreenLibAdd        scrAdd(font,&L);
    ScreenLibDelete     scrDel(font,&L);
    ScreenLibUpdate     scrUpd(font,&L);

    
    ScreenBase *screen = nullptr;

    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed)
                window.close();

            // chọn màn hiện tại
            switch(current){
            case SCREEN_WELCOME:        screen = &scrWelcome; break;
            case SCREEN_ROLE:           screen = &scrRole; break;

            case SCREEN_READER_CHOICE:  screen = &scrRChoice; break;
            case SCREEN_READER_LOGIN:   screen = &scrRLogin; break;
            case SCREEN_READER_REGISTER:screen = &scrRReg; break;
            case SCREEN_READER_MENU:    screen = &scrRMenu; break;

            case SCREEN_LIB_LOGIN:      screen = &scrLibLogin; break;



            case SCREEN_SEARCH_BOOK:    screen = &scrSearch; break;
            case SCREEN_VIEW_BOOK:      screen = &scrViewBook; break;
            case SCREEN_BORROW_BOOK:    screen = &scrBorrow; break;
            case SCREEN_RETURN_BOOK:    screen = &scrReturn; break;
            case SCREEN_RATE_BOOK:      screen = &scrRate; break;
            case SCREEN_TOP_BOOK:       screen = &scrTop; break;
            case SCREEN_HISTORY:        screen = &scrHist; break;

            case SCREEN_VIEW_READER:    screen = &scrViewReader; break;
            case SCREEN_LIB_ADD:        screen = &scrAdd; break;
            case SCREEN_LIB_DELETE:     screen = &scrDel; break;
            case SCREEN_LIB_UPDATE:     screen = &scrUpd; break;
            }

            if(screen)
                screen->handleEvent(e,current,&window);
        }

        if(screen) screen->update();

        window.clear(Theme::BG);
        if(screen) screen->draw(window);
        window.display();
    }
    return 0;
}
