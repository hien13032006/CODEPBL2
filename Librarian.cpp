#include <iostream>
#include "Librarian.h"
#include "LibrarySystem.h"
#include "Book.h"
using namespace std;

void Librarian::SetThongTin(string maID,string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
}


void Librarian::XoaSach(LibrarySystem &L) {
    string maSach;
    cout << "Nhap ma sach can xoa: ";
    getline(cin,maSach);
    L.XoaSach(maSach);
}