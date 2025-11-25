#ifndef READER_HISTORY_SCREEN_HPP
#define READER_HISTORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include "Reader.h"
#include "Button.hpp"
#include "RoundedRectangle.hpp"
#include "ScrollView.hpp"
#include "Theme.hpp"

struct HistoryItem { RoundedRectangleShape box; sf::Text hanhDong, maSach, tenSach, thoiGian; };

class ReaderHistoryScreen {
private:
    sf::Font &font; Reader* reader; sf::RectangleShape bgOverlay, panel; sf::Text header, colHanhDong, colMaSach, colTenSach, colThoiGian;
    std::vector<HistoryItem> items; Button btnBack; bool closed; ScrollView* scrollView;
    const float LIST_START_Y = 200.0f; const float VIEW_HEIGHT = 400.0f;

public:
    ReaderHistoryScreen(sf::Font &f, Reader* r) : font(f), reader(r), btnBack({1050, 110}, {50, 50}, "X", f, 0, Theme::Error), closed(false) {
        bgOverlay.setSize({1300, 720}); bgOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        panel.setSize({1000, 600}); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(150, 60);
        header.setFont(font); header.setCharacterSize(32); header.setString("LICH SU MUON TRA"); header.setFillColor(Theme::TextDark); header.setPosition(450, 110);
        auto setupCol = [&](sf::Text& txt, std::string s, float x) { txt.setFont(font); txt.setString(s); txt.setCharacterSize(18); txt.setFillColor(Theme::Secondary); txt.setPosition(x, 160); };
        setupCol(colHanhDong, "HANH DONG", 200); setupCol(colMaSach, "MA SACH", 350); setupCol(colTenSach, "TEN SACH", 500); setupCol(colThoiGian, "THOI GIAN", 850);
        scrollView = new ScrollView(sf::FloatRect(150, LIST_START_Y, 1000, VIEW_HEIGHT));
        loadFromFile();
    }
    ~ReaderHistoryScreen() { delete scrollView; }
    void reset() { closed = false; loadFromFile(); scrollView->reset(); }

    void loadFromFile() {
        items.clear(); if (!reader) return; std::ifstream in(reader->getFileLichSu()); if (!in.is_open()) return;
        std::string line; float itemY = LIST_START_Y; const float ITEM_HEIGHT = 60.0f;
        while (std::getline(in, line)) {
            if (line.empty()) continue; std::stringstream ss(line); std::string hanhDong, maDocGia, maSach, tenSach, thoiGian;
            std::getline(ss, hanhDong, '|'); std::getline(ss, maDocGia, '|'); std::getline(ss, maSach, '|'); std::getline(ss, tenSach, '|'); std::getline(ss, thoiGian, '|');
            HistoryItem item; item.box.setSize({900, ITEM_HEIGHT - 10}); item.box.setCornerRadius(8.0f); item.box.setPosition(200, itemY); item.box.setFillColor(Theme::Background);
            auto setupText = [&](sf::Text& txt, std::string s, float x, sf::Color c = Theme::TextDark) { txt.setFont(font); txt.setString(s); txt.setCharacterSize(16); txt.setFillColor(c); txt.setPosition(x, itemY + (ITEM_HEIGHT-10)/2 - 10); };
            setupText(item.hanhDong, hanhDong, 210, (hanhDong == "Muon") ? Theme::Primary : Theme::Success); setupText(item.maSach, maSach, 350);
            if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "..."; setupText(item.tenSach, tenSach, 500); setupText(item.thoiGian, thoiGian, 850, Theme::TextLight);
            items.push_back(item); itemY += ITEM_HEIGHT;
        }
        // FIX: Padding 600.0f
        scrollView->setMaxScroll(std::max(0.0f, itemY - LIST_START_Y + 600.0f));
    }
    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos) { if (e.type == sf::Event::MouseButtonPressed && btnBack.handleClick(mousePos)) closed = true; scrollView->handleScroll(e, mousePos); }
    void update(sf::Vector2f mousePos) { btnBack.update(mousePos); }
    void render(sf::RenderWindow &window) {
        window.setView(window.getDefaultView()); window.draw(bgOverlay); window.draw(panel); window.draw(header); window.draw(colHanhDong); window.draw(colMaSach); window.draw(colTenSach); window.draw(colThoiGian); btnBack.draw(window);
        sf::View listView = window.getDefaultView(); listView.setViewport(sf::FloatRect(150.f/1300.f, LIST_START_Y/720.f, 1000.f/1300.f, VIEW_HEIGHT/720.f)); listView.setSize(1000, VIEW_HEIGHT); listView.setCenter(150 + 1000/2, LIST_START_Y + VIEW_HEIGHT/2 + scrollView->getScrollOffset()); window.setView(listView);
        for (const auto &item : items) { window.draw(item.box); window.draw(item.hanhDong); window.draw(item.maSach); window.draw(item.tenSach); window.draw(item.thoiGian); } window.setView(window.getDefaultView());
    }
    bool wasClosed() const { return closed; }
};
#endif