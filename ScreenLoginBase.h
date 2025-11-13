#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include "Button.h"
#include "Popup.h"
#include "Theme.h"
#include "ScreenBase.h"

class ScreenLoginBase : public ScreenBase {
protected:
    sf::Font font;
    sf::Text title, lbUser, lbPass;
    sf::RectangleShape boxUser, boxPass;
    sf::String inUser, inPass;
    bool fUser = false, fPass = false;
    Button btnLogin, btnBack;
    Popup popup;

public:
    ScreenLoginBase() : popup(font) {} // khởi tạo popup với font

    virtual void onLogin() {}
    virtual void onBack() {}
    virtual std::string titleText() const { return "LOGIN"; }

    void init() {
        font.loadFromFile("DejaVuSans.ttf");
        title.setFont(font);
        title.setString(titleText());
        title.setCharacterSize(36);
        title.setFillColor(Theme::Title);
        title.setStyle(sf::Text::Bold);
        title.setPosition(0, 70);

        setupLabel(lbUser, "Username:", 360, 220);
        setupLabel(lbPass, "Password:", 360, 300);
        setupBox(boxUser, 360, 255);
        setupBox(boxPass, 360, 335);

        btnLogin = Button("DANG NHAP", font, sf::Vector2f(380, 420), sf::Vector2f(200, 50));
        btnBack  = Button("BACK", font, sf::Vector2f(380, 480), sf::Vector2f(200, 50));

        btnLogin.setCallback([&]() { onLogin(); });
        btnBack.setCallback([&]() { onBack(); });
    }

    void handleEvent(sf::Event& evt, AppState& state, sf::RenderWindow* w) override {
        btnLogin.handleEvent(evt, *w);
        btnBack.handleEvent(evt, *w);

        if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
            auto m = sf::Mouse::getPosition(*w);
            sf::Vector2f p((float)m.x, (float)m.y);
            fUser = boxUser.getGlobalBounds().contains(p);
            fPass = boxPass.getGlobalBounds().contains(p);
        }
        if (evt.type == sf::Event::TextEntered) {
            pushText(evt, fUser, inUser);
            pushText(evt, fPass, inPass);
        }
    }

    void update(sf::RenderWindow& w) {
        btnLogin.update();
        btnBack.update();
        popup.update(0.f); // dt = 0 nếu chưa có frame time
        Theme::centerTextX(title, w.getSize().x);
    }

    void draw(sf::RenderWindow& w) override {
        w.draw(title);
        w.draw(lbUser);
        w.draw(lbPass);
        drawBox(w, boxUser, inUser);
        drawBox(w, boxPass, mask(inPass));
        btnLogin.draw(w);
        btnBack.draw(w);
        popup.draw(w);
    }

protected:

void handleEventBase(sf::Event& evt, AppState& state, sf::RenderWindow* w) {
        btnLogin.handleEvent(evt, *w);
        btnBack.handleEvent(evt, *w);

        if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
            auto m = sf::Mouse::getPosition(*w);
            sf::Vector2f p((float)m.x, (float)m.y);
            fUser = boxUser.getGlobalBounds().contains(p);
            fPass = boxPass.getGlobalBounds().contains(p);
        }
        if (evt.type == sf::Event::TextEntered) {
            pushText(evt, fUser, inUser);
            pushText(evt, fPass, inPass);
        }
    }

    void setupLabel(sf::Text& t, const std::string& s, float x, float y) {
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(22);
        t.setFillColor(Theme::Text);
        t.setPosition(x, y);
    }

    void setupBox(sf::RectangleShape& r, float x, float y) {
        r.setSize({500, 45});
        r.setFillColor(Theme::InputFill);
        r.setOutlineColor(Theme::InputOutline);
        r.setOutlineThickness(2);
        r.setPosition(x, y);
    }

    void pushText(const sf::Event& evt, bool active, sf::String& s) {
        if (!active) return;
        if (evt.text.unicode == 8) { // backspace
            if (!s.isEmpty())
                s.erase(s.getSize() - 1, 1);
        } else if (evt.text.unicode >= 32 && evt.text.unicode <= 126) {
            s += static_cast<char>(evt.text.unicode);
        }
    }

    void drawBox(sf::RenderWindow& w, const sf::RectangleShape& r, const sf::String& s) {
        w.draw(r);
        sf::Text tt(s, font, 22);
        tt.setFillColor(sf::Color::Black);
        tt.setPosition(r.getPosition().x + 6, r.getPosition().y + 8);
        w.draw(tt);
    }

    sf::String mask(const sf::String& s) {
        return sf::String(std::string(s.getSize(), '*'));
    }
};
