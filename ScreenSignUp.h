#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include "Button.h"
#include "Popup.h"
#include "Reader.h"
#include "Node.h"

class ScreenSignup {
    sf::Font font;
    sf::Text title;

    sf::Text lbHoTen, lbSDT, lbEmail, lbUser, lbPass;
    sf::RectangleShape bxHoTen, bxSDT, bxEmail, bxUser, bxPass;
    sf::String inHoTen, inSDT, inEmail, inUser, inPass;

    bool fHoTen=false, fSDT=false, fEmail=false, fUser=false, fPass=false;

    Button btnOK, btnBack;
    Popup popup;

    NodeReader*& headReaders;
    std::function<void()> onBack;  // quay về login

public:
    ScreenSignup(NodeReader*& readers,
                 std::function<void()> backFn)
        : headReaders(readers), onBack(backFn),
          btnOK(), btnBack()
    {}

    void init() {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString("DANG KY READER");
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(400, 30);

        initLabel(lbHoTen, "Ho ten",   300,120);
        initLabel(lbSDT,   "SDT",      300,190);
        initLabel(lbEmail, "Email",    300,260);
        initLabel(lbUser,  "Username", 300,330);
        initLabel(lbPass,  "Password", 300,400);

        initBox(bxHoTen,  300,150);
        initBox(bxSDT,    300,220);
        initBox(bxEmail,  300,290);
        initBox(bxUser,   300,360);
        initBox(bxPass,   300,430);

        btnOK   = Button(font,"TAO TAI KHOAN",350,520);
        btnBack = Button(font,"BACK",         600,520);

        btnOK.setCallback([&](){ onOK(); });
        btnBack.setCallback([&](){ onBack(); });
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);

        drawBox(t, bxHoTen, inHoTen);
        drawBox(t, bxSDT,   inSDT);
        drawBox(t, bxEmail, inEmail);
        drawBox(t, bxUser,  inUser);
        drawBox(t, bxPass,  inPass);

        t.draw(lbHoTen); t.draw(lbSDT);
        t.draw(lbEmail); t.draw(lbUser);
        t.draw(lbPass);

        btnOK.draw(t);
        btnBack.draw(t);

        popup.draw(t);
    }

    void update(const sf::RenderWindow& w) {
        btnOK.update(w);
        btnBack.update(w);
    }

    void handle(const sf::Event& evt) {
        btnOK.handle(evt);
        btnBack.handle(evt);

        if(evt.type == sf::Event::MouseButtonPressed &&
           evt.mouseButton.button == sf::Mouse::Left)
        {
            auto mp = sf::Mouse::getPosition();
            sf::Vector2f p(mp.x,mp.y);
            fHoTen = bxHoTen.getGlobalBounds().contains(p);
            fSDT   = bxSDT.getGlobalBounds().contains(p);
            fEmail = bxEmail.getGlobalBounds().contains(p);
            fUser  = bxUser.getGlobalBounds().contains(p);
            fPass  = bxPass.getGlobalBounds().contains(p);
        }

        if(evt.type == sf::Event::TextEntered) {
            pushText(evt,fHoTen, inHoTen);
            pushText(evt,fSDT,   inSDT);
            pushText(evt,fEmail, inEmail);
            pushText(evt,fUser,  inUser);
            pushText(evt,fPass,  inPass);
        }
    }

private:

    void initLabel(sf::Text& t, const std::string& s, float x, float y) {
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Black);
        t.setPosition(x,y);
    }

    void initBox(sf::RectangleShape& r, float x, float y) {
        r.setSize({450,40});
        r.setFillColor(sf::Color::White);
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(1);
        r.setPosition(x,y);
    }

    void drawBox(sf::RenderTarget& t, const sf::RectangleShape& box, const sf::String& s) {
        t.draw(box);
        sf::Text tt(s,font,20);
        tt.setFillColor(sf::Color::Black);
        tt.setPosition(box.getPosition().x+5, box.getPosition().y+7);
        t.draw(tt);
    }

    void pushText(const sf::Event& evt, bool f, sf::String& s) {
        if(!f) return;
        if(evt.text.unicode == 8) {
            if(!s.isEmpty()) s.erase(s.getSize()-1,1);
        }
        else if(evt.text.unicode >= 32 && evt.text.unicode <= 126) {
            s += (char)evt.text.unicode;
        }
    }

    bool userExists(const std::string& user) {
        NodeReader* p = headReaders;
        while(p) {
            if(p->data.getUsername() == user) return true;
            p = p->next;
        }
        return false;
    }

    std::string newMaReader() {
        int cnt = 1;
        NodeReader* p = headReaders;
        while(p){
            cnt++;
            p = p->next;
        }

        char buf[10];
        sprintf(buf,"R%04d",cnt);
        return std::string(buf);
    }

    void onOK() {
        if(inHoTen.isEmpty() || inSDT.isEmpty() || inEmail.isEmpty()
        || inUser.isEmpty()  || inPass.isEmpty())
        {
            popup.show("Thieu thong tin!");
            return;
        }

        std::string user = inUser.toAnsiString();
        if(userExists(user)) {
            popup.show("User da ton tai!");
            return;
        }

        // tạo reader
        Reader r;
        r.setMaReader(newMaReader());
        r.setHoTen(inHoTen.toAnsiString());
        r.setSDT(inSDT.toAnsiString());
        r.setEmail(inEmail.toAnsiString());
        r.setUsername(user);
        r.setPassword(inPass.toAnsiString());

        // thêm vào danh sách
        NodeReader* node = new NodeReader(r);
        node->next = headReaders;
        headReaders = node;

        // ghi file
        std::ofstream out("Reader.txt", std::ios::app);
        out << r.getMaID() << "|" << r.getHoTen() << "|" << r.getSDT()
            << "|" << r.getUsername() << "|" << inPass.toAnsiString() << "\n";
        out.close();

        popup.show("Tao tai khoan thanh cong!");
        clear();
    }

    void clear() {
        inHoTen.clear();
        inSDT.clear();
        inEmail.clear();
        inUser.clear();
        inPass.clear();
    }
};
