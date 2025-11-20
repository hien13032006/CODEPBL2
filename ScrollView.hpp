#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <SFML/Graphics.hpp>

class ScrollView {
private:
    float scrollOffset;
    float maxScroll;
    sf::FloatRect bounds;
    float scrollSpeed;

public:
    ScrollView(sf::FloatRect viewBounds) : bounds(viewBounds) {
        scrollOffset = 0;
        maxScroll = 0;
        scrollSpeed = 30.0f;
    }

    void handleScroll(sf::Event& event, sf::Vector2f mousePos) {
        // Kiểm tra chuột có trong vùng scroll không
        if (!bounds.contains(mousePos)) return;
        
        if (event.type == sf::Event::MouseWheelScrolled) {
            
            // FIX: Đảo dấu của event.mouseWheelScroll.delta để cuộn đúng chiều
            // (Thử lại với dấu trừ để đảo chiều cuộn)
            scrollOffset -= event.mouseWheelScroll.delta * scrollSpeed; 
            
            // Đảm bảo không cuộn quá giới hạn
            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > maxScroll) scrollOffset = maxScroll;
        }
    }

    void setMaxScroll(float max) { 
        maxScroll = std::max(0.0f, max);
    }
    
    float getScrollOffset() const { return scrollOffset; }
    void reset() { scrollOffset = 0; }
    
    // Áp dụng offset cho position của object
    sf::Vector2f applyScroll(sf::Vector2f pos) const {
        return sf::Vector2f(pos.x, pos.y - scrollOffset);
    }
};

#endif
