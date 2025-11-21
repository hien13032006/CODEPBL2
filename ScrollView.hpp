#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>

enum class ScrollDirection { VERTICAL, HORIZONTAL };

class ScrollView {
private:
    float scrollOffset;
    float maxScroll;
    sf::FloatRect bounds; 
    float scrollSpeed;
    ScrollDirection direction;

public:
    ScrollView(sf::FloatRect viewBounds, ScrollDirection dir = ScrollDirection::VERTICAL) 
        : bounds(viewBounds), direction(dir) {
        scrollOffset = 0;
        maxScroll = 0;
        scrollSpeed = 40.0f; // Tốc độ cuộn
    }

    void handleScroll(sf::Event& event, sf::Vector2f mousePos) {
        // Chỉ cuộn khi chuột nằm trong vùng cho phép
        if (!bounds.contains(mousePos)) return;
        
        if (event.type == sf::Event::MouseWheelScrolled) {
            scrollOffset -= event.mouseWheelScroll.delta * scrollSpeed;
            // Giới hạn cuộn không âm và không quá max
            scrollOffset = std::max(0.0f, std::min(maxScroll, scrollOffset));
        }
    }

    void setMaxScroll(float contentSize) {
        float viewportSize = (direction == ScrollDirection::VERTICAL) ? bounds.height : bounds.width;
        // Thêm padding 50px để cuộn thoải mái hơn
        maxScroll = std::max(0.0f, contentSize - viewportSize + 50.0f); 
    }
    
    float getScrollOffset() const { return scrollOffset; }
    void reset() { scrollOffset = 0; }
};

#endif