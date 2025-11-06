#pragma once
#include "ScreenLoginBase.h"
#include "Librarian.h"
#include "LibrarianLoader.h"
#include "LibrarianLogin.h"


class ScreenLoginLibrarian : public ScreenLoginBase {
NodeLibrarian*& head;
std::function<void()> onOk;
std::function<void()> onBackFn;
public:
ScreenLoginLibrarian(NodeLibrarian*& h, std::function<void()> okFn, std::function<void()> backFn)
: head(h), onOk(okFn), onBackFn(backFn) {}


std::string titleText() const override { return "DANG NHAP THU THU"; }
void onBack() override { onBackFn(); }


void onLogin() override {
if(inUser.isEmpty() || inPass.isEmpty()){ popup.show("Thieu thong tin!"); return; }
if(loginLibrarian(head, inUser, inPass)){ popup.show("Thanh cong!"); onOk(); }
else { popup.show("Sai thong tin!"); }
}
};