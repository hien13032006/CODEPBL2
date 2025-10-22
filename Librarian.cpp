#include "Librarian.h"
#include "Book.h"
#include <iostream>
using namespace std;

int Librarian::LibrarianCount = 0;

Librarian::Librarian() {
    LibrarianCount++;
    maID = "L" + to_string(LibrarianCount);
}

void Librarian::SignUp(string hoTen, string SDT, string Email, string username, string password) {
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    cout << "Dang ky thanh cong! Ma ID cua ban la: " << maID << endl;
}

void Librarian::HienThiThongTin() const {
    cout << "\n--- THONG TIN THU THU ---\n";
    USER::HienThiThongTin();
}

void Librarian::ThemSach(LibrarySystem &L) {
    Sach sachMoi;
    sachMoi.nhapThongTin();
    L.ThemSach(sachMoi);
}

void Librarian::XoaSach(LibrarySystem &L) {
    string maSach;
    cout << "Nhap ma sach can xoa: ";
    getline(cin,maSach);
    L.XoaSach();
}






