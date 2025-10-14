#ifndef Book_H
#define Book_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Sach {
private:
    string maSach, tenSach, tacGia, nhaXuatBan, theLoai, ngonNgu;
    int namXuatBan, soLuong;
    bool tinhTrang;

public:
    Sach();
    void nhapThongTin();
    void hienThiThongTin() const;
    void capNhatTinhTrang();
    void muonSach();
    void traSach();
    void ghiFile(ofstream &out) const;
    bool docFile(ifstream &in);
};

#endif
