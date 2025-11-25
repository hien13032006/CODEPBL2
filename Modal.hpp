#ifndef MODAL_HPP
#define MODAL_HPP

#include <SFML/Graphics.hpp>

class Modal {
private:
    sf::RectangleShape overlay;
    bool visible;

public:
    Modal(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1300, 720)); // Cập nhật kích thước chuẩn
        overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Đen mờ
        visible = false;
    }

    void show() { visible = true; }
    void hide() { visible = false; }
    bool isShown() const { return visible; }
    
    bool handleClose(sf::Vector2f mousePos) { return false; }

    void draw(sf::RenderWindow& window) {
        if (visible) window.draw(overlay);
    }
};
#endif