#include "Reader.h"
#include <iostream>
using namespace std;

int Reader::readerCount = 0;

Reader::Reader() {
    readerCount++;
    maID = "R" + to_string(readerCount);
    HeadDsMuonSach = nullptr; // Khoi tao danh sach muon sach rong
}

Reader::~Reader() {
    // Giai phong danh sach muon sach
    NodeMuonSach *current = HeadDsMuonSach;
    while (current != nullptr) {
        NodeMuonSach *temp = current;
        current = current->next;
        delete temp;
    }
}

void Reader::SignUp(string hoTen, string SDT, string Email, string username, string password) {
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    cout << "Dang ky thanh cong! Ma ID cua ban la: " << maID << endl;
}

void Reader::muonSach(const Sach &book) {
    NodeMuonSach *newNode = new NodeMuonSach;
    newNode->data = book;
    newNode->next = HeadDsMuonSach;
    HeadDsMuonSach = newNode;
    cout << "Muon sach thanh cong: " << book.getTenSach() << endl;
}

void Reader::traSach(const string &maSach) {
    NodeMuonSach *current = HeadDsMuonSach;
    NodeMuonSach *prev = nullptr;
    while (current != nullptr) {
        if (current->data.getMaSach() == maSach) {
            if (prev == nullptr) {
                HeadDsMuonSach = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            cout << "Tra sach thanh cong: " << maSach << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Khong tim thay sach voi ma: " << maSach << endl;
}

void Reader::HienThiSachDaMuon() const {
    NodeMuonSach *current = HeadDsMuonSach;
    if (current == nullptr) {
        cout << "Ban chua muon sach nao." << endl;
        return;
    }
    while (current != nullptr) {
        current->data.hienThiThongTin();
        current = current->next;
    }
}

void Reader::HienThiThongTin() const {
    USER::HienThiThongTin();
    cout << "Sach da muon: " << endl;
    HienThiSachDaMuon();
}


