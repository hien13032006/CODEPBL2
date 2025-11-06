#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "Button.h"
#include "Popup.h"
#include "Theme.h"


class ScreenLoginBase {
protected:
sf::Font font;
sf::Text title, lbUser, lbPass;
sf::RectangleShape boxUser, boxPass;
sf::String inUser, inPass;
bool fUser=false, fPass=false;
Button btnLogin, btnBack;
Popup popup;


public:
virtual void onLogin() {}
virtual void onBack() {}
virtual std::string titleText() const { return "LOGIN"; }


void init(){
font.loadFromFile("assets/DejaVuSans.ttf");
title.setFont(font); title.setString(titleText()); title.setCharacterSize(36);
title.setFillColor(Theme::Title); title.setStyle(sf::Text::Bold); title.setPosition(0, 70);
setupLabel(lbUser, "Username:", 360, 220); setupLabel(lbPass, "Password:", 360, 300);
setupBox(boxUser, 360, 255); setupBox(boxPass, 360, 335);
btnLogin = Button(font, "DANG NHAP", 380, 420); btnBack = Button(font, "BACK", 380, 480);
btnLogin.setCallback([&](){ onLogin(); }); btnBack.setCallback([&](){ onBack(); });
}


void handle(sf::RenderWindow& w, const sf::Event& evt){
btnLogin.handle(evt); btnBack.handle(evt);
if(evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left){
auto m = sf::Mouse::getPosition(w); sf::Vector2f p((float)m.x,(float)m.y);
fUser = boxUser.getGlobalBounds().contains(p); fPass = boxPass.getGlobalBounds().contains(p);
}
if(evt.type == sf::Event::TextEntered){ pushText(evt, fUser, inUser); pushText(evt, fPass, inPass); }
}


void update(sf::RenderWindow& w){ btnLogin.update(w); btnBack.update(w); popup.update(w); Theme::centerTextX(title, w.getSize().x); }


void draw(sf::RenderTarget& t){ t.draw(title); t.draw(lbUser); t.draw(lbPass); drawBox(t, boxUser, inUser); drawBox(t, boxPass, mask(inPass)); btnLogin.draw(t); btnBack.draw(t); popup.draw(t); }


protected:
void setupLabel(sf::Text& t, const std::string& s, float x, float y){ t.setFont(font); t.setString(s); t.setCharacterSize(22); t.setFillColor(Theme::Text); t.setPosition(x,y); }
void setupBox(sf::RectangleShape& r, float x, float y){ r.setSize({500,45}); r.setFillColor(Theme::InputFill); r.setOutlineColor(Theme::InputOutline); r.setOutlineThickness(2); r.setPosition(x,y); }
void pushText(const sf::Event& evt, bool f, sf::String& s){ if(!f) return; if(evt.text.unicode==8){ if(!s.isEmpty()) s.erase(s.getSize()-1,1);} else if(evt.text.unicode>=32 && evt.text.unicode<=126){ s += (char)evt.text.unicode; } }
void drawBox(sf::RenderTarget& t, const sf::RectangleShape& r, const sf::String& s){ t.draw(r); sf::Text tt(s, font, 22); tt.setFillColor(sf::Color::Black); tt.setPosition(r.getPosition().x+6, r.getPosition().y+8); t.draw(tt); }
sf::String mask(const sf::String& s){ std::string r(s.getSize(), '*'); return sf::String(r); }
};