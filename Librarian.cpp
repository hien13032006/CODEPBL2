#include "Librarian.h"
#include "LibrarySystem.h"
#include "Book.h"
#include <iostream>
using namespace std;

void Librarian::SetThongTin(string maID,string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
}

void Librarian::HienThiThongTin() const {
    cout << "\n--- THONG TIN THU THU ---\n";
    USER::HienThiThongTin();
}

void Librarian::ThemSach(LibrarySystem &L) {

}

void Librarian::XoaSach(LibrarySystem &L) {
    string maSach;
    cout << "Nhap ma sach can xoa: ";
    getline(cin,maSach);
    L.XoaSach(maSach);
}






