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
            
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            
            std::cout << "Dang nhap Thu Thu thanh cong!" << std::endl;
            
            loginLibrarianScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);
        } else {
            std::cout << "Dang nhap that bai!" << std::endl;
        }
    }

    void handleReaderLogin() {
        std::string username = loginReaderScreen->getUsername();
        std::string password = loginReaderScreen->getPassword();
        
        USER* tempUser = nullptr;
        if (libSystem->DangNhapDocGia(tempUser)) {
            // TODO: Xác thực username/password đúng
            currentUser = tempUser;
            currentReader = dynamic_cast<Reader*>(tempUser);
            currentUserRole = UserRole::READER;
            
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            
            std::cout << "Dang nhap Doc Gia thanh cong!" << std::endl;
            
            loginReaderScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);
        } else {
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
        
        // Đăng ký mới
        libSystem->DangKyDocGia();
        
        std::cout << "Dang ky thanh cong!" << std::endl;
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