#ifndef SCREEN_LIB_ADD_H
#define SCREEN_LIB_ADD_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include <SFML/Graphics.hpp>

class ScreenLibAdd : public ScreenBase {
private:
    LibrarySystem *lib;

    sf::Text label1, label2, label3, label4, label5;

    TextBox boxName;
    TextBox boxAuthor;
    TextBox boxCate;
    TextBox boxYear;
    TextBox boxNXB;

    Button btnOK, btnBack;

    sf::Font *fontUse;

public:
    ScreenLibAdd(sf::Font &font, LibrarySystem *L){
        lib = L;
        fontUse = &font;

        float px = 350;
        float py = 150;
        float dy = 60;

        label1.setFont(font);
        label1.setString("Ten sach");
        label1.setCharacterSize(22);
        label1.setPosition(px - 250, py);

        label2.setFont(font);
        label2.setString("Tac gia");
        label2.setCharacterSize(22);
        label2.setPosition(px - 250, py + dy);

        label3.setFont(font);
        label3.setString("The loai");
        label3.setCharacterSize(22);
        label3.setPosition(px - 250, py + dy * 2);

        label4.setFont(font);
        label4.setString("Nam XB");
        label4.setCharacterSize(22);
        label4.setPosition(px - 250, py + dy * 3);

        label5.setFont(font);
        label5.setString("Nha XB");
        label5.setCharacterSize(22);
        label5.setPosition(px - 250, py + dy * 4);

        boxName   = TextBox(font, {px, py},         {300,40});
        boxAuthor = TextBox(font, {px, py+dy},      {300,40});
        boxCate   = TextBox(font, {px, py+dy*2},    {300,40});
        boxYear   = TextBox(font, {px, py+dy*3},    {300,40});
        boxNXB    = TextBox(font, {px, py+dy*4},    {300,40});

        btnOK   = Button("Them", font,     {px, py+dy*6},  {200,50});
        btnBack = Button("Quay lai", font, {px, py+dy*7},  {200,50});
    }

    void handleEvent(sf::Event &e, AppState &state) override {
        boxName.handleEvent(e);
        boxAuthor.handleEvent(e);
        boxCate.handleEvent(e);
        boxYear.handleEvent(e);
        boxNXB.handleEvent(e);

        if(e.type == sf::Event::MouseButtonPressed){
            float mx = e.mouseButton.x;
            float my = e.mouseButton.y;

            if(btnBack.hit(mx,my)){
                state = SCREEN_READER_MENU;
                return;
            }

            if(btnOK.hit(mx,my)){
                // Lấy dữ liệu
                std::string ten   = boxName.getText();
                std::string tac   = boxAuthor.getText();
                std::string loai  = boxCate.getText();
                std::string nam   = boxYear.getText();
                std::string nxb   = boxNXB.getText();

                if(ten=="" || tac=="" || loai=="" || nam=="" || nxb==""){
                    // thiếu dữ liệu
                    return;
                }

                int namXB = std::stoi(nam);

                // tạo sách
                Sach *s = Sach::createFromData(
                    ten,
                    tac,
                    loai,
                    namXB,
                    nxb
                );

                lib->addBook(s);
                lib->GhiFileHeThong("DanhSachSach.txt");

                // về menu
                state = SCREEN_READER_MENU;
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        w.draw(label1);
        w.draw(label2);
        w.draw(label3);
        w.draw(label4);
        w.draw(label5);

        boxName.draw(w);
        boxAuthor.draw(w);
        boxCate.draw(w);
        boxYear.draw(w);
        boxNXB.draw(w);

        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif