#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <SFML/Graphics.hpp>

class ScrollView {
private:
    sf::View view;
    float scrollOffset;
    float maxScroll;
    sf::FloatRect bounds;

public:
    ScrollView(sf::FloatRect viewBounds) : bounds(viewBounds) {
        view.reset(viewBounds);
        view.setViewport(sf::FloatRect(
            viewBounds.left / 1400.0f,
            viewBounds.top / 900.0f,
            viewBounds.width / 1400.0f,
            viewBounds.height / 900.0f
        ));
        scrollOffset = 0;
        maxScroll = 0;
    }

    void handleScroll(sf::Event& event) {
        if (event.type == sf::Event::MouseWheelScrolled) {
            scrollOffset -= event.mouseWheelScroll.delta * 30;
            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > maxScroll) scrollOffset = maxScroll;
            view.setCenter(bounds.left + bounds.width / 2, 
                          bounds.top + bounds.height / 2 + scrollOffset);
        }
    }

    void setMaxScroll(float max) { maxScroll = max; }
    sf::View& getView() { return view; }
    void reset() { scrollOffset = 0; }
};

#endif