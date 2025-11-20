#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "State.hpp"
#include "Modal.hpp"
#include "SplashScreen.hpp"
#include "HomeScreen.hpp"
#include "CategoryScreen.hpp"
#include "BookDetailScreen.hpp"
#include "LoginChoiceScreen.hpp"
#include "LoginLibrarianScreen.hpp"
#include "LoginReaderScreen.hpp"
#include "RegisterReaderScreen.hpp"
#include "ScreenReaderCard.hpp"
#include "BorrowBookScreen.hpp"
#include "ReturnBookScreen.hpp"
#include "RatingBookScreen.hpp"
#include "ScreenReaderBorrowedBooks.hpp"
#include "ReaderHistoryScreen.hpp"
#include "ScreenLibrarianCard.hpp"
#include "AddBookScreen.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Librarian.h"

class App {
private:
    sf::RenderWindow window;
    AppState currentState;
    UserRole currentUserRole;
    sf::Font mainFont;
    
    // Library System
    LibrarySystem* libSystem;
    USER* currentUser;
    Reader* currentReader;
    Librarian* currentLibrarian;
    
    // Screens
    SplashScreen* splashScreen;
    HomeScreen* homeScreen;
    CategoryScreen* categoryScreen;
    BookDetailScreen* bookDetailScreen;
    LoginChoiceScreen* loginChoiceScreen;
    LoginLibrarianScreen* loginLibrarianScreen;
    LoginReaderScreen* loginReaderScreen;
    RegisterReaderScreen* registerReaderScreen;
    BorrowBookScreen* borrowBookScreen;
    ReturnBookScreen* returnBookScreen;
    RatingBookScreen* ratingBookScreen;
    ScreenReaderCard *screenReaderCard = nullptr;
    ScreenReaderBorrowedBooks* screenBorrowedBooks = nullptr;
    ReaderHistoryScreen* readerHistoryScreen = nullptr;

    ScreenLibrarianCard* screenLibrarianCard = nullptr;
    AddBookScreen* addBookScreen = nullptr;
    
    // UI Components
    Modal* modal;
    Modal* detailModal;

public:

    std::string trim(const std::string &s) {
        auto start = s.find_first_not_of(" \t\n\r\f\v");
        auto end = s.find_last_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }


    App() : window(sf::VideoMode(1400, 900), "Quan Ly Thu Vien") {
        window.setFramerateLimit(60);
        currentState = AppState::SPLASH;
        currentUserRole = UserRole::NONE;
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        
        initResources();
        initLibrarySystem();
        initScreens();
    }

    ~App() {
        delete splashScreen;
        delete homeScreen;
        delete categoryScreen;
        delete bookDetailScreen;
        delete loginChoiceScreen;

        delete loginReaderScreen;
        delete registerReaderScreen;
        delete screenReaderCard;
        delete borrowBookScreen;
        delete returnBookScreen;
        if (screenReaderCard) delete screenReaderCard;
        if (readerHistoryScreen) delete readerHistoryScreen;

        delete loginLibrarianScreen;
        if (screenLibrarianCard) delete screenLibrarianCard;
        if (addBookScreen) delete addBookScreen;

        delete modal;
        delete detailModal;
        delete libSystem;
        
    }

