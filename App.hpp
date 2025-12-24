#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

// Core & Theme
#include "State.hpp"
#include "Theme.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Librarian.h"
#include "Modal.hpp"
#include "MessageBox.hpp"

// Screens & Popups
#include "SplashScreen.hpp"
#include "HomeScreen.hpp"
#include "CategoryScreen.hpp"
#include "SearchScreen.hpp"
#include "BookDetailScreen.hpp"
#include "LoginChoiceScreen.hpp"
#include "LoginLibrarianScreen.hpp"
#include "LoginReaderScreen.hpp"
#include "RegisterReaderScreen.hpp"
#include "TopBooksScreen.hpp"
#include "AllBooksListScreen.hpp"
#include "BorrowedBooksScreen.hpp" 
#include "ReaderHistoryScreen.hpp"
#include "ManageBooksScreen.hpp"
#include "ManageReadersScreen.hpp"
#include "StatisticsScreen.hpp"
#include "OverdueReadersScreen.hpp"
#include "ScreenLibrarianCard.hpp"
#include "ScreenReaderCard.hpp"
#include "AddBookScreen.hpp"
#include "DeleteBookScreen.hpp"
#include "BorrowBookScreen.hpp"
#include "ReturnBookScreen.hpp"
#include "RatingBookScreen.hpp"
#include "UpdateLibrarianScreen.hpp"
#include "UpdateReaderScreen.hpp"

enum class PopupState { NONE, CONFIRM_RETURN, ASK_TO_RATE, CONFIRM_RETURN_ADMIN };

class App {
private:
    sf::RenderWindow window;
    AppState currentState, previousState; 
    UserRole currentUserRole;
    sf::Font mainFont; 
    LibrarySystem* libSystem;
    USER* currentUser; Reader* currentReader; Librarian* currentLibrarian;
    
    Reader* selectedReaderForAdmin;

    // Screens
    SplashScreen* splashScreen; 
    HomeScreen* homeScreen; 
    CategoryScreen* categoryScreen;
    SearchScreen* searchScreen; 
    TopBooksScreen* topBooksScreen; 
    AllBooksListScreen* allBooksListScreen;
    BookDetailScreen* bookDetailScreen; 
    LoginChoiceScreen* loginChoiceScreen;
    LoginLibrarianScreen* loginLibrarianScreen; 
    LoginReaderScreen* loginReaderScreen;
    RegisterReaderScreen* registerReaderScreen; 
    BorrowedBooksScreen* borrowedBooksScreen;
    ReaderHistoryScreen* readerHistoryScreen; 
    ScreenReaderCard* readerCardScreen;
    ManageBooksScreen* manageBooksScreen; 
    ManageReadersScreen* manageReadersScreen;
    StatisticsScreen* statisticsScreen; 
    OverdueReadersScreen* overdueReadersScreen;
    ScreenLibrarianCard* librarianCardScreen;

    // Popups
    BorrowBookScreen* borrowBookScreen; 
    ReturnBookScreen* returnBookScreen;
    RatingBookScreen* ratingBookScreen; 
    AddBookScreen* addBookScreen;
    DeleteBookScreen* deleteBookScreen;
     UpdateLibrarianScreen* updateLibrarianScreen;
    UpdateReaderScreen* updateReaderScreen;
    Modal *modal, *detailModal;

    MessageBox* messageBox;
    std::string pendingReturnBookID;
    PopupState popupState; 

public:
    App() : window(sf::VideoMode(1300, 720), "Quan Ly Thu Vien - Light Theme", sf::Style::Close | sf::Style::Titlebar) {
        window.setFramerateLimit(60);
        currentState = AppState::SPLASH; 
        previousState = AppState::SPLASH; 
        currentUserRole = UserRole::NONE;
        popupState = PopupState::NONE; 
        selectedReaderForAdmin = nullptr; 
        initResources(); 
        initLibrarySystem(); 
        initScreens();
    }

    ~App() {
        delete splashScreen; 
        delete homeScreen; 
        delete categoryScreen; 
        delete searchScreen;
        delete topBooksScreen; 
        delete allBooksListScreen; 
        delete bookDetailScreen;
        delete loginChoiceScreen; 
        delete loginLibrarianScreen; 
        delete loginReaderScreen;
        delete registerReaderScreen;
        delete borrowedBooksScreen; 
        delete readerHistoryScreen; 
        delete readerCardScreen;
        delete manageBooksScreen; 
        delete manageReadersScreen; 
        delete statisticsScreen; 
        delete overdueReadersScreen; 
        delete librarianCardScreen;
        delete borrowBookScreen; 
        delete returnBookScreen; 
        delete ratingBookScreen;
        delete addBookScreen; 
        delete deleteBookScreen; 
        delete updateLibrarianScreen;
        delete updateReaderScreen;
        delete modal; 
        delete detailModal; 
        delete messageBox; 
        delete libSystem;
    }

