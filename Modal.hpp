#ifndef MODAL_HPP
#define MODAL_HPP

#include <SFML/Graphics.hpp>
#include "RoundedRectangle.hpp" // Nếu muốn modal bo góc

class Modal {
private:
    sf::RectangleShape overlay;
    // RoundedRectangleShape box; // Có thể dùng box nền ở đây hoặc vẽ riêng trong từng screen
    bool visible;

public:
    Modal(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Đen mờ
        visible = false;
    }

    void show() { visible = true; }
    void hide() { visible = false; }
    bool isShown() const { return visible; }

    // Hàm tiện ích để check click ra ngoài (đóng modal)
    bool handleClose(sf::Vector2f mousePos) {
        // Logic này nên để từng Screen tự xử lý nếu click nút X
        return false; 
    }

    RoundedRectangleShape& getModalBox() { 
        // Helper cho code cũ, nhưng code mới các screen tự vẽ panel
        static RoundedRectangleShape dummy; 
        return dummy; 
    }

    void draw(sf::RenderWindow& window) {
        if (visible) {
            window.draw(overlay);
        }
    }
};

#endif