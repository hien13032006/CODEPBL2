#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Button.h"

class ScreenLogin {
    sf::Font font;
    sf::Text title, lbUser, lbPass;
    sf::RectangleShape boxUser, boxPass;
    sf::String inputUser, inputPass;
    bool focusUser = false, focusPass = false;

    Button btnLogin;
    Button btnBack;
    std::function<void(const std::string&, const std::string&)> onLogin;
    std::function<void()> onBack;

public:
    ScreenLogin(const std::string& titleText,
                std::function<void(const std::string&, const std::string&)> loginFn,
                std::function<void()> backFn)
        : onLogin(loginFn), onBack(backFn)
    {
        (void)titleText;
    }

    void init(const std::string& titleStr) {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString(titleStr);
        title.setCharacterSize(32);
        title.setFillColor(sf::Color(20,20,80));
        title.setPosition(400, 80);

        lbUser.setFont(font); lbUser.setString("Username:"); lbUser.setCharacterSize(22); lbUser.setPosition(350, 200);
        lbPass.setFont(font); lbPass.setString("Password:"); lbPass.setCharacterSize(22); lbPass.setPosition(350, 280);

        boxUser.setSize({400, 40}); boxUser.setPosition(350, 230);
        boxUser.setFillColor(sf::Color::White); boxUser.setOutlineColor(sf::Color::Black); boxUser.setOutlineThickness(1);
        boxPass.setSize({400, 40}); boxPass.setPosition(350, 310);
        boxPass.setFillColor(sf::Color::White); boxPass.setOutlineColor(sf::Color::Black); boxPass.setOutlineThickness(1);

        btnLogin = Button(font, "LOGIN", 420, 390);
        btnBack  = Button(font, "BACK",  420, 450);

        btnLogin.setCallback([&](){
            onLogin(inputUser.toAnsiString(), inputPass.toAnsiString());
        });
        btnBack.setCallback([&](){
            onBack();
        });
    }

    void handle(const sf::Event& evt) {
        btnLogin.handle(evt);
        btnBack.handle(evt);

        if (evt.type == sf::Event::MouseButtonPressed) {
            auto mpos = sf::Mouse::getPosition();
            sf::Vector2f p((float)mpos.x, (float)mpos.y);
            focusUser = boxUser.getGlobalBounds().contains(p);
            focusPass = boxPass.getGlobalBounds().contains(p);
        }

        if (evt.type == sf::Event::TextEntered) {
            if (focusUser) {
                if (evt.text.unicode == 8) { if (!inputUser.isEmpty()) inputUser.erase(inputUser.getSize()-1,1); }
                else if (evt.text.unicode >= 32 && evt.text.unicode <= 126) inputUser += (char)evt.text.unicode;
            }
            else if (focusPass) {
                if (evt.text.unicode == 8) { if (!inputPass.isEmpty()) inputPass.erase(inputPass.getSize()-1,1); }
                else if (evt.text.unicode >= 32 && evt.text.unicode <= 126) inputPass += (char)evt.text.unicode;
            }
        }
    }

    void update(const sf::RenderWindow& w) {
        btnLogin.update(w);
        btnBack.update(w);
    }

    void draw(sf::RenderTarget& t) {
        t.draw(title);
        t.draw(lbUser);
        t.draw(lbPass);
        t.draw(boxUser);
        t.draw(boxPass);

        sf::Text u(inputUser, font, 20);
        u.setFillColor(sf::Color::Black);
        u.setPosition(355,236);
        t.draw(u);

        std::string pw(inputPass.getSize(), '*');
        sf::Text p(pw.c_str(), font, 20);
        p.setFillColor(sf::Color::Black);
        p.setPosition(355,316);
        t.draw(p);

        btnLogin.draw(t);
        btnBack.draw(t);
    }
};