    void initResources() {
        if (!mainFont.loadFromFile("NotoSerif-Regular.ttf")) 
            if (!mainFont.loadFromFile("arial.ttf")) std::cerr << "Loi font!\n";
    }

    void initLibrarySystem() {
        libSystem = new LibrarySystem();
        libSystem->DocFileHeThong("DanhSachSach.txt");
        libSystem->DocFileDocGia();
        libSystem->DocTatCaDanhSachMuon();
        libSystem->BuildHashTable();
        libSystem->XepHangSach();
    }

    void initScreens() {
        splashScreen = new SplashScreen(mainFont);
        homeScreen = new HomeScreen(mainFont, libSystem);
        categoryScreen = new CategoryScreen(mainFont, libSystem);
        searchScreen = new SearchScreen(mainFont, libSystem);
        topBooksScreen = new TopBooksScreen(mainFont, libSystem);
        allBooksListScreen = new AllBooksListScreen(mainFont, libSystem);
        
        modal = new Modal(mainFont);
        detailModal = new Modal(mainFont);
        messageBox = new MessageBox(mainFont);
        
        bookDetailScreen = new BookDetailScreen(mainFont, detailModal);
        loginChoiceScreen = new LoginChoiceScreen(mainFont, modal);
        loginLibrarianScreen = new LoginLibrarianScreen(mainFont, modal);
        loginReaderScreen = new LoginReaderScreen(mainFont, modal);
        registerReaderScreen = new RegisterReaderScreen(mainFont, modal);
        
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, nullptr, libSystem);
        readerHistoryScreen = new ReaderHistoryScreen(mainFont, nullptr, libSystem);
        
        manageBooksScreen = new ManageBooksScreen(mainFont, libSystem);
        manageBooksScreen->setWindow(&window);
        
        manageReadersScreen = new ManageReadersScreen(mainFont, libSystem);
        manageReadersScreen->setWindow(&window);
        
        statisticsScreen = new StatisticsScreen(mainFont, libSystem);
        overdueReadersScreen = new OverdueReadersScreen(mainFont, libSystem);
        overdueReadersScreen->setWindow(&window);
        
        addBookScreen = new AddBookScreen(mainFont, libSystem);
        deleteBookScreen = new DeleteBookScreen(mainFont, modal);
        borrowBookScreen = new BorrowBookScreen(mainFont, modal);
        returnBookScreen = new ReturnBookScreen(mainFont, modal);
        ratingBookScreen = new RatingBookScreen(mainFont, modal);
        
