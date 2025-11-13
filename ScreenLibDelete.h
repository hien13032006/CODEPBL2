#ifndef SCREEN_LIB_DELETE_H
#define SCREEN_LIB_DELETE_H

#include "ScreenBase.h"
#include "Textbox.h"
#include "Button.h"
#include "LibrarySystem.h"
#include <SFML/Graphics.hpp>

class ScreenLibDelete : public ScreenBase {
private:
    LibrarySystem* lib;

    sf::Text lblID, lblTitle;
    TextBox  boxID;
    Button   btnOK, btnBack;

public:
    ScreenLibDelete(sf::Font &font, LibrarySystem *L) {
        lib = L;

        lblTitle.setFont(font);
        lblTitle.setString("Xoa sach");
        lblTitle.setCharacterSize(28);
        lblTitle.setPosition(320, 70);

        lblID.setFont(font);
        lblID.setString("Ma sach");
        lblID.setCharacterSize(22);
        lblID.setPosition(220, 160);

        boxID  = TextBox(font, {320,155}, {320,42});
        btnOK  = Button("Xoa",      font, {320,230}, {180,50});
        btnBack= Button("Quay lai", font, {320,300}, {180,50});
    }

    void handleEvent(sf::Event &e, AppState &state, sf::RenderWindow *w) override {
        boxID.handleEvent(e);

        if (e.type == sf::Event::MouseButtonPressed) {
            float mx = e.mouseButton.x, my = e.mouseButton.y;

            if (btnBack.hit(mx,my)) {
                state = SCREEN_LIB_MENU;   // quay ve menu thu thu
                return;
            }

            if (btnOK.hit(mx,my)) {
                std::string id = boxID.getText();
                if (!id.empty()) {
                    // Ham XoaSach tra ve bool (true neu xoa thanh cong)
                    bool ok = lib->XoaSach(id);
                    // Cap nhat file he thong
                    lib->GhiFileHeThong("DanhSachSach.txt");
                    // co the hien popup neu ban da co Popup
                }
                state = SCREEN_LIB_MENU;
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &w) override {
        w.draw(lblTitle);
        w.draw(lblID);
        boxID.draw(w);
        btnOK.draw(w);
        btnBack.draw(w);
    }
};

#endif