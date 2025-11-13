#ifndef SCREEN_LIB_UPDATE_H
#define SCREEN_LIB_UPDATE_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include <SFML/Graphics.hpp>
#include <string>

class ScreenLibUpdate : public ScreenBase {
private:
    LibrarySystem* lib;

    // label
    sf::Text title;
    sf::Text lID, lName, lAuthor, lCate, lYear, lNXB;

    // input
    TextBox boxID;
    TextBox boxName;
    TextBox boxAuthor;
    TextBox boxCate;
    TextBox boxYear;
    TextBox boxNXB;

    // button
    Button btnOK, btnBack;

public:
    ScreenLibUpdate(sf::Font &font, LibrarySystem *L) {
        lib = L;

        float px = 360.f;      // goc x
        float py = 140.f;      // goc y
        float dy = 58.f;       // step

        title.setFont(font);
        title.setString("Cap nhat sach");
        title.setCharacterSize(28);
        title.setPosition(px - 140, py - 70);

        lID.setFont(font);     lID.setString("Ma sach");   lID.setCharacterSize(20); lID.setPosition(px-220, py+dy*0);
        lName.setFont(font);   lName.setString("Ten sach");lName.setCharacterSize(20);lName.setPosition(px-220, py+dy*1);
        lAuthor.setFont(font); lAuthor.setString("Tac gia");lAuthor.setCharacterSize(20);lAuthor.setPosition(px-220, py+dy*2);
        lCate.setFont(font);   lCate.setString("The loai");lCate.setCharacterSize(20); lCate.setPosition(px-220, py+dy*3);
        lYear.setFont(font);   lYear.setString("Nam XB");  lYear.setCharacterSize(20); lYear.setPosition(px-220, py+dy*4);
        lNXB.setFont(font);    lNXB.setString("Nha XB");   lNXB.setCharacterSize(20);  lNXB.setPosition(px-220, py+dy*5);

        boxID     = TextBox(font, {px, py+dy*0}, {330,40});
        boxName   = TextBox(font, {px, py+dy*1}, {330,40});
        boxAuthor = TextBox(font, {px, py+dy*2}, {330,40});
        boxCate   = TextBox(font, {px, py+dy*3}, {330,40});
        boxYear   = TextBox(font, {px, py+dy*4}, {330,40});
        boxNXB    = TextBox(font, {px, py+dy*5}, {330,40});

        btnOK   = Button("Cap nhat", font, {px,        py+dy*7}, {200,50});
        btnBack = Button("Quay lai", font, {px+220.f,  py+dy*7}, {200,50});
    }

    void handleEvent(sf::Event &e, AppState &state, sf::RenderWindow *w) override {
        boxID.handleEvent(e);
        boxName.handleEvent(e);
        boxAuthor.handleEvent(e);
        boxCate.handleEvent(e);
        boxYear.handleEvent(e);
        boxNXB.handleEvent(e);

        if (e.type == sf::Event::MouseButtonPressed) {
            float mx = e.mouseButton.x, my = e.mouseButton.y;

            if (btnBack.hit(mx,my)) {
                state = SCREEN_LIB_MENU;
                return;
            }

            if (btnOK.hit(mx,my)) {
                std::string id   = boxID.getText();
                std::string ten  = boxName.getText();
                std::string tac  = boxAuthor.getText();
                std::string loai = boxCate.getText();
                std::string nam  = boxYear.getText();
                std::string nxb  = boxNXB.getText();

                if (!id.empty()) {
                    int namXB = 0;
                    if (!nam.empty()) {
                        try { namXB = std::stoi(nam); } catch(...) { namXB = 0; }
                    }

                    // 1) cap nhat cac truong co trong updateBook()
                    //    (ban da khai bao: bool updateBook(const string& id, string tenMoi, string tacGiaMoi, int namMoi, int soLuongMoi))
                    lib->updateBook(id, ten, tac, namXB, 1);

                    // 2) bo sung the loai / nha XB neu ham updateBook khong support
                    Sach* s = lib->findBookByID(id);
                    if (s) {
                        if (!loai.empty()) s->setTheLoai(loai);
                        if (!nxb.empty())  s->setNhaXuatBan(nxb);
                    }

                    lib->GhiFileHeThong("DanhSachSach.txt");
                }

                state = SCREEN_LIB_MENU;
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        w.draw(title);
        w.draw(lID);   w.draw(lName); w.draw(lAuthor);
        w.draw(lCate); w.draw(lYear); w.draw(lNXB);

        boxID.draw(w);
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