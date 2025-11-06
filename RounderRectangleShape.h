#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstddef>

class RoundedRectangleShape : public sf::Shape {
public:
    explicit RoundedRectangleShape(
        const sf::Vector2f& size = {0.f, 0.f},
        float radius = 0.f,
        std::size_t cornerPointCount = 6
    )
    : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) {
        m_size = size;
        update();
    }

    const sf::Vector2f& getSize() const {
        return m_size;
    }

    void setRadius(float radius) {
        m_radius = radius;
        update();
    }

    float getRadius() const {
        return m_radius;
    }

    void setCornerPointCount(std::size_t count) {
        m_cornerPointCount = count;
        update();
    }

    std::size_t getCornerPointCount() const {
        return m_cornerPointCount;
    }

    virtual std::size_t getPointCount() const override {
        return (m_radius > 0.f) ? (m_cornerPointCount * 4) : 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (m_radius <= 0.f) {
            switch (index) {
                default:
                case 0: return {0.f, 0.f};
                case 1: return {m_size.x, 0.f};
                case 2: return {m_size.x, m_size.y};
                case 3: return {0.f, m_size.y};
            }
        }

        float r = std::min(m_radius, std::min(m_size.x, m_size.y) * 0.5f);

        std::size_t cp     = m_cornerPointCount;
        std::size_t corner = index / cp;
        std::size_t i      = index % cp;

        float angleStep = (float)M_PI_2 / (float)(cp - 1);

        sf::Vector2f center;
        float startAngle = 0.f;

        switch (corner) {
            case 0:
                center = {r, r};
                startAngle = (float)M_PI;
                break;
            case 1:
                center = {m_size.x - r, r};
                startAngle = (float)M_PI_2 * 3.f;
                break;
            case 2:
                center = {m_size.x - r, m_size.y - r};
                startAngle = 0.f;
                break;
            default:
                center = {r, m_size.y - r};
                startAngle = (float)M_PI_2;
                break;
        }

        float angle = startAngle + angleStep * (float)i;
        return {
            center.x + std::cos(angle) * r,
            center.y + std::sin(angle) * r
        };
    }

    sf::FloatRect getLocalBounds() const {
        return sf::FloatRect(0.f, 0.f, m_size.x, m_size.y);
    }

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }

private:
    sf::Vector2f  m_size;
    float         m_radius;
    std::size_t   m_cornerPointCount;
};
