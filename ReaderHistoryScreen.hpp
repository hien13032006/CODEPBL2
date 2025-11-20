#ifndef READER_HISTORY_SCREEN_H
#define READER_HISTORY_SCREEN_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include "Reader.h"
#include "Button.hpp"
#include "LichSuMuonTra.h"

struct HistoryItem {
    sf::RectangleShape box;
    sf::Text hanhDong;
    sf::Text maSach;
    sf::Text tenSach;
    sf::Text thoiGian;
};

class ReaderHistoryScreen {
private:
    sf::Font &font;
    Reader* reader;

    sf::RectangleShape bg;
    sf::Text header;

    sf::Text colHanhDong, colMaSach, colTenSach, colThoiGian;

    std::vector<HistoryItem> items;

    Button btnBack;
    bool closed;

    float scrollOffset; // cuộn
    float maxScroll;
    sf::View view;      // view để clip vùng hiển thị

    sf::FloatRect viewRect; // vùng hiển thị (clip)

public:
    ReaderHistoryScreen(sf::Font &f, Reader* r);

    void handleEvent(sf::Event &e, const sf::Vector2f &mousePos);
    void render(sf::RenderWindow &window);
    bool wasClosed() const { return closed; }

private:
    void loadFromFile();
};

// ------------------

ReaderHistoryScreen::ReaderHistoryScreen(sf::Font &f, Reader* r)
    : font(f), reader(r), btnBack({1185, 150}, {80,40}, "X", f), closed(false), scrollOffset(0)
{
    // background
    bg.setSize({1000, 600});
    bg.setFillColor(sf::Color(245,245,245,255));
    bg.setOutlineColor(sf::Color(100,100,100));
    bg.setOutlineThickness(2);
    bg.setPosition(270, 150);

    // view clip
    viewRect = sf::FloatRect(185, 280, 1000, 470); // vùng hiển thị danh sách
    view.setViewport(sf::FloatRect(
        viewRect.left / 1000.f, viewRect.top / 900.f,
        viewRect.width / 1000.f, viewRect.height / 900.f
    ));

    // header
    header.setFont(font);
    header.setCharacterSize(36);
    header.setString("LICH SU MUON TRA");
    header.setFillColor(sf::Color::Black);
    header.setPosition(570, 190);

    btnBack.setColor(sf::Color(200,100,100));

    // tiêu đề cột
    colHanhDong.setFont(font); colHanhDong.setFillColor(sf::Color::Black); colHanhDong.setCharacterSize(20); colHanhDong.setString("Hanh dong"); colHanhDong.setPosition(360, 240);
    colMaSach.setFont(font); colMaSach.setFillColor(sf::Color::Black); colMaSach.setCharacterSize(20); colMaSach.setString("Ma sach"); colMaSach.setPosition(510, 240);
    colTenSach.setFont(font); colTenSach.setFillColor(sf::Color::Black); colTenSach.setCharacterSize(20); colTenSach.setString("Ten sach"); colTenSach.setPosition(700, 240);
    colThoiGian.setFont(font); colThoiGian.setFillColor(sf::Color::Black); colThoiGian.setCharacterSize(20); colThoiGian.setString("Thoi gian"); colThoiGian.setPosition(960, 240);

    loadFromFile();
}

void ReaderHistoryScreen::loadFromFile() {
    items.clear();
    std::ifstream in(reader->getFileLichSu());
    if (!in.is_open()) return;

    std::string line;
    float y = 0.f; // tính relative trong view
    bool alternate = false;

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string hanhDong, maDocGia, maSach, tenSach, thoiGian;

        std::getline(ss, hanhDong, '|');
        std::getline(ss, maDocGia, '|'); 
        std::getline(ss, maSach, '|');
        std::getline(ss, tenSach, '|');
        std::getline(ss, thoiGian, '|');

        HistoryItem item;
        item.box.setSize({780,40});
        item.box.setFillColor(alternate ? sf::Color(255,255,255,200) : sf::Color(173,216,230));
        item.box.setOutlineColor(sf::Color(150,150,150));
        item.box.setOutlineThickness(1);
        item.box.setPosition(0, y); // relative
        alternate = !alternate;

        item.hanhDong.setFont(font); item.hanhDong.setCharacterSize(18);
        item.hanhDong.setString(hanhDong); item.hanhDong.setFillColor(sf::Color::Black);
        item.hanhDong.setPosition(10, y+10);

        item.maSach.setFont(font); item.maSach.setCharacterSize(18);
        item.maSach.setString(maSach); item.maSach.setFillColor(sf::Color::Black);
        item.maSach.setPosition(160, y+10);

        item.tenSach.setFont(font); item.tenSach.setCharacterSize(18);
        item.tenSach.setString(tenSach); item.tenSach.setFillColor(sf::Color::Black);
        item.tenSach.setPosition(310, y+10);

        item.thoiGian.setFont(font); item.thoiGian.setCharacterSize(18);
        item.thoiGian.setString(thoiGian); item.thoiGian.setFillColor(sf::Color::Black);
        item.thoiGian.setPosition(580, y+10);

        items.push_back(item);
        y += 50;
    }

    maxScroll = std::max(0.f, y - viewRect.height);
}

void ReaderHistoryScreen::handleEvent(sf::Event &e, const sf::Vector2f &mousePos) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (btnBack.handleClick(mousePos)) closed = true;
    }

    if (e.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= e.mouseWheelScroll.delta * 30.f;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void ReaderHistoryScreen::render(sf::RenderWindow &window) {
    window.draw(bg);
    window.draw(header);
    window.draw(colHanhDong);
    window.draw(colMaSach);
    window.draw(colTenSach);
    window.draw(colThoiGian);

    btnBack.draw(window);

    // lưu view cũ
    sf::View oldView = window.getView();

    sf::View listView = oldView;
    listView.setViewport(sf::FloatRect(
        viewRect.left / 1400.f, viewRect.top / 900.f,
        viewRect.width / 1400.f, viewRect.height / 900.f
    ));
    listView.setSize(viewRect.width, viewRect.height);
    listView.setCenter(viewRect.width/2, viewRect.height/2 + scrollOffset);
    listView.setRotation(0);

    window.setView(listView);

    // render các items relative
    for (auto &item : items) {
        sf::RectangleShape box = item.box;
        box.move(viewRect.left, 0);
        sf::Text h = item.hanhDong, m = item.maSach, t = item.tenSach, th = item.thoiGian;
        h.move(viewRect.left, 0);
        m.move(viewRect.left, 0);
        t.move(viewRect.left, 0);
        th.move(viewRect.left, 0);

        window.draw(box);
        window.draw(h);
        window.draw(m);
        window.draw(t);
        window.draw(th);
    }

    window.setView(oldView);
}

#endif
