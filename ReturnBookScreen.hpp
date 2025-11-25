#ifndef RETURN_BOOK_SCREEN_HPP
#define RETURN_BOOK_SCREEN_HPP
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Modal.hpp"
#include "InputField.hpp"
#include "RoundedRectangle.hpp"
#include "Theme.hpp"

class ReturnBookScreen {
private:
    Modal* modal; RoundedRectangleShape panel, ratingPanel; sf::Text titleText, returnMsg, askRatingText; InputField* maSachField;
    Button *returnButton, *cancelButton, *yesButton, *noButton; bool shaking = false; float shakeTime = 0.0f; bool askRatingVisible = false;
public:
    ReturnBookScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        panel.setSize(sf::Vector2f(700, 450)); panel.setCornerRadius(15.0f); panel.setFillColor(sf::Color::White); panel.setOutlineThickness(2); panel.setOutlineColor(Theme::Primary); panel.setPosition(300, 135);
        titleText.setFont(font); titleText.setString("TRA SACH"); titleText.setCharacterSize(32); titleText.setFillColor(Theme::Primary); titleText.setPosition(580, 170);
        maSachField = new InputField({350, 240}, {600, 55}, "Nhap ma sach can tra...", font);
        returnMsg.setFont(font); returnMsg.setCharacterSize(18); returnMsg.setFillColor(Theme::Error); returnMsg.setPosition(350, 310);
        returnButton = new Button({370, 480}, {200, 55}, "Tra Sach", font, 0, Theme::Secondary); cancelButton = new Button({730, 480}, {200, 55}, "Huy", font, 0, sf::Color(150, 150, 150));
        ratingPanel.setSize({500, 300}); ratingPanel.setCornerRadius(15.0f); ratingPanel.setFillColor(sf::Color::White); ratingPanel.setOutlineThickness(2); ratingPanel.setOutlineColor(Theme::Success); ratingPanel.setPosition(400, 210);
        askRatingText.setFont(font); askRatingText.setString("Tra sach thanh cong!\nBan co muon danh gia khong?"); askRatingText.setCharacterSize(22); askRatingText.setFillColor(Theme::TextDark); askRatingText.setPosition(440, 260);
        yesButton = new Button({450, 410}, {180, 50}, "Co, Luon!", font, 3, Theme::Success); noButton = new Button({670, 410}, {180, 50}, "De sau", font, 4, sf::Color(150, 150, 150));
    }
    ~ReturnBookScreen() { delete returnButton; delete cancelButton; delete maSachField; delete yesButton; delete noButton; }
    std::string getMaSachInput() const { return maSachField->getText(); } void setReturnMessage(const std::string& msg) { returnMsg.setString(msg); } void startShake() { shaking = true; shakeTime = 0; }
    bool validate() { if (getMaSachInput().empty()) { setReturnMessage("Vui long nhap ma sach!"); startShake(); return false; } return true; }
    void showAskRating() { askRatingVisible = true; } void hideAskRating() { askRatingVisible = false; } void clear() { maSachField->clear(); returnMsg.setString(""); askRatingVisible = false; }
    void update(sf::Vector2f mousePos) { if (!askRatingVisible) { maSachField->update(); returnButton->update(mousePos); cancelButton->update(mousePos); if (shaking) { shakeTime += 0.02f; float offset = sin(shakeTime * 50) * 10; panel.setPosition(300 + offset, 135); if (shakeTime >= 0.4f) { shaking = false; panel.setPosition(300, 135); } } } else { yesButton->update(mousePos); noButton->update(mousePos); } }
    void handleEvent(sf::Event& event, sf::Vector2f mousePos) { if (!askRatingVisible) maSachField->handleEvent(event, mousePos); }
    int handleClick(sf::Vector2f mousePos) { if (askRatingVisible) { if (yesButton->handleClick(mousePos)) return 3; if (noButton->handleClick(mousePos)) return 4; return 0; } if (returnButton->handleClick(mousePos)) return 1; if (cancelButton->handleClick(mousePos)) return 2; return 0; }
    void render(sf::RenderWindow& window) { if (!modal || !modal->isShown()) return; if (!askRatingVisible) { window.draw(panel); window.draw(titleText); maSachField->draw(window); window.draw(returnMsg); returnButton->draw(window); cancelButton->draw(window); } if (askRatingVisible) { window.draw(ratingPanel); window.draw(askRatingText); yesButton->draw(window); noButton->draw(window); } }
};
#endif