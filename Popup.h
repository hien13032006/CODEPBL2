#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.h"


class Popup {
sf::Font font;
sf::RectangleShape overlay;
sf::RectangleShape box;
sf::Text text;


bool visible = false;
sf::Clock timer;
float anim = 0.f; // 0..1


public:
Popup(){
font.loadFromFile("assets/DejaVuSans.ttf");
overlay.setFillColor(sf::Color(0,0,0,120));
box.setSize({520.f, 120.f});
box.setFillColor(Theme::Panel);
box.setOutlineColor(Theme::PanelOutline);
box.setOutlineThickness(3.f);
text.setFont(font);
text.setCharacterSize(22);
text.setFillColor(Theme::Text);
}


void show(const std::string& s){ text.setString(s); visible = true; anim = 0.f; timer.restart(); }
void hide(){ visible = false; }
bool isVisible() const { return visible; }


void update(const sf::RenderWindow& w){
if(!visible) return;
float t = timer.getElapsedTime().asSeconds();
anim = (t < 0.2f) ? (t/0.2f) : 1.f;
overlay.setSize( sf::Vector2f((float)w.getSize().x, (float)w.getSize().y) );


sf::Vector2f center( w.getSize().x*0.5f, w.getSize().y*0.5f );
auto tb = text.getLocalBounds();
box.setOrigin(box.getSize()*0.5f);
box.setPosition(center);
text.setPosition(center.x - tb.width*0.5f - tb.left, center.y - tb.height*0.5f - tb.top);
}


void draw(sf::RenderTarget& t){
if(!visible) return;
t.draw(overlay);
sf::RenderStates st; sf::Transform tr; float s = 0.85f + 0.15f*anim;
tr.translate(box.getPosition()); tr.scale(s,s); tr.translate(-box.getPosition()); st.transform = tr;
t.draw(box, st); t.draw(text, st);
}
};