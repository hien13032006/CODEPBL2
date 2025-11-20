#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "State.hpp"
#include "Modal.hpp"
#include "SplashScreen.hpp"
#include "HomeScreen.hpp"
#include "CategoryScreen.hpp"
#include "SearchScreen.hpp"
#include "BookDetailScreen.hpp"
#include "LoginChoiceScreen.hpp"
#include "LoginLibrarianScreen.hpp"
#include "LoginReaderScreen.hpp"
#include "RegisterReaderScreen.hpp"
#include "BorrowedBooksScreen.hpp"
#include "HistoryScreen.hpp"
#include "ManageBooksScreen.hpp"
#include "ManageReadersScreen.hpp"
#include "StatisticsScreen.hpp"
#include "OverdueReadersScreen.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Librarian.h"

class App {
private:
    sf::RenderWindow window;
    AppState currentState;
    AppState previousState; 
    UserRole currentUserRole;
    sf::Font mainFont; 
    
    LibrarySystem* libSystem;
    USER* currentUser;
    Reader* currentReader;
    Librarian* currentLibrarian;
    
    // Screens
    SplashScreen* splashScreen;
    HomeScreen* homeScreen;
    CategoryScreen* categoryScreen;
    SearchScreen* searchScreen;
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
    OverdueReadersScreen* overdueReadersScreen;

    
    Modal* modal;
    Modal* detailModal;

public:
    App() : window(sf::VideoMode(1400, 900), "Quan Ly Thu Vien") {
        window.setFramerateLimit(60);
        currentState = AppState::SPLASH;
        previousState = AppState::SPLASH; 
        currentUserRole = UserRole::NONE;
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        
        std::cout << "Khoi tao ung dung...\n";
        
        initResources();
        initLibrarySystem();
        initScreens();
        
        std::cout << "Khoi tao thanh cong!\n";
    }

    ~App() {
        delete splashScreen;
        delete homeScreen;
        delete categoryScreen;
        delete searchScreen;
        delete bookDetailScreen;
        delete loginChoiceScreen;
        delete loginLibrarianScreen;
        delete loginReaderScreen;
        delete registerReaderScreen;
        delete borrowedBooksScreen;
        delete historyScreen;
        delete manageBooksScreen;
        delete manageReadersScreen;
        delete statisticsScreen;
        delete overdueReadersScreen;
        delete modal;
        delete detailModal;
        delete libSystem;
    }

    void initResources() {
        if (!mainFont.loadFromFile("NotoSerif-Regular.ttf")) {
            std::cerr << "Loi: Khong the load font!" << std::endl;
        }
    }

    void initLibrarySystem() {
        libSystem = new LibrarySystem();
        
        std::cout << "====================================\n";
        std::cout << "   HE THONG QUAN LY THU VIEN\n";
        std::cout << "====================================\n";
        
        libSystem->DocFileHeThong("DanhSachSach.txt");
        libSystem->DocFileDocGia();
        libSystem->DocTatCaDanhSachMuon();
        libSystem->BuildHashTable();
        
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
        
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, nullptr);
        historyScreen = new HistoryScreen(mainFont, nullptr);
        
        manageBooksScreen = new ManageBooksScreen(mainFont, libSystem);
        manageReadersScreen = new ManageReadersScreen(mainFont, libSystem);
        statisticsScreen = new StatisticsScreen(mainFont, libSystem);
        overdueReadersScreen = new OverdueReadersScreen(mainFont, libSystem);

