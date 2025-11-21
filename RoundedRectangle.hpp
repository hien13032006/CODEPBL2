
#ifndef ROUNDED_RECTANGLE_HPP
#define ROUNDED_RECTANGLE_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape {
private:
    sf::Vector2f size;
    float radius;
    unsigned int cornerPointCount;

public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0), 
                                   float radius = 10, 
                                   unsigned int cornerPointCount = 10)
        : size(size), radius(radius), cornerPointCount(cornerPointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) {
        this->size = size;
        update();
    }

    const sf::Vector2f& getSize() const {
        return size;
    }

    void setCornerRadius(float radius) {
        this->radius = radius;
        update();
    }

    float getCornerRadius() const {
        return radius;
    }

    virtual std::size_t getPointCount() const override {
        return cornerPointCount * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= cornerPointCount * 4)
            return sf::Vector2f(0, 0);

        float deltaAngle = 90.0f / (cornerPointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / cornerPointCount;
        unsigned int pointIndex = index % cornerPointCount;

        switch (centerIndex) {
            case 0: // Top-left
                center.x = radius;
                center.y = radius;
                return center + sf::Vector2f(
                    radius * std::cos((180 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((180 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 1: // Top-right
                center.x = size.x - radius;
                center.y = radius;
                return center + sf::Vector2f(
                    radius * std::cos((270 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((270 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 2: // Bottom-right
                center.x = size.x - radius;
                center.y = size.y - radius;
                return center + sf::Vector2f(
                    radius * std::cos((0 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((0 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 3: // Bottom-left
                center.x = radius;
                center.y = size.y - radius;
                return center + sf::Vector2f(
                    radius * std::cos((90 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((90 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
        }

        return sf::Vector2f(0, 0);
    }
};

#endif