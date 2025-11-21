#ifndef STATE_HPP
#define STATE_HPP

enum class AppState {
    SPLASH,
    HOME,
    CATEGORY,
    SEARCH,
    BOOK_DETAIL,
    
    // Auth
    LOGIN_CHOICE,
    LOGIN_READER,
    LOGIN_LIBRARIAN,
    REGISTER_READER,
    
    // Reader Features
    BORROWED_BOOKS, // Danh sách đang mượn
    HISTORY,        // Lịch sử
    READER_CARD,    // Thẻ bạn đọc
    BORROW_BOOK,    // Màn hình mượn (Popup)
    RETURN_BOOK,    // Màn hình trả (Popup)
    RATING_BOOK,    // Màn hình đánh giá (Popup)

    // Librarian Features
    MANAGE_BOOKS,
    MANAGE_READERS,
    STATISTICS,
    OVERDUE_READERS,
    LIBRARIAN_CARD, // Thẻ thủ thư
    ADD_BOOK,       // Thêm sách (Popup)
    DELETE_BOOK,    // Xóa sách (Popup)
    UPDATE_LIBRARIAN_INFO,
    UPDATE_READER_INFO,

    // Lists
    TOP_BOOKS,
    ALL_BOOKS_LIST
};

#endif