        // FIX: Gán con trỏ window cho ManageBooksScreen
        manageBooksScreen->setWindow(&window);
    }

    void changeState(AppState newState) {
        if (newState != currentState && 
            currentState != AppState::SPLASH && 
            currentState != AppState::BOOK_DETAIL &&
            currentState != AppState::LOGIN_CHOICE &&
            currentState != AppState::LOGIN_READER &&
            currentState != AppState::LOGIN_LIBRARIAN &&
            currentState != AppState::REGISTER_READER) {
            previousState = currentState;
        }
        currentState = newState;
    }

    Sach* findBookById(const std::string& bookId) {
        if (!libSystem) return nullptr;
        
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            if (current->data->getMaSach() == bookId) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    void handleSidebarNavigation(int menuId, Sidebar* sidebar) {
        std::string menuLabel = sidebar->getMenuLabel(menuId);
        
        std::cout << "Menu clicked: " << menuLabel << std::endl;
        
        if (menuLabel == "Home") {
            changeState(AppState::HOME);
        } 
        else if (menuLabel == "Top 10 sach" || menuLabel == "Tat ca sach") {
            changeState(AppState::HOME);
        }
        else if (menuLabel == "The loai") {
            categoryScreen->loadCategories(mainFont);
            changeState(AppState::CATEGORY);
        } 
        else if (menuLabel == "Tim kiem") {
            changeState(AppState::SEARCH);
        } 
        else if (menuLabel == "Thanh vien" && currentUserRole == UserRole::NONE) {
            modal->show();
            changeState(AppState::LOGIN_CHOICE);
        } 
        else if (menuLabel == "Sach dang muon" && currentUserRole == UserRole::READER) {
            if (currentReader) {
                libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                delete borrowedBooksScreen;
                borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
                borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
            }
            changeState(AppState::BORROWED_BOOKS);
        } 
        else if (menuLabel == "Lich su" && currentUserRole == UserRole::READER) {
            if (currentReader) {
                delete historyScreen;
                historyScreen = new HistoryScreen(mainFont, currentReader);
                historyScreen->setUserRole(currentUserRole, mainFont);
            }
            changeState(AppState::HISTORY);
        } 
        else if (menuLabel == "Quan ly sach" && currentUserRole == UserRole::LIBRARIAN) {
            // FIX: Lỗi App.hpp Ln 711, Col 80: Đã sửa bằng cách đảm bảo getSidebar() tồn tại
            manageBooksScreen->loadBooksTable(mainFont);
            changeState(AppState::MANAGE_BOOKS);
        } 
        else if (menuLabel == "Quan ly doc gia" && currentUserRole == UserRole::LIBRARIAN) {
            manageReadersScreen->loadReaders(mainFont);
            changeState(AppState::MANAGE_READERS);
        } 
        else if (menuLabel == "Doc gia qua han" && currentUserRole == UserRole::LIBRARIAN) {
            overdueReadersScreen->loadOverdueReaders(mainFont);
            changeState(AppState::OVERDUE_READERS);
        } 
        else if (menuLabel == "Thong ke" && currentUserRole == UserRole::LIBRARIAN) {
            statisticsScreen->loadStatistics();
            changeState(AppState::STATISTICS);
        } 
        else if (menuLabel == "Dang xuat") {
            handleLogout();
        }
    }

    void handleLibrarianLogin() {
        std::string username = loginLibrarianScreen->getUsername();
        std::string password = loginLibrarianScreen->getPassword();
        
        if (username.empty() || password.empty()) {
            std::cout << "Loi: Vui long nhap username va password!" << std::endl;
            return;
        }
        
        USER* tempUser = nullptr;
        if (libSystem->DangNhapThuThu(username, password, tempUser)) {
            currentUser = tempUser;
            currentLibrarian = dynamic_cast<Librarian*>(tempUser);
            currentUserRole = UserRole::LIBRARIAN;
            
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            searchScreen->setUserRole(currentUserRole, mainFont);
            manageBooksScreen->setUserRole(currentUserRole, mainFont);
            manageReadersScreen->setUserRole(currentUserRole, mainFont);
            statisticsScreen->setUserRole(currentUserRole, mainFont);
            overdueReadersScreen->setUserRole(currentUserRole, mainFont);
            
            std::cout << "Dang nhap thanh cong! Chao mung Thu Thu: " 
                      << currentLibrarian->getHoTen() << std::endl;
            
            loginLibrarianScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);
        } else {
            std::cout << "Loi: Sai username hoac password!" << std::endl;
        }
    }

    void handleReaderLogin() {
        std::string username = loginReaderScreen->getUsername();
        std::string password = loginReaderScreen->getPassword();
        
        if (username.empty() || password.empty()) {
            std::cout << "Loi: Vui long nhap username va password!" << std::endl;
            return;
        }
        
        NodeReader* current = libSystem->getDanhSachDocGia();
        bool found = false;
        
        while (current != nullptr) {
            if (current->data->getUsername() == username && 
                current->data->getPassword() == password) {
                currentUser = current->data;
                currentReader = current->data;
                currentUserRole = UserRole::READER;
                found = true;
                
                libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                
                homeScreen->setUserRole(currentUserRole, mainFont);
                categoryScreen->setUserRole(currentUserRole, mainFont);
                searchScreen->setUserRole(currentUserRole, mainFont);
                
                delete borrowedBooksScreen;
                borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
                borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
                
                delete historyScreen;
                historyScreen = new HistoryScreen(mainFont, currentReader);
                historyScreen->setUserRole(currentUserRole, mainFont);
                
                std::cout << "Dang nhap thanh cong! Chao mung Doc Gia: " 
                          << currentReader->getHoTen() << std::endl;
                
                loginReaderScreen->clearFields();
                modal->hide();
                changeState(AppState::HOME);
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            std::cout << "Loi: Sai username hoac password!" << std::endl;
        }
    }

    void handleReaderRegister() {
        std::string name = registerReaderScreen->getName();
        std::string phone = registerReaderScreen->getPhone();
        std::string email = registerReaderScreen->getEmail();
        std::string username = registerReaderScreen->getUsername();
        std::string password = registerReaderScreen->getPassword();
        std::string confirmPass = registerReaderScreen->getConfirmPassword();
        
        if (name.empty() || phone.empty() || email.empty() || 
            username.empty() || password.empty()) {
            std::cout << "Loi: Vui long dien day du thong tin!" << std::endl;
            return;
        }
        
        if (password != confirmPass) {
            std::cout << "Loi: Mat khau khong khop!" << std::endl;
            return;
        }
        
        if (password.length() != 8) {
            std::cout << "Loi: Mat khau phai co dung 8 ki tu!" << std::endl;
            return;
        }
        
        if (libSystem->KiemTraDocGiaDaDangKy(username)) {
            std::cout << "Loi: Username da ton tai!" << std::endl;
            return;
        }
        
        Reader* newReader = new Reader();
        newReader->SignUp(name, phone, email, username, password);
        
        std::ofstream out("DocGia.txt", std::ios::app);
        if (out.is_open()) {
            out << newReader->toCSV() << std::endl;
            out.close();
        }
        
        libSystem->DocFileDocGia();
        
        std::cout << "Dang ky thanh cong! Ma doc gia: " << newReader->getMaID() << std::endl;
        
        registerReaderScreen->clearFields();
        changeState(AppState::LOGIN_READER);
    }

    void handleLogout() {
        if (currentUser) {
            std::cout << "Da dang xuat: " << currentUser->getHoTen() << std::endl;
        }
        
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        currentUserRole = UserRole::NONE;
        
        homeScreen->setUserRole(currentUserRole, mainFont);
        categoryScreen->setUserRole(currentUserRole, mainFont);
        searchScreen->setUserRole(currentUserRole, mainFont);
        
        changeState(AppState::HOME);
    }

    void handleBorrowBook() {
        if (!currentReader || !bookDetailScreen->getCurrentBook()) return;
        
        Sach* book = bookDetailScreen->getCurrentBook();
        std::string maSach = book->getMaSach();
        
        if (book->getSoLuong() == 0) {
            std::cout << "Loi: Sach da het!" << std::endl;
            return;
        }
        
        if (currentReader->DaMuonSach(maSach)) {
            std::cout << "Loi: Ban da muon sach nay roi!" << std::endl;
            return;
        }
        
        if (currentReader->DemSachDaMuon() >= currentReader->getGioiHanMuon()) {
            std::cout << "Loi: Ban da muon toi da sach!" << std::endl;
            return;
        }
        
        int soQuaHan = currentReader->DemSachQuaHan();
        if (soQuaHan > 0) {
            std::cout << "Loi: Ban co sach qua han!" << std::endl;
            return;
        }
        
        libSystem->MuonSach(currentReader, maSach);
        
        delete borrowedBooksScreen;
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
        borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        homeScreen->loadBooks(mainFont);
        
        std::cout << "Muon sach thanh cong!" << std::endl;
        
        detailModal->hide();
        changeState(previousState); // FIX: Quay lại màn hình trước đó
    }

    void handleReturnBook(const std::string& bookId) {
        if (!currentReader) return;
        
        NodeMuonSach* phieuNode = currentReader->getDanhSachPhieuMuon();
        bool quaHan = false;
        int soNgayQuaHan = 0;
        std::string tenSach = "";
        
        while (phieuNode != nullptr) {
            if (phieuNode->phieu->sach->getMaSach() == bookId) {
                tenSach = phieuNode->phieu->sach->getTenSach();
                if (phieuNode->phieu->daQuaHan()) {
                    quaHan = true;
                    soNgayQuaHan = abs(phieuNode->phieu->soNgayConLai());
                }
                break;
            }
            phieuNode = phieuNode->next;
        }
        
        if (quaHan) {
            std::cout << "CANH BAO: Sach qua han " << soNgayQuaHan << " ngay!" << std::endl;
        }
        
        libSystem->TraSach(currentReader, bookId);
        
        delete borrowedBooksScreen;
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
        borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        homeScreen->loadBooks(mainFont);
        
        std::cout << "Da tra sach: " << tenSach << std::endl;
    }

    void handleDeleteBook() {
        if (!currentLibrarian || !bookDetailScreen->getCurrentBook()) return;
        
        Sach* book = bookDetailScreen->getCurrentBook();
        std::string maSach = book->getMaSach();
        std::string tenSach = book->getTenSach();
        
        libSystem->XoaSach(maSach);
        libSystem->GhiFileHeThong("DanhSachSach.txt"); // Ghi lại file hệ thống sau khi xóa
        homeScreen->loadBooks(mainFont);
        categoryScreen->loadCategories(mainFont);
        manageBooksScreen->loadBooksTable(mainFont); // Cập nhật lại bảng quản lý
        
        std::cout << "Da xoa sach: " << tenSach << std::endl;
        
        detailModal->hide();
        changeState(previousState); // FIX: Quay lại màn hình trước đó
    }

    void handleUpdateBook() {
        if (!currentLibrarian || !bookDetailScreen->getCurrentBook()) return;

        Sach* bookToUpdate = bookDetailScreen->getCurrentBook();
        
        // 1. Đóng modal chi tiết sách
        detailModal->hide(); 

        // 2. Gán sách cần cập nhật vào ManageBooksScreen
        manageBooksScreen->setBookToEdit(bookToUpdate); 
        
        // 3. Chuyển trạng thái về ManageBooksScreen
        changeState(AppState::MANAGE_BOOKS);
        
        // 4. Mở modal chỉnh sửa
        manageBooksScreen->showUpdateModal(); 
        
        std::cout << "Chuyen sang cap nhat sach: " << bookToUpdate->getMaSach() << std::endl;
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
                    homeScreen->handleScrollEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = homeScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, homeScreen->getSidebar());
                        }
                        
                        std::string bookId = homeScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                previousState = AppState::HOME;
                                // FIX: Cần truyền libSystem vào setBook
                                bookDetailScreen->setBook(book, currentUserRole, libSystem);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::CATEGORY:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = categoryScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, categoryScreen->getSidebar());
                        }
                        
                        if (categoryScreen->handleCategoryButtonClick(mousePos, mainFont)) {
                            break;
                        }
                        categoryScreen->handleClick(mousePos, mainFont);
                        
                        std::string bookId = categoryScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                previousState = AppState::CATEGORY;
                                // FIX: Cần truyền libSystem vào setBook
                                bookDetailScreen->setBook(book, currentUserRole, libSystem);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::SEARCH:
                    searchScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = searchScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, searchScreen->getSidebar());
                        }
                        
                        searchScreen->handleSearchClick(mousePos, mainFont);
                        
                        std::string bookId = searchScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                previousState = AppState::SEARCH;
                                // FIX: Cần truyền libSystem vào setBook
                                bookDetailScreen->setBook(book, currentUserRole, libSystem);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::BOOK_DETAIL:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (detailModal->handleClose(mousePos)) {
                            detailModal->hide();
                            changeState(previousState); 
                        } else {
                            // Modal chi tiết sách quản lý bởi LIBRARIAN không có nút mượn/xóa
                            int action = bookDetailScreen->handleClick(mousePos); 
                            if (action == 0) {
                                detailModal->hide();
                                changeState(previousState); 
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

                case AppState::BORROWED_BOOKS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = borrowedBooksScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, borrowedBooksScreen->getSidebar());
                        }
                        
                        std::string bookId = borrowedBooksScreen->handleReturnClick(mousePos);
                        if (!bookId.empty() && bookId.find("BOOK_ID_") == 0) {
                            // TODO: Get actual book ID
                        }
                    }
                    break;

                case AppState::HISTORY:
                    historyScreen->handleScrollEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = historyScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, historyScreen->getSidebar());
                        }
                    }
                    break;

                case AppState::MANAGE_BOOKS:
                    manageBooksScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageBooksScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, manageBooksScreen->getSidebar());
                        }
                        
                        int action = manageBooksScreen->handleClick(mousePos);
                        if (action == 1) { // Thêm sách
                            manageBooksScreen->handleAddBook();
                            libSystem->BuildHashTable(); 
                            homeScreen->loadBooks(mainFont);
                        } else if (action == 5) { // Cập nhật sách
                            manageBooksScreen->handleUpdateBookLogic(); 
                            libSystem->BuildHashTable(); 
                            homeScreen->loadBooks(mainFont);
                        } else if (action == 7) { // Xem chi tiết sách (Click vào hàng sách)
                            std::string bookId = manageBooksScreen->getClickedBookId(mousePos);
                            if (!bookId.empty()) {
                                Sach* book = findBookById(bookId);
                                if (book) {
                                    previousState = AppState::MANAGE_BOOKS;
                                    // FIX: Cần truyền libSystem vào setBook
                                    bookDetailScreen->setBook(book, currentUserRole, libSystem);
                                    detailModal->show();
                                    changeState(AppState::BOOK_DETAIL);
                                }
                            }
                        } else if (action == 3) { // Xem tất cả (Quay lại HOME)
                             changeState(AppState::HOME);
                        }
                    }
                    break;

                case AppState::MANAGE_READERS:
                    manageReadersScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageReadersScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, manageReadersScreen->getSidebar());
                        }
                        manageReadersScreen->handleClick(mousePos, mainFont);
                    }
                    break;

                case AppState::STATISTICS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = statisticsScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, statisticsScreen->getSidebar());
                        }

                        int clickedStat = statisticsScreen->handleClick(mousePos);

                        if (clickedStat == 1) { // Tổng số sách
                            changeState(AppState::HOME); 
                        } else if (clickedStat == 2) { // Tổng độc giả
                            manageReadersScreen->loadReaders(mainFont);
                            changeState(AppState::MANAGE_READERS);
                        } else if (clickedStat == 4) { // Sách quá hạn
                            overdueReadersScreen->loadOverdueReaders(mainFont);
                            changeState(AppState::OVERDUE_READERS);
                        }
                    }
                    break;

                case AppState::OVERDUE_READERS:
                    overdueReadersScreen->handleScrollEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = overdueReadersScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, overdueReadersScreen->getSidebar());
                        }
                    }
                    break;
            }
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
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->update(mousePos);
                break;
            case AppState::HISTORY:
                historyScreen->update(mousePos);
                break;
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->update();
                break;
            case AppState::MANAGE_READERS:
                manageReadersScreen->update(mousePos);
                break;
            case AppState::STATISTICS:
                statisticsScreen->update(mousePos);
                break;
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->update(mousePos);
                break;
        }
    }

    void render() {
        window.clear(sf::Color(245, 245, 250)); // Light background

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
                
            case AppState::SEARCH:
                searchScreen->render(window);
                break;
                
            case AppState::BOOK_DETAIL:
            { // <-- FIX: Mở scope mới để giải quyết lỗi bypass initialization
                sf::RectangleShape overlay(sf::Vector2f(1400, 900));
                overlay.setFillColor(sf::Color(0, 0, 0, 180));
                window.draw(overlay); 
                detailModal->draw(window);
                bookDetailScreen->render(window);
            } // <-- FIX: Đóng scope
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
                
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->render(window);
                break;
                
            case AppState::HISTORY:
                historyScreen->render(window);
                break;
                
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->render();
                break;
                
            case AppState::MANAGE_READERS:
                manageReadersScreen->render(window);
                break;
                
            case AppState::STATISTICS:
                statisticsScreen->render(window);
                break;
                
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->render(window);
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

#endif // APP_HPP