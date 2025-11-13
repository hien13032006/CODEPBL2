#pragma once
#include "ScreenLoginBase.h"
#include "Librarian.h"
#include "LibrarianLoader.h"
#include "LibrarianLogin.h"
#include "LibrarySystem.h"


class ScreenLoginLibrarian : public ScreenLoginBase {
    NodeLibrarian* head;
    LibrarySystem* system;
    std::function<void()> onOk;
    std::function<void()> onBackFn;
public:
    ScreenLoginLibrarian(NodeLibrarian* h, std::function<void()> okFn, std::function<void()> backFn)
    : head(h), onOk(okFn), onBackFn(backFn) {}

    ScreenLoginLibrarian(sf::Font& f, LibrarySystem* sys) : system(sys) {
        font = f;
        init();
    }

    std::string titleText() const override { return "DANG NHAP THU THU"; }
    void onBack() override { onBackFn(); }

    void handleEvent(sf::Event& evt, AppState& state, sf::RenderWindow* w) override {
        if(w) handleEventBase(evt, state, w);
    }

    void draw(sf::RenderWindow& w) override {
        ScreenLoginBase::draw(w);
    }


    void onLogin() override {
        if(inUser.isEmpty() || inPass.isEmpty()){ 
            popup.show("Thieu thong tin!"); 
            return; 
        }
        if(loginLibrarian(head, inUser, inPass)){ 
            popup.show("Thanh cong!"); 
            onOk(); 
        }
        else { 
            popup.show("Sai thong tin!"); 
        }
    }
};