    void initResources() {
        if (!mainFont.loadFromFile("NotoSerif-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }

    void initLibrarySystem() {
        libSystem = new LibrarySystem();
        
        // Nạp dữ liệu từ file
        libSystem->DocFileHeThong("DanhSachSach.txt");
        libSystem->DocFileDocGia();
        
        std::cout << "Da nap du lieu tu file thanh cong!" << std::endl;
    }

    void initScreens() {
        splashScreen = new SplashScreen(mainFont);
        homeScreen = new HomeScreen(mainFont, libSystem);
        categoryScreen = new CategoryScreen(mainFont, libSystem);
        
        modal = new Modal(mainFont);
        detailModal = new Modal(mainFont);
        
        bookDetailScreen = new BookDetailScreen(mainFont, detailModal);
        loginChoiceScreen = new LoginChoiceScreen(mainFont, modal);
        loginLibrarianScreen = new LoginLibrarianScreen(mainFont, modal);
        loginReaderScreen = new LoginReaderScreen(mainFont, modal);
        registerReaderScreen = new RegisterReaderScreen(mainFont, modal);
        borrowBookScreen = new BorrowBookScreen(mainFont, modal);
        returnBookScreen = new ReturnBookScreen(mainFont, modal);
        ratingBookScreen = new RatingBookScreen(mainFont, modal);

    }

    void changeState(AppState newState) {
        currentState = newState;
    }

    void handleEvents() {
        sf::Event event;
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            switch (currentState) {
                case AppState::SPLASH:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (splashScreen->handleClick(mousePos)) {
                            changeState(AppState::HOME);
                        }
                    }
                    break;

                case AppState::HOME:
                    homeScreen->handleSearchEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        // Xử lý sidebar click
                        int menuId = homeScreen->handleSidebarClick(mousePos);
                        std::string menuLabel = homeScreen->getSidebar()->getMenuLabel(menuId);
                        
                        if (menuLabel == "Thanh vien" && currentUserRole == UserRole::NONE) {
                            modal->show();
                            changeState(AppState::LOGIN_CHOICE);
                        } else if (menuLabel == "The loai") {
                            changeState(AppState::CATEGORY);
                        } else if (menuLabel == "Dang xuat") {
                            handleLogout();
                        } else if (menuLabel == "Muon sach" && currentUserRole == UserRole::READER) {
                            modal->show();
                            changeState(AppState::BORROWED_BOOKS);
                        }
                        else if (menuLabel == "Tra sach" && currentUserRole == UserRole::READER) {
                            modal->show();
                            returnBookScreen->clear();
                            changeState(AppState::RETURN_BOOKS);
                        }
                        else if (menuLabel == "The doc gia" && currentUserRole == UserRole::READER) {
                            modal->show();
                            changeState(AppState::READER_CARD);
                        }
                        else if (menuLabel == "Sach dang muon" && currentUserRole == UserRole::READER) {
                            modal->show();
                            changeState(AppState::READER_BORROWED_BOOKS);
                        }
                        else if (menuLabel == "Lich su muon tra" && currentUserRole == UserRole::READER) {
                            modal->show();
                            changeState(AppState::READER_HISTORY);
                        }
                        else if (menuLabel == "The thu thu" && currentUserRole == UserRole::LIBRARIAN) {
                            modal->show();
                            changeState(AppState::LIBRARIAN_CARD);
                        }
                        else if (menuLabel == "Them sach" && currentUserRole == UserRole::LIBRARIAN) {
                            modal->show();
                            changeState(AppState::ADD_BOOKS);
                        }
                        
                        // Xử lý click card
                        std::string bookId = homeScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            // TODO: Lấy sách từ LibrarySystem
                            detailModal->show();
                            changeState(AppState::BOOK_DETAIL);
                        }
                    }
                    break;

                case AppState::CATEGORY:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = categoryScreen->handleSidebarClick(mousePos);
                        std::string menuLabel = categoryScreen->getSidebar()->getMenuLabel(menuId);
                        
                        if (menuLabel == "Home") {
                            changeState(AppState::HOME);
                        } else if (menuLabel == "Thanh vien" && currentUserRole == UserRole::NONE) {
                            modal->show();
                            changeState(AppState::LOGIN_CHOICE);
                        } else if (menuLabel == "Dang xuat") {
                            handleLogout();
                        }
                        
                        categoryScreen->handleClick(mousePos);
                        
                        std::string bookId = categoryScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            detailModal->show();
                            changeState(AppState::BOOK_DETAIL);
                        }
                    }
                    break;

