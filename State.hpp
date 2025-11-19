#ifndef STATE_HPP
#define STATE_HPP

enum class AppState {
    SPLASH,
    HOME,
    CATEGORY,
    SEARCH,
    BOOK_DETAIL,
    LOGIN_CHOICE,
    LOGIN_READER,
    LOGIN_LIBRARIAN,
    REGISTER_READER,
    BORROWED_BOOKS,
    HISTORY,
    MANAGE_BOOKS,
    MANAGE_READERS,
    STATISTICS,
    OVERDUE_READERS  // Màn hình độc giả quá hạn
};

#endif