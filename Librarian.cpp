#include "Librarian.h"
#include "Book.h"
#include <iostream>
using namespace std;

int Librarian::LibrarianCount = 0;

Librarian::Librarian() {
    LibrarianCount++;
    maID = "L" + to_string(LibrarianCount);
}

Librarian::void SingUp(string hoTen, string SDT, string Email, string username, string password) {
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    cout << "Dang ky thanh cong! Ma ID cua ban la: " << maID << endl;
}

Librarian::void HienThiThongTin() const {
    cout << "=== Thong tin thu thu ===" << endl;
    USER::HienThiThongTin();
    cout << "Quyen han: " << quyenHan << endl;
}




