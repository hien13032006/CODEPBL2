#include "USER.h"
#include <iomanip> 
#include <iostream>
using namespace std;


USER::USER() {
    maID = "";
    hoTen = "";
    SDT = "";
    Email = "";
    username = "";
    password = "";
    trangThaiHoatDong = true;
    vaiTro = UserRole::Reader;
}

USER::USER(string maID, string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    this->trangThaiHoatDong = true;
    this->vaiTro = UserRole::Reader;
}

USER::~USER() {}

void USER::HienThiThongTin() const {

    cout << left << setw(10) << maID 
                 << setw(25) << hoTen  
                 <<setw(20)  << SDT      
                 << setw(25) << Email    
                 << setw(15) << username;
}

void USER::CapNhatThongTin() {
    cout << "Cap nhat thong tin ca nhan: " << endl;
    cout << "Nhap ho ten moi: ";
    getline(cin, hoTen);
    cout << "Nhap SDT moi: ";
    getline(cin, SDT);
    cout << "Nhap email moi: ";
    getline(cin, Email);
    cout << "Thong tin da duoc cap nhat." << endl;
}

bool USER::Login(string username, string password) {
    return (this->username == username && this->password == password);
}

void USER::Logout() {
    cout << username << " da dang xuat khoi he thong." << endl;
}