                case AppState::BOOK_DETAIL:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (detailModal->handleClose(mousePos)) {
                            detailModal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = bookDetailScreen->handleClick(mousePos);
                            if (action == 0) {
                                detailModal->hide();
                                changeState(AppState::HOME);
                            } else if (action == 1) {
                                // Mượn sách
                                handleBorrowBook();
                            } else if (action == 2) {
                                // Xóa sách
                                handleDeleteBook();
                            } else if (action == 3) {
                                // Cập nhật sách
                                handleUpdateBook();
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_CHOICE:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int choice = loginChoiceScreen->handleClick(mousePos);
                            if (choice == 1) {
                                changeState(AppState::LOGIN_LIBRARIAN);
                            } else if (choice == 2) {
                                changeState(AppState::LOGIN_READER);
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_LIBRARIAN:
                    loginLibrarianScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = loginLibrarianScreen->handleClick(mousePos);
                            if (action == 1) {
                                if (loginLibrarianScreen->validate())
                                    handleLibrarianLogin();
                            } else if (action == 2) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_READER:
                    loginReaderScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = loginReaderScreen->handleClick(mousePos);
                            if (action == 1) {
                                if (loginReaderScreen->validate())
                                       handleReaderLogin();
                            } else if (action == 2) {
                                changeState(AppState::REGISTER_READER);
                            } else if (action == 3) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::REGISTER_READER:
                    registerReaderScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = registerReaderScreen->handleClick(mousePos);
                            if (action == 1) {
                                if (registerReaderScreen->validate()) {
                                    handleReaderRegister();
                                }
                            } else if (action == 2) {
                                changeState(AppState::LOGIN_READER);
                            } else if (action == 3) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::READER_CARD:
                    // tạo màn hình thẻ nếu chưa có (tại thời điểm mở, currentReader phải hợp lệ)
                    if (!screenReaderCard) {
                        screenReaderCard = new ScreenReaderCard(mainFont, currentReader);
                    }

                    // chuyển sự kiện cho screen
                    screenReaderCard->handleEvent(event, mousePos);

                    // nếu event là click chuột -> kiểm tra màn hình báo đã đóng
                    if (event.type == sf::Event::MouseButtonPressed) {
                            if (screenReaderCard->wasClosed()) {
                            // đóng và hủy object để tạo lại lần sau nếu cần
                                    delete screenReaderCard;
                                    screenReaderCard = nullptr;
                                    changeState(AppState::HOME);
                            }
                    }
                    break;

                case AppState::BORROWED_BOOKS:
                    borrowBookScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                        // Click vào panel → kiểm tra nút
                            int action = borrowBookScreen->handleClick(mousePos);

                            if (action == 0) {
                                detailModal->hide();
                                changeState(AppState::HOME);
                            } else if (action == 1) {
                                handleBorrowBook(); // gọi hàm mượn
                                modal->show();      // hiển thị thông báo
                                changeState(AppState::BORROWED_BOOKS);
                            } else if (action == 2) { // nhấn Hủy
                                modal->hide();
                                borrowBookScreen->clearInput();
                                changeState(AppState::HOME);
                            }
                        }
                    }
                    break;

                case AppState::RETURN_BOOKS:
                    returnBookScreen->handleEvent(event, mousePos);

                    if (event.type == sf::Event::MouseButtonPressed) {

                        if (modal->handleClose(mousePos)) {
                           modal->hide();
                           returnBookScreen->clear();
                           changeState(AppState::HOME);
                        }

                        int action = returnBookScreen->handleClick(mousePos);

                        if (action == 1) {  
                            handleReturnBook();
                        }
                        else if (action == 2) {
                            modal->hide();
                            returnBookScreen->clear();
                            changeState(AppState::HOME);
                        }
                        else if (action == 3) { // Nhấn "Co" trên askRating
                            ratingBookScreen->setBookID(returnBookScreen->getMaSach());
                            returnBookScreen->hideAskRating();
                            modal->hide();
                            returnBookScreen->clear();
                            changeState(AppState::RATING_BOOK);
                        }
                        else if (action == 4) { // Nhấn "Khong" trên askRating
                            returnBookScreen->hideAskRating();
                            modal->hide();
                            returnBookScreen->clear();
                            changeState(AppState::HOME);
        }
                    }
                    break;

                case AppState::RATING_BOOK:
                    ratingBookScreen->handleEvent(event, mousePos);

                    if (event.type == sf::Event::MouseButtonPressed) {
                        int action = ratingBookScreen->handleClick(mousePos);

                        if (action == 1) { 
                            Sach* s = libSystem->TimSachTheoMa(ratingBookScreen->getBookID());
                            if (s != nullptr) {
                                int diem = ratingBookScreen->getScore();      // nếu có input điểm
                                std::string text = ratingBookScreen->getComment(); // nếu có input comment
                                libSystem->DanhGiaSach(currentReader, s, diem, text);
                            }
                            modal->showMessage("Cam on ban da danh gia!");
                            changeState(AppState::HOME);
                        }
                        else if (action == 2) {
                            changeState(AppState::HOME);
                        }
                    }
                    break;

                case AppState::READER_BORROWED_BOOKS:
                    if (!screenBorrowedBooks) {
                        libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                        screenBorrowedBooks = new ScreenReaderBorrowedBooks(mainFont, currentReader);
                    }
                    screenBorrowedBooks->handleEvent(event, mousePos);
                    if (screenBorrowedBooks->wasClosed()) {
                        delete screenBorrowedBooks;
                        screenBorrowedBooks = nullptr;
                        changeState(AppState::HOME);
                    }
                    break;

                case AppState::READER_HISTORY:
                    if (!readerHistoryScreen && currentReader) {
                        readerHistoryScreen = new ReaderHistoryScreen(mainFont, currentReader);
                    }
                    if (readerHistoryScreen) {
                        sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));
                        readerHistoryScreen->handleEvent(event, mousePosF);

