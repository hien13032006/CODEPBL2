#pragma once
#include "ScreenLoginBase.h"
#include "Reader.h"
#include "ReaderLoader.h"
#include "ReaderLogin.h"


class ScreenLoginReader : public ScreenLoginBase {
NodeReader*& head;
std::function<void()> onOk;
std::function<void()> onBackFn;
public:
ScreenLoginReader(NodeReader*& h, std::function<void()> okFn, std::function<void()> backFn)
: head(h), onOk(okFn), onBackFn(backFn) {}


std::string titleText() const override { return "DANG NHAP DOC GIA"; }
void onBack() override { onBackFn(); }


void onLogin() override {
if(inUser.isEmpty() || inPass.isEmpty()){ popup.show("Thieu thong tin!"); return; }
if(loginReader(head, inUser, inPass)){ popup.show("Thanh cong!"); onOk(); }
else { popup.show("Sai ten hoac mat khau!"); }
}
};