        readerCardScreen = nullptr; 
        librarianCardScreen = nullptr;
        updateLibrarianScreen = nullptr; 
        updateReaderScreen = nullptr;
    }

    void changeState(AppState newState) {
        if (newState != currentState && 
            newState != AppState::ADD_BOOK && newState != AppState::DELETE_BOOK &&
            newState != AppState::BORROW_BOOK && newState != AppState::RETURN_BOOK &&
            newState != AppState::RATING_BOOK && newState != AppState::LIBRARIAN_CARD &&
            newState != AppState::READER_CARD && newState != AppState::UPDATE_LIBRARIAN_INFO &&
            newState != AppState::UPDATE_READER_INFO &&
            currentState != AppState::SPLASH && currentState != AppState::LOGIN_CHOICE) {
            previousState = currentState;
        }
        currentState = newState;
    }

    Sach* findBookById(const std::string& bookId) {
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) { 
            if (current->data->getMaSach() == bookId) 
                return current->data; 
            current = current->next; 
        }
        return nullptr;
    }

    void handleSidebarNavigation(int menuId, Sidebar* sidebar) {
        std::string label = sidebar->getMenuLabel(menuId);
        if (label == "Home") 
            changeState(AppState::HOME);

        else if (label == "Top 10 sach") { 
            libSystem->XepHangSach(); 
            topBooksScreen->loadBooks(mainFont); 
            changeState(AppState::TOP_BOOKS); 
        }

        else if (label == "Tat ca sach") { 
            allBooksListScreen->loadBooks(mainFont); 
            changeState(AppState::ALL_BOOKS_LIST); 
        }

        else if (label == "The loai") { 
            categoryScreen->loadCategories(mainFont); 
            changeState(AppState::CATEGORY); 
        }

        else if (label == "Tim kiem") 
            changeState(AppState::SEARCH);

        else if (label == "Thanh vien" && currentUserRole == UserRole::NONE) { 
            loginChoiceScreen->reset(); 
            modal->show(); 
            changeState(AppState::LOGIN_CHOICE); 
        }

        else if (label == "Dang xuat") {
            currentUser = nullptr; 
            currentReader = nullptr; 
            currentLibrarian = nullptr; 
            currentUserRole = UserRole::NONE;
            homeScreen->setUserRole(currentUserRole, mainFont); 
            categoryScreen->setUserRole(currentUserRole, mainFont);
            searchScreen->setUserRole(currentUserRole, mainFont); 
            topBooksScreen->setUserRole(currentUserRole, mainFont);
            allBooksListScreen->setUserRole(currentUserRole, mainFont);
            changeState(AppState::HOME);
        }

        else if (label == "Sach dang muon") { 
            if(currentReader) { 
                libSystem->DocDanhSachMuonCuaDocGia(currentReader); 
                borrowedBooksScreen->loadBorrowedBooks(mainFont); }
            changeState(AppState::BORROWED_BOOKS); 
        }

        else if (label == "Lich su") { 
            if(currentReader) { readerHistoryScreen->setReader(currentReader); }
            changeState(AppState::HISTORY); 
        }
        else if (label == "Thong tin" && currentUserRole == UserRole::READER) {
            if(readerCardScreen) delete readerCardScreen;
            if(currentReader) { 
                readerCardScreen = new ScreenReaderCard(mainFont, currentReader); 
                modal->show(); 
                changeState(AppState::READER_CARD); }
        }
        else if (label == "Quan ly sach") { 
            manageBooksScreen->loadBooksTable(mainFont); 
            changeState(AppState::MANAGE_BOOKS); 
        }

        else if (label == "Quan ly doc gia") { 
            manageReadersScreen->loadReaders(mainFont); 
            changeState(AppState::MANAGE_READERS); 
        }
        else if (label == "Doc gia qua han") { 
            overdueReadersScreen->loadOverdueReaders(mainFont); 
            changeState(AppState::OVERDUE_READERS); 
        }
        else if (label == "Thong ke") { 
            statisticsScreen->loadStatistics(); 
            changeState(AppState::STATISTICS); 
        }
        else if (label == "Thong tin" && currentUserRole == UserRole::LIBRARIAN) {
            if(librarianCardScreen) delete librarianCardScreen;
            if(currentLibrarian) { 
                librarianCardScreen = new ScreenLibrarianCard(mainFont, currentLibrarian); 
                modal->show(); 
                changeState(AppState::LIBRARIAN_CARD); 
            }
        }
    }

    
    void handleReaderLogin() {
        loginReaderScreen->update(sf::Vector2f(0,0));
        if (!loginReaderScreen->validate()) return;
        NodeReader* cur = libSystem->getDanhSachDocGia();
        while(cur) {
            if(cur->data->getUsername() == loginReaderScreen->getUsername() && cur->data->getPassword() == loginReaderScreen->getPassword()) {
                currentUser = cur->data; 
                currentReader = cur->data; 
                currentUserRole = UserRole::READER;
                libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                homeScreen->setUserRole(currentUserRole, mainFont); 
                categoryScreen->setUserRole(currentUserRole, mainFont);
                searchScreen->setUserRole(currentUserRole, mainFont); 
                topBooksScreen->setUserRole(currentUserRole, mainFont);
                allBooksListScreen->setUserRole(currentUserRole, mainFont);
                delete borrowedBooksScreen; borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader, libSystem); 
                borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
                loginReaderScreen->clearFields(); 
                modal->hide(); changeState(AppState::HOME); return;
            } 
            cur = cur->next;
        } 
        loginReaderScreen->setLoginError("Sai thong tin dang nhap!");
    }

    void handleLibrarianLogin() {
        loginLibrarianScreen->update(sf::Vector2f(0,0));
        if (!loginLibrarianScreen->validate()) return;
        USER* temp = nullptr;
        if (libSystem->DangNhapThuThu(loginLibrarianScreen->getUsername(), loginLibrarianScreen->getPassword(), temp)) {
            currentUser = temp; 
            currentLibrarian = dynamic_cast<Librarian*>(temp); 
            currentUserRole = UserRole::LIBRARIAN;
            homeScreen->setUserRole(currentUserRole, mainFont); 
            categoryScreen->setUserRole(currentUserRole, mainFont);
            searchScreen->setUserRole(currentUserRole, mainFont); 
            topBooksScreen->setUserRole(currentUserRole, mainFont);
            allBooksListScreen->setUserRole(currentUserRole, mainFont); 
            manageBooksScreen->setUserRole(currentUserRole, mainFont);
            manageReadersScreen->setUserRole(currentUserRole, mainFont); 
            statisticsScreen->setUserRole(currentUserRole, mainFont);
            overdueReadersScreen->setUserRole(currentUserRole, mainFont);
            loginLibrarianScreen->clearFields(); modal->hide(); changeState(AppState::HOME);
        } else { 
            loginLibrarianScreen->setLoginError("Sai thong tin dang nhap!"); 
        }
    }

    void handleEvents() {
        sf::Event event; 
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            switch (currentState) {

                case AppState::SPLASH: 
                    if(event.type==sf::Event::MouseButtonPressed && splashScreen->handleClick(mousePos)) 
                        changeState(AppState::HOME); 
                    break;

                case AppState::HOME:
                    homeScreen->handleSearchEvent(event, mousePos);

                    if (homeScreen->isSearchRequested() || homeScreen->checkSearchClick(mousePos)) { 
                        searchScreen->setSearchQuery(homeScreen->getSearchQuery()); 
                        searchScreen->performSearch(mainFont); 
                        changeState(AppState::SEARCH); }

                    if (event.type == sf::Event::MouseButtonPressed) {
                        int m = homeScreen->handleSidebarClick(mousePos); 
                        if (m >= 0) handleSidebarNavigation(m, homeScreen->getSidebar());
                        std::string bid = homeScreen->handleCardClick(mousePos); 
                        if (!bid.empty()) { 
                            Sach* s=findBookById(bid); 
                            if(s) { 
                                previousState=AppState::HOME; 
                                bookDetailScreen->setBook(s, currentUserRole, libSystem); 
                                detailModal->show(); changeState(AppState::BOOK_DETAIL); 
                            } 
                        }
                    } 
                    break;

                case AppState::TOP_BOOKS: 
                    topBooksScreen->handleScrollEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed) { 
                        int m=topBooksScreen->handleSidebarClick(mousePos); 
                        if(m>=0) handleSidebarNavigation(m, topBooksScreen->getSidebar()); 
                        std::string b=topBooksScreen->handleCardClick(mousePos); 
                        if(!b.empty()){ 
                            Sach* s=findBookById(b); 
                            if(s){ 
                                previousState=AppState::TOP_BOOKS; 
                                bookDetailScreen->setBook(s, currentUserRole, libSystem); 
                                detailModal->show(); changeState(AppState::BOOK_DETAIL); 
                            }
                        }
                    } 
                    break;

                case AppState::ALL_BOOKS_LIST: 
                    allBooksListScreen->handleScrollEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed) { 
                        int m=allBooksListScreen->handleSidebarClick(mousePos); 
                        if(m>=0) handleSidebarNavigation(m, allBooksListScreen->getSidebar()); 
                        std::string b=allBooksListScreen->handleCardClick(mousePos); 
                        if(!b.empty()){ 
                            Sach* s=findBookById(b); 
                            if(s){
                                previousState=AppState::ALL_BOOKS_LIST; 
                                bookDetailScreen->setBook(s, currentUserRole, libSystem); 
                                detailModal->show(); 
                                changeState(AppState::BOOK_DETAIL); 
                            }
                        }
                    } 
                    break;

                case AppState::CATEGORY: 
                    categoryScreen->handleScrollEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed) { 
                        int m=categoryScreen->handleSidebarClick(mousePos); 
                        if(m>=0) handleSidebarNavigation(m, categoryScreen->getSidebar()); 
                        if(categoryScreen->handleCategoryButtonClick(mousePos, mainFont)) 
                            break; 

                        categoryScreen->handleClick(mousePos, mainFont); 
                        std::string b=categoryScreen->handleCardClick(mousePos); 

                        if(!b.empty()){ 
                            Sach* s=findBookById(b); 
                            if(s){ 
                                previousState=AppState::CATEGORY; 
                                bookDetailScreen->setBook(s, currentUserRole, libSystem); 
                                detailModal->show(); 
                                changeState(AppState::BOOK_DETAIL); 
                            }
                        }
                    } 
                    break;

                case AppState::SEARCH: 
                    searchScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        int m=searchScreen->handleSidebarClick(mousePos); 
                    
                        if(m>=0) handleSidebarNavigation(m, searchScreen->getSidebar()); 
                        searchScreen->handleSearchClick(mousePos, mainFont); 
                        std::string b=searchScreen->handleCardClick(mousePos); 

                        if(!b.empty()){ 
                            Sach* s=findBookById(b); 
                            if(s){ 
                                previousState=AppState::SEARCH; 
                                bookDetailScreen->setBook(s, currentUserRole, libSystem); 
                                detailModal->show(); changeState(AppState::BOOK_DETAIL); 
                            }
                        }
                    } 
                    break;

                case AppState::BOOK_DETAIL: 
                    bookDetailScreen->handleScrollEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        if(detailModal->handleClose(mousePos)){ 
                            detailModal->hide(); 
                            changeState(previousState); } 
                        else { 
                            int a = bookDetailScreen->handleClick(mousePos); 
                            if(a==0){ 
                                detailModal->hide(); 
                                changeState(previousState); } 
                            else if(a==2){ 
                                detailModal->hide(); 
                                deleteBookScreen->setBookID(bookDetailScreen->getCurrentBook()->getMaSach()); 
                                modal->show(); 
                                changeState(AppState::DELETE_BOOK); 
                            } 
                            else if(a==3){ 
                                detailModal->hide(); 
                                manageBooksScreen->setBookToEdit(bookDetailScreen->getCurrentBook()); 
                                manageBooksScreen->showUpdateModal(); 
                                changeState(AppState::MANAGE_BOOKS); 
                            }
                        } 
                    } 
                    break;
                
                case AppState::LOGIN_CHOICE: 
                    loginChoiceScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        if(loginChoiceScreen->isClosed()){ modal->hide(); 
                            changeState(AppState::HOME); 
                        } else { 
                            int c=loginChoiceScreen->handleClick(mousePos); 
                            if(c==1) changeState(AppState::LOGIN_LIBRARIAN); 
                            else if(c==2) changeState(AppState::LOGIN_READER); 
                        } 
                    } 
                    break;

                case AppState::LOGIN_LIBRARIAN: 
                    loginLibrarianScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        if(modal->handleClose(mousePos)){ 
                            modal->hide(); changeState(AppState::HOME); 
                        } else { 
                            int a=loginLibrarianScreen->handleClick(mousePos); 
                            if(a==1) handleLibrarianLogin();
                            else if(a==2)
                            changeState(AppState::LOGIN_CHOICE); 
                        } 
                    } 
                    break;

                case AppState::LOGIN_READER: 
                    loginReaderScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        if(modal->handleClose(mousePos)){ 
                            modal->hide(); 
                            changeState(AppState::HOME); 
                        } else { 
                            int a=loginReaderScreen->handleClick(mousePos); 
                            if(a==1) handleReaderLogin(); 
                            else if(a==2) changeState(AppState::REGISTER_READER); 
                            else if(a==3) changeState(AppState::LOGIN_CHOICE); 
                        } 
                    } 
                    break;

        
                case AppState::BORROWED_BOOKS: 
                    borrowedBooksScreen->handleScrollEvent(event, mousePos);

                    if (event.type == sf::Event::MouseButtonPressed) {
                        int m = borrowedBooksScreen->handleSidebarClick(mousePos); 
                        if (m >= 0) handleSidebarNavigation(m, borrowedBooksScreen->getSidebar()); 
                        
                        std::string bookID = borrowedBooksScreen->handleRateClick(mousePos);
                        
                    } 
                    break;

                case AppState::HISTORY: 
                    readerHistoryScreen->handleEvent(event, mousePos); 

                    if (event.type == sf::Event::MouseButtonPressed) {
                        std::string bookID = readerHistoryScreen->handleClickRate(mousePos);
                        if (!bookID.empty()) {
                            // Xác định người đang được xem (User hoặc Admin xem hộ)
                            Reader* targetReader = (currentUserRole == UserRole::LIBRARIAN) ? selectedReaderForAdmin : currentReader;
                            if (currentUserRole == UserRole::READER) {
                                ratingBookScreen->setBookID(bookID);
                                int oldScore = libSystem->LayDiemDanhGia(targetReader->getMaID(), bookID);
                                ratingBookScreen->setExistingRating(oldScore);
                                modal->show();
                                
                                // Lưu trạng thái cũ là HISTORY để khi đánh giá xong quay lại đây
                                previousState = AppState::HISTORY; 
                                changeState(AppState::RATING_BOOK);
                            }
                        }
                    }

                    // Logic đóng cửa sổ lịch sử
                    if(readerHistoryScreen->wasClosed()) {
                        readerHistoryScreen->reset(); 
                        if (currentUserRole == UserRole::LIBRARIAN) {
                            changeState(AppState::MANAGE_READERS);
                            if (selectedReaderForAdmin) {
                                manageReadersScreen->showDetailFor(selectedReaderForAdmin, mainFont);
                            }
                        } else {
                            changeState(AppState::HOME); 
                        }
                    } 
                    break;

                case AppState::MANAGE_BOOKS: 
                    manageBooksScreen->handleEvent(event, mousePos); 

                    if(event.type == sf::Event::MouseButtonPressed) { 
                        int m = manageBooksScreen->handleSidebarClick(mousePos); 
                        if(m >= 0) handleSidebarNavigation(m, manageBooksScreen->getSidebar()); 
                        
                        int act = manageBooksScreen->handleClick(mousePos); 
                        
                        if(act == 1 || act == 5){
                            libSystem->BuildHashTable(); 
                            homeScreen->loadBooks(mainFont);
                        } 
                        else if(act == 7){
                            std::string id = manageBooksScreen->getClickedBookId(mousePos); 
                            if(!id.empty()){ 
                                manageBooksScreen->handleRowClick(id); 
                            } 
                        } 
                        else if(act == 8){
                            deleteBookScreen->clear(); 
                            modal->show(); 
                            changeState(AppState::DELETE_BOOK);
                        } 
                    } 
                    break;
                
                case AppState::MANAGE_READERS: 
                    manageReadersScreen->handleEvent(event, mousePos); 

                    if (messageBox->isShown()) {
                        if (event.type == sf::Event::MouseButtonPressed) {
                            int choice = messageBox->handleClick(mousePos);
                            if (popupState == PopupState::CONFIRM_RETURN_ADMIN) {
                                if (choice == 1) { 
                                    selectedReaderForAdmin = manageReadersScreen->getSelectedReader();
                                    libSystem->TraSach(selectedReaderForAdmin, pendingReturnBookID);
                                    manageReadersScreen->refreshCurrentReader(mainFont);
                                    messageBox->hide();
                                    popupState = PopupState::NONE;
                                } else if (choice == 2) { 
                                    messageBox->hide();
                                    popupState = PopupState::NONE;
                                }
                            }
                        }
                    }
                    else if(event.type==sf::Event::MouseButtonPressed){ 
                        int m=manageReadersScreen->handleSidebarClick(mousePos); 
                        if(m>=0) handleSidebarNavigation(m, manageReadersScreen->getSidebar()); 
                        
                        int action = manageReadersScreen->handleClick(mousePos, mainFont);
                        
                        if (action == 101) { 
                            selectedReaderForAdmin = manageReadersScreen->getSelectedReader();
                            borrowBookScreen->clearInput();
                            borrowBookScreen->clearMessage();
                            modal->show();
                            changeState(AppState::BORROW_BOOK);
                        }
                        else if (action == 103) { 
                            selectedReaderForAdmin = manageReadersScreen->getSelectedReader();
                            readerHistoryScreen->setReader(selectedReaderForAdmin);
                            changeState(AppState::HISTORY);
                        }
                        else if (action == 104) { 
                            pendingReturnBookID = manageReadersScreen->getClickedReturnBookID();
                            popupState = PopupState::CONFIRM_RETURN_ADMIN;
                            messageBox->setButtonLabels("Dong Y", "Huy");
                            messageBox->show("XAC NHAN", "Ban co chac chan muon tra sach nay?", MsgType::CONFIRM, true);
                        }
                    } 
                    break;
                
                case AppState::STATISTICS: 
                    if(event.type == sf::Event::MouseButtonPressed) { 
                        int m = statisticsScreen->handleSidebarClick(mousePos); 

                        if(m >= 0) handleSidebarNavigation(m, statisticsScreen->getSidebar()); 
                            int c = statisticsScreen->handleClick(mousePos); 
                        if(c == 1) { 
                            manageBooksScreen->loadBooksTable(mainFont); 
                            changeState(AppState::MANAGE_BOOKS); } 
                        else if(c == 2) { 
                            manageReadersScreen->loadReaders(mainFont); 
                            changeState(AppState::MANAGE_READERS); } 
                        else if(c == 4) { 
                            overdueReadersScreen->loadOverdueReaders(mainFont); 
                            changeState(AppState::OVERDUE_READERS); } 
                    } 
                    break;

                case AppState::OVERDUE_READERS: 
                    overdueReadersScreen->handleScrollEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed) { 
                       int m=overdueReadersScreen->handleSidebarClick(mousePos); 
                    if(m>=0) handleSidebarNavigation(m, overdueReadersScreen->getSidebar()); 
                } 
                break;
                
                case AppState::BORROW_BOOK: 
                    borrowBookScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        int a=borrowBookScreen->handleClick(mousePos); 
                        if(a==1){ 
                            if(borrowBookScreen->validate()) {
                                std::string id=borrowBookScreen->getMaSachInput(); 
                                Reader* target = (currentUserRole == UserRole::LIBRARIAN) ? selectedReaderForAdmin : currentReader;
                                
                                Sach* s = findBookById(id);
                                if (s == nullptr) {
                                    borrowBookScreen->setErrorMessage("Ma sach khong ton tai!");
                                }
                                else if (target && target->DemSachDaMuon() >= 5) {
                                    borrowBookScreen->setErrorMessage("Da muon toi da 5 cuon!");
                                }
                                else if (target && target->DaMuonSach(id)) {
                                    borrowBookScreen->setErrorMessage("Ban da muon sach nay roi!");
                                }
                                else if (s->getSoLuong() <= 0) {
                                    borrowBookScreen->setErrorMessage("Sach nay da het!");
                                }
                                else {
                                    libSystem->MuonSach(target, id); 
                                    borrowBookScreen->setSuccessMessage("Muon thanh cong!"); 
                                    if(currentUserRole == UserRole::LIBRARIAN) {
                                        manageReadersScreen->refreshCurrentReader(mainFont);
                                    }
                                    borrowBookScreen->setCurrentBook(s);
                                }
                            }
                        } 
                        else if(a==2){ 
                            modal->hide(); 
                            if (currentUserRole == UserRole::LIBRARIAN) 
                            changeState(AppState::MANAGE_READERS);
                            else changeState(previousState);
                        } 
                    } 
                    break;
                
                case AppState::RETURN_BOOK: 
                    returnBookScreen->handleEvent(event, mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        int a=returnBookScreen->handleClick(mousePos); 
                        if(a==1){
                            if(returnBookScreen->validate()){
                                std::string id=returnBookScreen->getMaSachInput(); 
                                Reader* target = selectedReaderForAdmin;
                                if(target) {
                                    if(!target->DaMuonSach(id)) returnBookScreen->setReturnMessage("Chua muon!"); 
                                    else {
                                        libSystem->TraSach(target, id); 
                                        returnBookScreen->setReturnMessage("Tra thanh cong!");
                                    }
                                }
                            }
                        } 
                        else if(a==2 || a==4){
                            modal->hide(); 
                            changeState(AppState::MANAGE_READERS);
                        } 
                    } 
                    break;
                
                case AppState::RATING_BOOK: 
                    ratingBookScreen->update(mousePos); 

                    if(event.type==sf::Event::MouseButtonPressed){ 
                        int a=ratingBookScreen->handleClick(mousePos); 
                        if(a==1){ // Submit
                            Sach* s=findBookById(ratingBookScreen->getBookID()); 
                            if(s){
                                int score = ratingBookScreen->getScore();
                                libSystem->DanhGiaSach(currentReader, s, score);
                            } 
                            modal->hide(); 
                            borrowedBooksScreen->loadBorrowedBooks(mainFont); 
                            if (previousState == AppState::HISTORY) {
                                readerHistoryScreen->loadFromFile();
                                changeState(AppState::HISTORY);
                            } else {
                                changeState(AppState::BORROWED_BOOKS);
                            }
                        } 
                        else if(a==2){ // Cancel
                            modal->hide(); 
                            if (previousState == AppState::HISTORY) {
                                changeState(AppState::HISTORY);
                            } else {
                                changeState(AppState::BORROWED_BOOKS);
                            }
                        } 
                    } 
                    break;

                case AppState::ADD_BOOK: 
                    addBookScreen->handleEvent(event, mousePos); 
                    if(addBookScreen->isClosed()){ 
                        modal->hide(); 
                        homeScreen->loadBooks(mainFont); 
                        changeState(AppState::MANAGE_BOOKS); 
                    } 
                    break;

                case AppState::DELETE_BOOK:

                    deleteBookScreen->handleEvent(event, mousePos);

                    if (event.type == sf::Event::MouseButtonPressed) {
                        int a = deleteBookScreen->handleClick(mousePos);

                        if (a == 1) {
                            if (deleteBookScreen->validate()) {

                                std::string maSach = deleteBookScreen->getMaSach();
                                int soLuong = deleteBookScreen->getSoLuong();

                                std::string ketQua = libSystem->XoaSach(maSach, soLuong);

                                libSystem->GhiFileHeThong("DanhSachSach.txt");

                                deleteBookScreen->setResult(ketQua);

                                manageBooksScreen->loadBooksTable(mainFont);
                            }
                        }

                        else if (a == 2) {
                            deleteBookScreen->clear();
                            modal->hide();
                            changeState(AppState::MANAGE_BOOKS);
                        }
    
                        break;
                    }

                case AppState::READER_CARD: 
                    if(readerCardScreen){
                        readerCardScreen->handleEvent(event, mousePos); 
                    if(readerCardScreen->wasClosed()){
                        modal->hide(); 
                        if (readerCardScreen->isUpdateRequested()) { 
                            if(updateReaderScreen) 
                                delete updateReaderScreen; 
                            updateReaderScreen = new UpdateReaderScreen(mainFont, currentReader); 
                            changeState(AppState::UPDATE_READER_INFO); 
                        } else { 
                            changeState(previousState); 
                        } 
                    } 
                } 
                break;

                case AppState::LIBRARIAN_CARD: 
                    if(librarianCardScreen){
                        librarianCardScreen->handleEvent(event, mousePos); 
                        if(librarianCardScreen->wasClosed()){
                            modal->hide(); 
                            if (librarianCardScreen->isUpdateRequested()) { 
                                if(updateLibrarianScreen) 
                                    delete updateLibrarianScreen; 
                                updateLibrarianScreen = new UpdateLibrarianScreen(mainFont, currentLibrarian); 
                                changeState(AppState::UPDATE_LIBRARIAN_INFO); 
                            } else { 
                                changeState(previousState); 
                            } 
                        } 
                    } 
                    break;

                case AppState::UPDATE_LIBRARIAN_INFO: 
                    if(updateLibrarianScreen) { 
                        updateLibrarianScreen->handleEvent(event, mousePos); 
                        if(updateLibrarianScreen->isClosed()) 
                            changeState(AppState::HOME); 
                    } 
                    break;

                case AppState::UPDATE_READER_INFO: 
                    if(updateReaderScreen) { 
                        updateReaderScreen->handleEvent(event, mousePos); 
                        if(updateReaderScreen->isClosed()) 
                            changeState(AppState::HOME); 
                    } 
                    break;
            }
        }
    }

    void update() {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (messageBox && messageBox->isShown()) { 
            messageBox->update(mousePos); 
            return; 
        }

        switch (currentState) {
            case AppState::SPLASH: 
                splashScreen->update(mousePos); 
                break;
            case AppState::HOME: 
                homeScreen->update(mousePos); 
                break;
            case AppState::TOP_BOOKS: 
                topBooksScreen->update(mousePos); 
                break;
            case AppState::ALL_BOOKS_LIST: 
                allBooksListScreen->update(mousePos); 
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
               readerHistoryScreen->update(mousePos); 
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
            case AppState::BORROW_BOOK: 
               borrowBookScreen->update(mousePos); 
               break;
            case AppState::RETURN_BOOK: 
               returnBookScreen->update(mousePos); 
               break;
            case AppState::RATING_BOOK: 
               ratingBookScreen->update(mousePos); 
               break;
            case AppState::DELETE_BOOK: 
               deleteBookScreen->update(mousePos); 
               break;
            case AppState::READER_CARD: 
               if(readerCardScreen) 
                   readerCardScreen->update(mousePos); 
                   break;
            case AppState::LIBRARIAN_CARD: 
                if(librarianCardScreen) 
                   librarianCardScreen->update(mousePos); 
                   break;
            case AppState::UPDATE_LIBRARIAN_INFO: 
                if(updateLibrarianScreen) 
                   updateLibrarianScreen->update(mousePos); 
                   break;
            case AppState::UPDATE_READER_INFO: 
                if(updateReaderScreen) 
                   updateReaderScreen->update(mousePos); 
                   break;
        }
    }

    void render() {
        window.clear(Theme::Background); 
        switch (currentState) {
            case AppState::SPLASH: 
               splashScreen->render(window); 
               break;
            case AppState::HOME: 
               homeScreen->render(window); 
               break;
            case AppState::TOP_BOOKS: 
               topBooksScreen->render(window); 
               break;
            case AppState::ALL_BOOKS_LIST: 
               allBooksListScreen->render(window); 
               break;
            case AppState::CATEGORY: 
               categoryScreen->render(window); 
               break;
            case AppState::SEARCH: 
               searchScreen->render(window); 
               break;
            case AppState::BOOK_DETAIL: 
                if(previousState==AppState::HOME) 
                   homeScreen->render(window); 
                else if(previousState==AppState::MANAGE_BOOKS) 
                   manageBooksScreen->render(); {
                   sf::RectangleShape ov({1300,720}); 
                   ov.setFillColor(sf::Color(0,0,0,150)); 
                   window.draw(ov); 
                   detailModal->draw(window); 
                   bookDetailScreen->render(window);
                } 
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
                if(messageBox->isShown()) 
                   messageBox->draw(window); 
                break;
            case AppState::HISTORY: 
                readerHistoryScreen->render(window); 
                break;
            case AppState::MANAGE_BOOKS: 
                manageBooksScreen->render(); 
                break;
            case AppState::MANAGE_READERS: 
                manageReadersScreen->render(window); 
                if(messageBox->isShown()) 
                    messageBox->draw(window); 
                break;
            case AppState::STATISTICS: 
                statisticsScreen->render(window); 
                break;
            case AppState::OVERDUE_READERS: 
                overdueReadersScreen->render(window); 
                break;
            case AppState::BORROW_BOOK: 
                if(previousState==AppState::HOME) 
                    homeScreen->render(window); 
                else if(previousState==AppState::BOOK_DETAIL) 
                    homeScreen->render(window); 
                    modal->draw(window); 
                    borrowBookScreen->render(window); 
                    break;
            case AppState::RETURN_BOOK: 
                manageReadersScreen->render(window); 
                modal->draw(window); 
                returnBookScreen->render(window); 
                break;
            case AppState::RATING_BOOK:  
                borrowedBooksScreen->render(window); 
                modal->draw(window); 
                ratingBookScreen->render(window); 
                break;
            case AppState::ADD_BOOK: 
                manageBooksScreen->render(); 
                modal->draw(window); 
                addBookScreen->render(window); 
                break;
            case AppState::DELETE_BOOK: 
                manageBooksScreen->render(); 
                modal->draw(window); 
                deleteBookScreen->render(window); 
                break;
            case AppState::READER_CARD: 
                if(previousState==AppState::HOME) 
                    homeScreen->render(window); 
                    modal->draw(window); 
                if(readerCardScreen) 
                   readerCardScreen->render(window); 
                   break;
            case AppState::LIBRARIAN_CARD: 
                if(previousState==AppState::HOME) 
                    homeScreen->render(window); 
                modal->draw(window); 
                if(librarianCardScreen) 
                    librarianCardScreen->render(window);
                    break;
            case AppState::UPDATE_LIBRARIAN_INFO: 
                if(updateLibrarianScreen) 
                updateLibrarianScreen->render(window); 
                break;
            case AppState::UPDATE_READER_INFO: 
                if(updateReaderScreen) 
                updateReaderScreen->render(window); 
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