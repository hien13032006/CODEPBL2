#ifndef APPSTATE_H
#define APPSTATE_H

enum AppState {

    // Intro
    SCREEN_WELCOME,
    SCREEN_ROLE,

    // Reader login/choice
    SCREEN_READER_CHOICE,
    SCREEN_READER_LOGIN,
    SCREEN_READER_REGISTER,

    // Librarian login
    SCREEN_LIB_LOGIN,

    // MENU
    SCREEN_READER_MENU,
    SCREEN_LIB_MENU,

    // Reader functions
    SCREEN_VIEW_BOOK,
    SCREEN_SEARCH_BOOK,
    SCREEN_BORROW_BOOK,
    SCREEN_RETURN_BOOK,
    SCREEN_RATE_BOOK,
    SCREEN_TOP_BOOK,
    SCREEN_HISTORY,

    // Librarian functions
    SCREEN_LIB_ADD,
    SCREEN_LIB_DELETE,
    SCREEN_LIB_UPDATE,
    SCREEN_VIEW_READER,
    
};

#endif