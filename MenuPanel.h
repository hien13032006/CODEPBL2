#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.h"

class MenuPanel {
protected:
    sf::Font font;
    sf::Text title;
    sf::RectangleShape panel;

public:
    void initBase(const std::string& titleStr)
    {
        font.loadFromFile("assets/DejaVuSans.ttf");

        title.setFont(font);
        title.setString(titleStr);
        title.setCharacterSize(34);
        title.setFillColor(Theme::Title);
        title.setStyle(sf::Text::Bold);
        title.setPosition(0, 60);

        panel.setSize({600.f, 500.f});
        panel.setFillColor(Theme::Panel);
        panel.setOutlineColor(Theme::PanelOutline);
        panel.setOutlineThickness(3.f);
        panel.setPosition(300.f, 130.f);
    }

    void drawBase(sf::RenderTarget& t, float winW)
    {
        Theme::centerTextX(title, winW);
        t.draw(panel);
        t.draw(title);
    }

    void drawDivider(sf::RenderTarget& t, float x, float y, float w)
    {
        sf::RectangleShape line({w, 2});
        line.setFillColor(sf::Color(200, 205, 255));
        line.setPosition(x, y);
        t.draw(line);
    }
};