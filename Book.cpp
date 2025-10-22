#include "Book.h"
#include <iomanip>
 
Sach::Sach() {
    maSach = "";
    tenSach = "";
    tacGia = "";
    nhaXuatBan = "";
    namXuatBan = 0;
    soLuong = 0;
}

Sach::~Sach() {}

void Sach::nhapThongTin() {
    cout << "Nhap ten sach: ";
    getline(cin, tenSach);
    cout << "Nhap tac gia: ";
    getline(cin, tacGia);
    cout << "Nhap nha xuat ban: ";
    getline(cin, nhaXuatBan);
    cout << "Nhap nam xuat ban: ";
    cin >> namXuatBan;
    cout << "Nhap so luong: ";
    cin >> soLuong;
    cin.ignore();
}

void Sach::hienThiThongTin() const {
    cout << "Ma: " << maSach << "| Ten: " << tenSach << "| Tac gia: " << tacGia
         << "| NXB: " << nhaXuatBan << "| Nam xuat ban: " << namXuatBan
         << "| So luong: " << soLuong << endl;
}

const string& Sach::getMaSach() const {
    return maSach;
}
const string& Sach::getTenSach() const {
    return tenSach;
}

const string& Sach::getTacGia() const {
    return tacGia;
}
const string& Sach::getNhaXuatBan() const {
    return nhaXuatBan;
}
int Sach::getNamXuatBan() const {
    return namXuatBan;
}
int Sach::getSoLuong() const {
    return soLuong;
}

void Sach::setMaSach(const string& m) {
    maSach = m;
}
void Sach::setTenSach(const string& t) {
    tenSach = t;
}
void Sach::setTacGia(const string& tg) {
    tacGia = tg;
}
void Sach::setNhaXuatBan(const string& nxb) {
    nhaXuatBan = nxb;
}
void Sach::setNamXuatBan(int n) {
    namXuatBan = n;
}
void Sach::setSoLuong(int s) {
    soLuong = s;
}

void Sach::setautoMaSach(int stt) {
    // stt: số thứ tự (1-based). Format 4 chữ số, zero-padded.
    ostringstream oss;
    oss << prefix();
    char buf[8];
    std::sprintf(buf, "%04d", stt);
    oss << buf;
    maSach = oss.str();
}

string Sach::tocsv() const {
    ostringstream oss;
   
    oss << maSach << "," << tenSach << "," << tacGia << "," << nhaXuatBan << "," << namXuatBan << "," << soLuong;
    return oss.str();
}

GiaoTrinh::GiaoTrinh() : Sach() {}
std::string GiaoTrinh::prefix() const { return "GTR"; }

ThamKhao::ThamKhao() : Sach() {}
std::string ThamKhao::prefix() const { return "STK"; }

TieuThuyet::TieuThuyet() : Sach() {}
std::string TieuThuyet::prefix() const { return "TIE"; }

TruyenNgan::TruyenNgan() : Sach() {}
std::string TruyenNgan::prefix() const { return "TRN"; }

TapChi::TapChi() : Sach() {}
std::string TapChi::prefix() const { return "TAP"; }

TruyenTranh::TruyenTranh() : Sach() {}
std::string TruyenTranh::prefix() const { return "TRC"; }

SachKiNang::SachKiNang() : Sach() {}
std::string SachKiNang::prefix() const { return "SKN"; }
