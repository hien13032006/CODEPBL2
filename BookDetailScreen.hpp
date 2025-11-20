#ifndef BOOK_DETAIL_SCREEN_HPP
#define BOOK_DETAIL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"
#include "Book.h"

class BookDetailScreen {
private:
    Modal* modal;
    sf::RectangleShape detailBox;
    sf::Text titleText;
    sf::Text infoText;
    Button* actionButton;
    Button* deleteButton;
    Button* updateButton;
    Button* closeButton;
    Sach* currentBook;
    UserRole userRole;

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        detailBox.setSize(sf::Vector2f(700, 500));
        detailBox.setPosition(350, 200);
        detailBox.setFillColor(sf::Color(30, 35, 50));

        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(400, 250);

        infoText.setFont(font);
        infoText.setCharacterSize(14);
        infoText.setFillColor(sf::Color(200, 200, 200));
        infoText.setPosition(400, 300);

        actionButton = new Button(sf::Vector2f(400, 600), sf::Vector2f(200, 50),
                                 "Muon Sach", font, 0, sf::Color(100, 180, 100));
        
        deleteButton = new Button(sf::Vector2f(400, 600), sf::Vector2f(200, 50),
                                 "Xoa Sach", font, 0, sf::Color(200, 80, 80));
        
        updateButton = new Button(sf::Vector2f(620, 600), sf::Vector2f(200, 50),
                                 "Cap Nhat", font, 0, sf::Color(100, 150, 200));
        
        closeButton = new Button(sf::Vector2f(840, 600), sf::Vector2f(150, 50),
                                "Dong", font, 0, sf::Color(100, 100, 120));

        userRole = UserRole::NONE;
    }

    ~BookDetailScreen() {
        delete actionButton;
        delete deleteButton;
        delete updateButton;
        delete closeButton;
    }

    void setBook(Sach* book, UserRole role) {
        currentBook = book;
        userRole = role;

        if (book) {
            titleText.setString(book->getTenSach());

            std::string info = "Ma sach: " + book->getMaSach() + "\n";
            info += "Tac gia: " + book->getTacGia() + "\n";
            info += "The loai: " + book->getTheLoai() + "\n";
            info += "Nam xuat ban: " + std::to_string(book->getNamXuatBan()) + "\n";
            info += "Nha xuat ban: " + book->getNhaXuatBan() + "\n";
            info += "So luong: " + std::to_string(book->getSoLuong()) + "\n";
            
            char rating[20];
            sprintf(rating, "Diem danh gia: %.1f/10\n", book->getDiemTrungBinh());
            info += rating;

            infoText.setString(info);
        }
    }

    void update(sf::Vector2f mousePos) {
        if (userRole == UserRole::READER) {
            actionButton->update(mousePos);
        } else if (userRole == UserRole::LIBRARIAN) {
            deleteButton->update(mousePos);
            updateButton->update(mousePos);
        }
        closeButton->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (closeButton->handleClick(mousePos)) {
            return 0; // Close
        }
        
        if (userRole == UserRole::READER) {
            if (actionButton->handleClick(mousePos)) {
                return 1; // Borrow book
            }
        } else if (userRole == UserRole::LIBRARIAN) {
            if (deleteButton->handleClick(mousePos)) {
                return 2; // Delete book
            }
            if (updateButton->handleClick(mousePos)) {
                return 3; // Update book
            }
        }
        
        return -1;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(detailBox);
            window.draw(titleText);
            window.draw(infoText);

            if (userRole == UserRole::READER) {
                actionButton->draw(window);
            } else if (userRole == UserRole::LIBRARIAN) {
                deleteButton->draw(window);
                updateButton->draw(window);
            }
            
            closeButton->draw(window);
        }
    }

    Sach* getCurrentBook() { return currentBook; }
};

#endif