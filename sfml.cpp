#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "LibrarySystem.h"  // để gọi tới lớp hệ thống của bạn

using namespace std;
using namespace sf;

struct Button {
    RectangleShape shape;
    Text text;
};

bool isMouseOver(const RenderWindow &window, const RectangleShape &shape) {
    auto pos = Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

Button createButton(Font &font, string label, Vector2f pos, Color color = Color(70,130,180)) {
    Button b;
    b.shape.setSize({300, 50});
    b.shape.setFillColor(color);
    b.shape.setPosition(pos);
    b.shape.setOutlineThickness(2);
    b.shape.setOutlineColor(Color::Black);

    b.text.setFont(font);
    b.text.setString(label);
    b.text.setCharacterSize(22);
    b.text.setFillColor(Color::White);
    b.text.setPosition(pos.x + 20, pos.y + 10);

    return b;
}

void drawButton(RenderWindow &window, Button &b) {
    window.draw(b.shape);
    window.draw(b.text);
}

// ===================== FORM HIEN THI DANH SACH SACH ==========================
void showBookList(RenderWindow &window, Font &font, LibrarySystem &lib) {
    bool running = true;
    while (running && window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                running = false;
        }

        window.clear(Color(245, 245, 245));

        Text title("DANH SACH SACH TRONG THU VIEN", font, 28);
        title.setFillColor(Color::Blue);
        title.setPosition(120, 30);
        window.draw(title);

        // Vẽ bảng
        float y = 120;
        Text header("", font, 20);
        header.setFillColor(Color::Black);
        header.setString("MaSach       | TenSach                 | TacGia             | TheLoai   | NamXB | NXB");
        header.setPosition(40, y);
        window.draw(header);
        y += 40;

        NodeBook *current = lib.HeadDsSach;
        while (current && y < 550) {
            Sach *s = current->data;
            stringstream ss;
            ss << left << setw(10) << s->getMaSach()
               << " | " << setw(22) << s->getTenSach().substr(0, 22)
               << " | " << setw(16) << s->getTacGia().substr(0, 16)
               << " | " << setw(9) << s->getTheLoai()
               << " | " << setw(6) << s->getNamXB()
               << " | " << s->getNhaXB().substr(0, 12);

            Text row(ss.str(), font, 18);
            row.setFillColor(Color::Black);
            row.setPosition(40, y);
            window.draw(row);
            y += 30;
            current = current->next;
        }

        Text hint("Nhan ESC de quay lai menu chinh", font, 18);
        hint.setFillColor(Color(100, 100, 100));
        hint.setPosition(230, 560);
        window.draw(hint);

        window.display();
    }
}

// ===================== FORM DANG NHAP DOC GIA ==========================
void showLoginForm(RenderWindow &window, Font &font, LibrarySystem &lib) {
    string username, password;
    Text title("DANG NHAP DOC GIA", font, 30);
    title.setPosition(200, 60);
    title.setFillColor(Color::Blue);

    Text uLabel("Username:", font, 22);
    uLabel.setPosition(150, 200);
    Text pLabel("Password:", font, 22);
    pLabel.setPosition(150, 270);

    Text uInput("", font, 22);
    uInput.setPosition(300, 200);
    uInput.setFillColor(Color::Black);

    Text pInput("", font, 22);
    pInput.setPosition(300, 270);
    pInput.setFillColor(Color::Black);

    bool enteringUser = true;
    bool done = false;
    USER* currentUser = nullptr;

    while (!done && window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::TextEntered) {
                if (e.text.unicode == '\b') { // Backspace
                    if (enteringUser && !username.empty()) username.pop_back();
                    else if (!enteringUser && !password.empty()) password.pop_back();
                } else if (e.text.unicode == '\r') { // Enter
                    if (enteringUser) enteringUser = false;
                    else {
                        if (lib.DangNhapDocGia(username, password, currentUser))
                            done = true;
                        else {
                            username.clear();
                            password.clear();
                            enteringUser = true;
                        }
                    }
                } else if (e.text.unicode < 128 && e.text.unicode >= 32) {
                    if (enteringUser) username += static_cast<char>(e.text.unicode);
                    else password += static_cast<char>(e.text.unicode);
                }
            }
        }

        uInput.setString(username);
        pInput.setString(string(password.size(), '*')); // che password

        window.clear(Color::White);
        window.draw(title);
        window.draw(uLabel);
        window.draw(pLabel);
        window.draw(uInput);
        window.draw(pInput);
        window.display();
    }

    if (done) {
        showBookList(window, font, lib);
    }
}

// ===================== MENU CHINH ==========================
int main() {
    RenderWindow window(VideoMode(900, 650), "He thong quan ly thu vien");
    LibrarySystem lib;
    lib.DocFileHeThong();

    Font font;
    if (!font.loadFromFile("assets/font/arial.ttf")) {
        cerr << "Khong tim thay font arial.ttf!\n";
        return 1;
    }

    Text title("HE THONG QUAN LY THU VIEN", font, 36);
    title.setFillColor(Color::Blue);
    title.setPosition(150, 80);

    Button btn1 = createButton(font, "1. Dang nhap / Dang ky (Doc gia)", {300, 200});
    Button btn2 = createButton(font, "2. Dang nhap (Thu thu)", {300, 280});
    Button btn3 = createButton(font, "3. Hien thi danh sach sach", {300, 360});
    Button btn0 = createButton(font, "0. Thoat chuong trinh", {300, 440}, Color(220, 50, 50));

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::MouseButtonPressed) {
                Vector2f mouse(Mouse::getPosition(window));
                if (btn1.shape.getGlobalBounds().contains(mouse)) {
                    showLoginForm(window, font, lib);
                } else if (btn3.shape.getGlobalBounds().contains(mouse)) {
                    showBookList(window, font, lib);
                } else if (btn0.shape.getGlobalBounds().contains(mouse)) {
                    window.close();
                }
            }
        }

        window.clear(Color(245, 245, 245));
        window.draw(title);
        drawButton(window, btn1);
        drawButton(window, btn2);
        drawButton(window, btn3);
        drawButton(window, btn0);
        window.display();
    }

    return 0;
}
