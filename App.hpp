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
#include "LibrarySystem.h"
#include "Reader.h"
#include "Librarian.h"
#include "BorrowedBooksScreen.hpp"
#include "HistoryScreen.hpp"
#include "ManageBooksScreen.hpp"
#include "ManageReadersScreen.hpp"
#include "StatisticsScreen.hpp"
#include "SearchScreen.hpp"
#include "OverdueReadersScreen.hpp"

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
    BorrowedBooksScreen* borrowedBooksScreen;
    HistoryScreen* historyScreen;
    ManageBooksScreen* manageBooksScreen;
    ManageReadersScreen* manageReadersScreen;
    StatisticsScreen* statisticsScreen;
    SearchScreen* searchScreen;
    OverdueReadersScreen* overdueReadersScreen;
    
    // UI Components
    Modal* modal;
    Modal* detailModal;

public:
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
        delete loginLibrarianScreen;
        delete loginReaderScreen;
        delete registerReaderScreen;
        delete modal;
        delete detailModal;
        delete libSystem;
        delete borrowedBooksScreen;
        delete historyScreen;
        delete manageBooksScreen;
        delete manageReadersScreen;
        delete statisticsScreen;
        delete searchScreen;
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
    libSystem->DocTatCaDanhSachMuon(); // Đọc tất cả phiếu mượn
    libSystem->BuildHashTable(); // Build hash table cho tìm kiếm
    
    std::cout << "====================================\n";
    std::cout << "   HE THONG QUAN LY THU VIEN\n";
    std::cout << "====================================\n";
    std::cout << "Da nap du lieu tu file thanh cong!\n";
    std::cout << "====================================\n";
}

    void initScreens() {
    splashScreen = new SplashScreen(mainFont);
    homeScreen = new HomeScreen(mainFont, libSystem);
    categoryScreen = new CategoryScreen(mainFont, libSystem);
    searchScreen = new SearchScreen(mainFont, libSystem);
    
    modal = new Modal(mainFont);
    detailModal = new Modal(mainFont);
    
    bookDetailScreen = new BookDetailScreen(mainFont, detailModal);
    loginChoiceScreen = new LoginChoiceScreen(mainFont, modal);
    loginLibrarianScreen = new LoginLibrarianScreen(mainFont, modal);
    loginReaderScreen = new LoginReaderScreen(mainFont, modal);
    registerReaderScreen = new RegisterReaderScreen(mainFont, modal);
    
    // Khởi tạo với nullptr, sẽ tạo lại khi đăng nhập
    borrowedBooksScreen = new BorrowedBooksScreen(mainFont, nullptr);
    historyScreen = new HistoryScreen(mainFont, nullptr);
    
    manageBooksScreen = new ManageBooksScreen(mainFont, libSystem);
    manageReadersScreen = new ManageReadersScreen(mainFont, libSystem);
    statisticsScreen = new StatisticsScreen(mainFont, libSystem);
    overdueReadersScreen = new OverdueReadersScreen(mainFont, libSystem);
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
                        int menuId = homeScreen->handleSidebarClick(mousePos);
                        std::string menuLabel = homeScreen->getSidebar()->getMenuLabel(menuId);
                        
                        if (menuLabel == "Home") {
                            changeState(AppState::HOME);
                        } else if (menuLabel == "The loai") {
                            changeState(AppState::CATEGORY);
                        } else if (menuLabel == "Tim kiem") {
                            changeState(AppState::SEARCH);
                        } else if (menuLabel == "Thanh vien" && currentUserRole == UserRole::NONE) {
                            modal->show();
                            changeState(AppState::LOGIN_CHOICE);
                        } else if (menuLabel == "Sach dang muon" && currentUserRole == UserRole::READER) {
                            changeState(AppState::BORROWED_BOOKS);
                        } else if (menuLabel == "Lich su" && currentUserRole == UserRole::READER) {
                            changeState(AppState::HISTORY);
                        } else if (menuLabel == "Quan ly sach" && currentUserRole == UserRole::LIBRARIAN) {
                            changeState(AppState::MANAGE_BOOKS);
                        } else if (menuLabel == "Quan ly doc gia" && currentUserRole == UserRole::LIBRARIAN) {
                            changeState(AppState::MANAGE_READERS);
                        } else if (menuLabel == "Thong ke" && currentUserRole == UserRole::LIBRARIAN) {
                            changeState(AppState::STATISTICS);
                        } else if (menuLabel == "Dang xuat") {
                            handleLogout();
                        }
                        
                        std::string bookId = homeScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            detailModal->show();
                            changeState(AppState::BOOK_DETAIL);
                        }
                    }
                    break;

                case AppState::BORROWED_BOOKS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = borrowedBooksScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, borrowedBooksScreen->getSidebar());
                        
                        std::string bookId = borrowedBooksScreen->handleReturnClick(mousePos);
                        if (!bookId.empty()) {
                            handleReturnBook(bookId);
                        }
                    }
                    break;

                case AppState::HISTORY:
                    historyScreen->handleScrollEvent(event);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = historyScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, historyScreen->getSidebar());
                    }
                    break;

                case AppState::MANAGE_BOOKS:
                    manageBooksScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageBooksScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, manageBooksScreen->getSidebar());
                        
                        int action = manageBooksScreen->handleClick(mousePos);
                        if (action == 1) {
                            manageBooksScreen->handleAddBook();
                        } else if (action == 3) {
                            changeState(AppState::HOME);
                        } else if (action == 4) {
                            std::string bookId = manageBooksScreen->getSearchText();
                            if (!bookId.empty()) {
                                // Tìm và hiển thị sách để cập nhật/xóa
                            }
                        }
                    }
                    break;

                case AppState::MANAGE_READERS:
                    manageReadersScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageReadersScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, manageReadersScreen->getSidebar());
                        manageReadersScreen->handleClick(mousePos, mainFont);
                    }
                    break;

                case AppState::STATISTICS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = statisticsScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, statisticsScreen->getSidebar());
                    }
                    break;

                case AppState::SEARCH:
                    searchScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = searchScreen->handleSidebarClick(mousePos);
                        handleSidebarNavigation(menuId, searchScreen->getSidebar());
                        
                        searchScreen->handleSearchClick(mousePos, mainFont);
                        
                        std::string bookId = searchScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
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
                                handleReaderRegister();
                            } else if (action == 2) {
                                changeState(AppState::LOGIN_READER);
                            } else if (action == 3) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;
                   case AppState::OVERDUE_READERS:
                        overdueReadersScreen->handleScrollEvent(event);
                        if (event.type == sf::Event::MouseButtonPressed) {
                            int menuId = overdueReadersScreen->handleSidebarClick(mousePos);
                            handleSidebarNavigation(menuId, overdueReadersScreen->getSidebar());
                        }
                        break; 

                default:
                    break;
            }
        }
    }

    void handleLibrarianLogin() {
    std::string username = loginLibrarianScreen->getUsername();
    std::string password = loginLibrarianScreen->getPassword();
    
    USER* tempUser = nullptr;
    if (libSystem->DangNhapThuThu(username, password, tempUser)) {
        currentUser = tempUser;
        currentLibrarian = dynamic_cast<Librarian*>(tempUser);
        currentUserRole = UserRole::LIBRARIAN;
        
        // Cập nhật sidebar cho tất cả screens
        homeScreen->setUserRole(currentUserRole, mainFont);
        categoryScreen->setUserRole(currentUserRole, mainFont);
        searchScreen->setUserRole(currentUserRole, mainFont);
        manageBooksScreen->setUserRole(currentUserRole, mainFont);
        manageReadersScreen->setUserRole(currentUserRole, mainFont);
        statisticsScreen->setUserRole(currentUserRole, mainFont);
        overdueReadersScreen->setUserRole(currentUserRole, mainFont);
        
        std::cout << "Chao mung Thu Thu: " << currentLibrarian->getHoTen() << std::endl;
        
        loginLibrarianScreen->clearFields();
        modal->hide();
        changeState(AppState::HOME);
    } else {
        std::cout << "Sai username hoac password!" << std::endl;
    }
}

    void handleReaderLogin() {
    std::string username = loginReaderScreen->getUsername();
    std::string password = loginReaderScreen->getPassword();
    
    // Tìm reader trong danh sách
    NodeReader* current = libSystem->getDanhSachDocGia();
    bool found = false;
    
    while (current != nullptr) {
        if (current->data->getUsername() == username && 
            current->data->getPassword() == password) {
            currentUser = current->data;
            currentReader = current->data;
            currentUserRole = UserRole::READER;
            found = true;
            
            // Đọc danh sách mượn của độc giả này
            libSystem->DocDanhSachMuonCuaDocGia(currentReader);
            
            // Cập nhật sidebar
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            searchScreen->setUserRole(currentUserRole, mainFont);
            
            // Reload borrowed books screen
            delete borrowedBooksScreen;
            borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
            borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        
        // Reload home screen để cập nhật số lượng
        homeScreen->loadBooks(mainFont);
        
        detailModal->hide();
        changeState(AppState::HOME);
    }
}
    }

    void handleReaderRegister() {
    std::string name = registerReaderScreen->getName();
    std::string phone = registerReaderScreen->getPhone();
    std::string email = registerReaderScreen->getEmail();
    std::string username = registerReaderScreen->getUsername();
    std::string password = registerReaderScreen->getPassword();
    std::string confirmPass = registerReaderScreen->getConfirmPassword();
    
    // Validation
    if (name.empty() || phone.empty() || email.empty() || 
        username.empty() || password.empty()) {
        std::cout << "Vui long dien day du thong tin!" << std::endl;
        return;
    }
    
    if (password != confirmPass) {
        std::cout << "Mat khau khong khop!" << std::endl;
        return;
    }
    
    // Kiểm tra username đã tồn tại
    if (libSystem->KiemTraDocGiaDaDangKy(username)) {
        std::cout << "Username da ton tai!" << std::endl;
        return;
    }
    
    // Tạo Reader mới
    Reader* newReader = new Reader();
    newReader->SignUp(name, phone, email, username, password);
    
    // Ghi vào file
    std::ofstream out("DocGia.txt", std::ios::app);
    if (out.is_open()) {
        out << newReader->toCSV() << std::endl;
        out.close();
    }
    
    // Reload danh sách độc giả
    libSystem->DocFileDocGia();
    
    std::cout << "Dang ky thanh cong! Ma doc gia: " << newReader->getMaID() << std::endl;
    registerReaderScreen->clearFields();
    changeState(AppState::LOGIN_READER);
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
        if (currentReader && bookDetailScreen->getCurrentBook()) {
            Sach* book = bookDetailScreen->getCurrentBook();
            libSystem->MuonSach(currentReader, book->getMaSach());
            
            detailModal->hide();
            changeState(AppState::HOME);
        }
    }

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
    void handleSidebarNavigation(int menuId, Sidebar* sidebar) {
    std::string menuLabel = sidebar->getMenuLabel(menuId);
    
    if (menuLabel == "Home") {
        changeState(AppState::HOME);
    } else if (menuLabel == "The loai") {
        changeState(AppState::CATEGORY);
    } else if (menuLabel == "Tim kiem") {
        changeState(AppState::SEARCH);
    } else if (menuLabel == "Sach dang muon") {
        // Reload trước khi hiển thị
        if (currentReader) {
            libSystem->DocDanhSachMuonCuaDocGia(currentReader);
            delete borrowedBooksScreen;
            borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
            borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        }
        changeState(AppState::BORROWED_BOOKS);
    } else if (menuLabel == "Lich su") {
        changeState(AppState::HISTORY);
    } else if (menuLabel == "Quan ly sach") {
        changeState(AppState::MANAGE_BOOKS);
    } else if (menuLabel == "Quan ly doc gia") {
        // Reload danh sách độc giả
        manageReadersScreen->loadReaders(mainFont);
        changeState(AppState::MANAGE_READERS);
    } else if (menuLabel == "Doc gia qua han") {
        // Reload danh sách độc giả quá hạn
        overdueReadersScreen->loadOverdueReaders(mainFont);
        changeState(AppState::OVERDUE_READERS);
    } else if (menuLabel == "Thong ke") {
        // Reload thống kê
        statisticsScreen->loadStatistics();
        changeState(AppState::STATISTICS);
    } else if (menuLabel == "Dang xuat") {
        handleLogout();
    }
}

        void handleReturnBook(const std::string& bookId) {
    if (currentReader) {
        // Gọi hàm trả sách từ LibrarySystem
        libSystem->TraSach(currentReader, bookId);
        
        // Reload borrowed books screen
        delete borrowedBooksScreen;
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
        borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        
        // Reload home screen
        homeScreen->loadBooks(mainFont);
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
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->update(mousePos);
                break;
            case AppState::HISTORY:
                historyScreen->update(mousePos);
                break;
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->update(mousePos);
                break;
            case AppState::MANAGE_READERS:
                manageReadersScreen->update(mousePos);
                break;
            case AppState::STATISTICS:
                statisticsScreen->update(mousePos);
                break;
            case AppState::SEARCH:
                searchScreen->update(mousePos);
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
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->update(mousePos);
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
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->render(window);
                break;
            case AppState::HISTORY:
                historyScreen->render(window);
                break;
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->render(window);
                break;
            case AppState::MANAGE_READERS:
                manageReadersScreen->render(window);
                break;
            case AppState::STATISTICS:
                statisticsScreen->render(window);
                break;
            case AppState::SEARCH:
                searchScreen->render(window);
                break;
            case AppState::BOOK_DETAIL:
                homeScreen->render(window);
                detailModal->draw(window);
                bookDetailScreen->render(window);
                break;
            case AppState::LOGIN_CHOICE:
                homeScreen->render(window);
                modal->draw(window);
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
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->render(window);
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