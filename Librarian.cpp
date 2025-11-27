#include "Librarian.h"
#include "LibrarySystem.h"
#include "Book.h"
#include <iostream>
#include <iomanip>
using namespace std;

Librarian::Librarian() : USER() {
    vaiTro = UserRole::LIBRARIAN;
    chucVu = "Thu Thu";
}

Librarian::Librarian(string maID, string hoTen, string SDT, string Email, string username, string password, string chucVu)
    : USER(maID, hoTen, SDT, Email, username, password) {
    this->vaiTro = UserRole::LIBRARIAN;
    this->chucVu = chucVu;
}

void Librarian::SetThongTin(string maID,string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    this->vaiTro = UserRole::LIBRARIAN;
}

void Librarian::HienThiThongTin() const {
    USER::HienThiThongTin();
    cout << setw(15) << chucVu << endl;
}
void Librarian::ThemSach(LibrarySystem &L) {
    L.DocFileSach("ThemSach.txt");
    L.GhiFileSach("DanhSachSach.txt");
}
string Librarian::toCSV(ostream &out) const {
    std::string csv =  maID  + "," + hoTen +  "," + SDT + "," + Email + "," + username + "," + password + "," + chucVu + "\n";
   out << csv;
   return csv;
}
void Librarian::XoaSach(LibrarySystem &L) {
    string maSach;
    cout << "Nhap ma sach can xoa: ";
    getline(cin,maSach);
    L.XoaSach(maSach);
}

void Librarian::CapNhatThongTinSach(LibrarySystem &L) {
    L.CapNhatThongTinSach();
}
