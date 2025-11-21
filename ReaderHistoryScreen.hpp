#ifndef READER_HISTORY_SCREEN_HPP
#define READER_HISTORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"

struct HistoryItem {
    RoundedRectangleShape box; sf::Text hanhDong; sf::Text maSach; sf::Text tenSach; sf::Text thoiGian;
};

class ReaderHistoryScreen {
private:
    sf::Font &font; Reader* reader;
    sf::RectangleShape bgOverlay, panel; sf::Text header;
    sf::Text colHanhDong, colMaSach, colTenSach, colThoiGian;
    std::vector<HistoryItem> items; Button btnBack; bool closed;
    ScrollView* scrollView;
    const float LIST_START_Y = 250.0f; const float VIEW_HEIGHT = 500.0f;

public:
    ReaderHistoryScreen(sf::Font &f, Reader* r) : font(f), reader(r), btnBack({1150, 160}, {50, 50}, "X", f, 0, sf::Color(200, 80, 80)), closed(false) {
        bgOverlay.setSize({1400, 900}); bgOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        panel.setSize({1000, 700}); panel.setFillColor(sf::Color(30, 35, 50));
        panel.setOutlineThickness(2); panel.setOutlineColor(sf::Color(100, 100, 120)); panel.setPosition(200, 100);

        header.setFont(font); header.setCharacterSize(32); header.setString("LICH SU MUON TRA");
        header.setFillColor(sf::Color::White); header.setPosition(550, 140);

        auto setupCol = [&](sf::Text& txt, std::string s, float x) {
            txt.setFont(font); txt.setString(s); txt.setCharacterSize(18); txt.setFillColor(sf::Color(255, 193, 94)); txt.setPosition(x, 210);
        };
        setupCol(colHanhDong, "HANH DONG", 250); setupCol(colMaSach, "MA SACH", 400);
        setupCol(colTenSach, "TEN SACH", 550); setupCol(colThoiGian, "THOI GIAN", 900);

        scrollView = new ScrollView(sf::FloatRect(200, LIST_START_Y, 1000, VIEW_HEIGHT));
        loadFromFile();
    }

    ~ReaderHistoryScreen() { delete scrollView; }

    void reset() { closed = false; loadFromFile(); scrollView->reset(); }

    void loadFromFile() {
        items.clear(); if (!reader) return;
        std::ifstream in(reader->getFileLichSu()); if (!in.is_open()) return;
        std::string line; float itemY = LIST_START_Y; const float ITEM_HEIGHT = 60.0f;

        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string hanhDong, maDocGia, maSach, tenSach, thoiGian;
            std::getline(ss, hanhDong, '|'); std::getline(ss, maDocGia, '|'); std::getline(ss, maSach, '|');
            std::getline(ss, tenSach, '|'); std::getline(ss, thoiGian, '|');

            HistoryItem item;
            item.box.setSize({900, ITEM_HEIGHT - 10}); item.box.setCornerRadius(8.0f);
            item.box.setPosition(250, itemY); item.box.setFillColor(sf::Color(40, 45, 60));

            auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = sf::Color::White) {
                txt.setFont(font); txt.setString(s); txt.setCharacterSize(16); txt.setFillColor(c);
                txt.setPosition(x, itemY + (ITEM_HEIGHT-10)/2 - 10);
            };
            setupText(item.hanhDong, hanhDong, 260, (hanhDong == "Muon") ? sf::Color(255, 193, 94) : sf::Color(100, 220, 100));
            setupText(item.maSach, maSach, 400);
            if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
            setupText(item.tenSach, tenSach, 550);
            setupText(item.thoiGian, thoiGian, 900, sf::Color(180, 180, 180));

            items.push_back(item); itemY += ITEM_HEIGHT;
        }
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_START_Y));
    }

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) {
        if (e.type == sf::Event::MouseButtonPressed && btnBack.handleClick(mousePos)) closed = true;
        scrollView->handleScroll(e, mousePos);
    }
    void update(sf::Vector2f mousePos) { btnBack.update(mousePos); }

    void render(sf::RenderWindow &window) {
        window.setView(window.getDefaultView());
        window.draw(bgOverlay); window.draw(panel); window.draw(header);
        window.draw(colHanhDong); window.draw(colMaSach); window.draw(colTenSach); window.draw(colThoiGian);
        btnBack.draw(window);

        sf::View listView = window.getDefaultView();
        listView.setViewport(sf::FloatRect(200.f/1400.f, LIST_START_Y/900.f, 1000.f/1400.f, VIEW_HEIGHT/900.f));
        listView.setSize(1000, VIEW_HEIGHT);
        listView.setCenter(200 + 1000/2, LIST_START_Y + VIEW_HEIGHT/2 + scrollView->getScrollOffset());
        window.setView(listView);

        for (const auto &item : items) {
            window.draw(item.box); window.draw(item.hanhDong); window.draw(item.maSach);
            window.draw(item.tenSach); window.draw(item.thoiGian);
        }
        window.setView(window.getDefaultView());
    }
    bool wasClosed() const { return closed; }
};

#endif