                        if (readerHistoryScreen->wasClosed()) {
                            delete readerHistoryScreen;
                            readerHistoryScreen = nullptr;
                            changeState(AppState::HOME);
                        }
                    }
                    break;

                case AppState::LIBRARIAN_CARD:
                    // tạo màn hình thẻ nếu chưa có (tại thời điểm mở, currentReader phải hợp lệ)
                    if (!screenLibrarianCard) {
                        screenLibrarianCard = new ScreenLibrarianCard(mainFont, currentLibrarian);
                    }

                    // chuyển sự kiện cho screen
                    screenLibrarianCard->handleEvent(event, mousePos);

                    // nếu event là click chuột -> kiểm tra màn hình báo đã đóng
                    if (event.type == sf::Event::MouseButtonPressed) {
                            if (screenLibrarianCard->wasClosed()) {
                            // đóng và hủy object để tạo lại lần sau nếu cần
                                    delete screenLibrarianCard;
                                    screenLibrarianCard = nullptr;
                                    changeState(AppState::HOME);
                            }
                    }
                    break;

                 case AppState::ADD_BOOKS:
                    if (!addBookScreen) {
                        addBookScreen = new AddBookScreen(mainFont, libSystem);
                    }
                    
                    addBookScreen->handleEvent(event, mousePos);

                    // Xử lý nút đóng/thoát nếu addBookScreen có method isClosed/isClosed()
                    if (addBookScreen->isClosed()) {
                        // xóa màn hình để tạo mới khi mở lại
                        delete addBookScreen;
                        addBookScreen = nullptr;

                        // ẩn modal và quay về Home
                        if (modal) modal->hide();
                        changeState(AppState::HOME);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    void handleLibrarianLogin() {
        if (!loginLibrarianScreen->validate()) {
            return;
        }
        std::string username = loginLibrarianScreen->getUsername();
        std::string password = loginLibrarianScreen->getPassword();
        
        USER* tempUser = nullptr;
        if (libSystem->DangNhapThuThu(username, password, tempUser)) {
            currentUser = tempUser;
            currentLibrarian = dynamic_cast<Librarian*>(tempUser);
            currentUserRole = UserRole::LIBRARIAN;
            
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            
            std::cout << "Dang nhap Thu Thu thanh cong!" << std::endl;
            
            loginLibrarianScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);
        } else {
            loginLibrarianScreen->setLoginError("Sai ten dang nhap hoac mat khau!");

            loginLibrarianScreen->startShake();
            std::cout << "Dang nhap that bai!" << std::endl;
        }
    }

    void handleReaderLogin() {
        if (!loginReaderScreen->validate()) {
            return;
        }

        std::string username = loginReaderScreen->getUsername();
        std::string password = loginReaderScreen->getPassword();

        USER* tempUser = nullptr;

        if (libSystem->DangNhapDocGia(username, password, tempUser)) {

            currentUser = tempUser;
            currentReader = dynamic_cast<Reader*>(tempUser);
            currentUserRole = UserRole::READER;

            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);

            std::cout << "Dang nhap thanh cong!" << std::endl;

            if (readerHistoryScreen) {
                delete readerHistoryScreen;
            }
            readerHistoryScreen = new ReaderHistoryScreen(mainFont, currentReader);

            loginReaderScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);

        } else {
            loginReaderScreen->setLoginError("Sai ten dang nhap hoac mat khau!");
            loginReaderScreen->startShake();
            std::cout << "Dang nhap that bai!" << std::endl;
        }
    }


    void handleReaderRegister() {
        std::string name = registerReaderScreen->getName();
        std::string phone = registerReaderScreen->getPhone();
        std::string email = registerReaderScreen->getEmail();
        std::string username = registerReaderScreen->getUsername();
        std::string password = registerReaderScreen->getPassword();
        std::string confirmPass = registerReaderScreen->getConfirmPassword();
    
        if (password != confirmPass) {
            std::cout << "Mat khau khong khop!" << std::endl;
            return;
        }

        bool ok = libSystem->DangKyDocGia(name, phone, email, username, password);

        if (ok) {
            std::cout << "Dang ky thanh cong!" << std::endl;
            registerReaderScreen->clearFields();
            changeState(AppState::LOGIN_READER);
        } else {
            std::cout << "Dang ky that bai!" << std::endl;
        }
    }


    void handleLogout() {
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        currentUserRole = UserRole::NONE;
        
        homeScreen->setUserRole(currentUserRole, mainFont);
        categoryScreen->setUserRole(currentUserRole, mainFont);
        
        std::cout << "Da dang xuat!" << std::endl;
        changeState(AppState::HOME);
    }

    void handleBorrowBook() {
        if (currentReader) {
            std::string maSach = borrowBookScreen->getMaSachInput();
            BorrowResult result = libSystem->MuonSach(currentReader, maSach);

            switch (result.status) {
                case BorrowStatus::SUCCESS:
                    borrowBookScreen->setBorrowMessage("Muon sach thanh cong!");
                    borrowBookScreen->setBorrowedBookInfo(*result.book, result.borrowDate, result.returnDate);
                    modal->show();
                    break;
                case BorrowStatus::OUT_OF_STOCK:
                    borrowBookScreen->setBorrowMessage("Sach da het!");
                    borrowBookScreen->startShake();
                    break;
                case BorrowStatus::ALREADY_BORROWED:
                    borrowBookScreen->setBorrowMessage("Ban da muon sach nay roi!");
                    borrowBookScreen->startShake();
                    break;
                case BorrowStatus::LIMIT_REACHED:
                    borrowBookScreen->setBorrowMessage("Da vuot gioi han muon sach!");
                    borrowBookScreen->startShake();
                    break;
                case BorrowStatus::OVERDUE:
                    borrowBookScreen->setBorrowMessage("Ban con sach qua han!");
                    borrowBookScreen->startShake();
                    break;
                case BorrowStatus::NOT_FOUND:
                    borrowBookScreen->setBorrowMessage("Khong tim thay sach!");
                    borrowBookScreen->startShake();
                    break;
            }
        }
    }

    void handleReturnBook() {
        if (!currentReader) {
            returnBookScreen->setMessage("Ban chua dang nhap!");
            return;
        }

        bool quaHan;
        int soNgayQuaHan;

        // Lấy mã sách và trim
        std::string maSach = trim(returnBookScreen->getMaSach());

        Sach* s = libSystem->TraSach(currentReader, maSach, quaHan, soNgayQuaHan);
        if (!s) {
            returnBookScreen->setMessage("Khong the tra sach! Kiem tra ma sach.");
            return;
        }

        // Cập nhật thông tin sách hiện tại trong màn hình trả sách
        returnBookScreen->setCurrentBook(s);

        // Nếu quá hạn → hiển thị cảnh báo
        if (quaHan) {
            returnBookScreen->setReturnMessage(
                "CANH BAO: Sach tra qua han " + std::to_string(soNgayQuaHan) + " ngay!"
            );
        } else {
            returnBookScreen->setReturnMessage("Tra sach thanh cong!");
        }

        // Ẩn các input + nút trên ReturnBookScreen, chỉ hiển thị panel hỏi đánh giá
        returnBookScreen->showAskRating();

        // Hiển thị modal hỏi đánh giá
        modal->show();

        // Xử lý sự kiện khi người dùng chọn "Có" hoặc "Không"
        modal->onOptionSelect = [&](int option){
            modal->hide();
            returnBookScreen->hideAskRating();  // ẩn modal trong màn hình trả sách
            returnBookScreen->clear();
            if (option == 1) { // "Có" → chuyển sang màn hình đánh giá
                ratingBookScreen->setBookID(maSach);
                changeState(AppState::RATING_BOOK);
            } else { // "Không" → đóng màn hình trả sách và modal, về HOME
                changeState(AppState::HOME);
            }
        };
    };


    void handleDeleteBook() {
        if (currentLibrarian && bookDetailScreen->getCurrentBook()) {
            Sach* book = bookDetailScreen->getCurrentBook();
            libSystem->XoaSach(book->getMaSach());
            
            detailModal->hide();
            changeState(AppState::HOME);
        }
    }

    void handleUpdateBook() {
        if (currentLibrarian) {
            libSystem->CapNhatThongTinSach();
            
            detailModal->hide();
            changeState(AppState::HOME);
        }
    }

    void update() {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        switch (currentState) {
            case AppState::SPLASH:
                splashScreen->update(mousePos);
                break;
            case AppState::HOME:
                homeScreen->update(mousePos);
                break;
            case AppState::CATEGORY:
                categoryScreen->update(mousePos);
                break;
            case AppState::BOOK_DETAIL:
                bookDetailScreen->update(mousePos);
                break;
            case AppState::LOGIN_CHOICE:
                loginChoiceScreen->update(mousePos);
                break;
            case AppState::LOGIN_LIBRARIAN:
                loginLibrarianScreen->update(mousePos);
                break;
            case AppState::LOGIN_READER:
                loginReaderScreen->update(mousePos);
                break;
            case AppState::REGISTER_READER:
                registerReaderScreen->update(mousePos);
                break;
            case AppState::BORROWED_BOOKS:
                borrowBookScreen->update(mousePos);
                break;
            case AppState::RETURN_BOOKS:
                returnBookScreen->update(mousePos);
                break;
            case AppState::RATING_BOOK:
                ratingBookScreen->update(mousePos);
                break;
            default:
                break;
        }
    }

    void render() {
        window.clear();

        switch (currentState) {
            case AppState::SPLASH:
                splashScreen->render(window);
                break;
                
            case AppState::HOME:
                homeScreen->render(window);
                break;
                
            case AppState::CATEGORY:
                categoryScreen->render(window);
                break;
                
            case AppState::BOOK_DETAIL:
                homeScreen->render(window);
                detailModal->draw(window);
                bookDetailScreen->render(window);
                break;
                
            case AppState::LOGIN_CHOICE:
                homeScreen->render(window);
                loginChoiceScreen->render(window);
                break;
                
            case AppState::LOGIN_LIBRARIAN:
                homeScreen->render(window);
                modal->draw(window);
                loginLibrarianScreen->render(window);
                break;
                
            case AppState::LOGIN_READER:
                homeScreen->render(window);
                modal->draw(window);
                loginReaderScreen->render(window);
                break;
                
            case AppState::REGISTER_READER:
                homeScreen->render(window);
                modal->draw(window);
                registerReaderScreen->render(window);
                break;

            case AppState::BORROWED_BOOKS:
                homeScreen->render(window);
                modal->draw(window);
                borrowBookScreen->render(window);
                break;
            case AppState::RETURN_BOOKS:
                homeScreen->render(window);
                modal->draw(window);
                returnBookScreen->render(window);
                break;
            case AppState::RATING_BOOK:
                homeScreen->render(window);
                modal->draw(window);
                ratingBookScreen->render(window);
                break;
            case AppState::READER_CARD:
                homeScreen->render(window);   // nếu muốn nền vẫn hiển home phía sau
                if (screenReaderCard) screenReaderCard->render(window);
                break;
            case AppState::READER_BORROWED_BOOKS:
                homeScreen->render(window);
                if (screenBorrowedBooks) screenBorrowedBooks->render(window);
                break;
            case AppState::READER_HISTORY:
                homeScreen->render(window);
                if (readerHistoryScreen) readerHistoryScreen->render(window);
                break;
            case AppState::LIBRARIAN_CARD:
                homeScreen->render(window);   // nếu muốn nền vẫn hiển home phía sau
                if (screenLibrarianCard) screenLibrarianCard->render(window);
                break;
            case AppState::ADD_BOOKS:
                homeScreen->render(window);   // giữ nền Home phía sau (như các modal khác)
                if (addBookScreen) addBookScreen->render(window);
                break;
                
            default:
                break;
        }

        window.display();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};

#endif