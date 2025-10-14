#include "Book.h"

Sach::Sach() : namXuatBan(0), soLuong(0), tinhTrang(true) {}

void Sach::nhapThongTin() {
    cout << "Nhap ma sach: ";
    getline(cin, maSach);
    cout << "Nhap ten sach: ";
    getline(cin, tenSach);
    cout << "Nhap tac gia: ";
    getline(cin, tacGia);
    cout << "Nhap nha xuat ban: ";
    getline(cin, nhaXuatBan);
    cout << "Nhap the loai: ";
    getline(cin, theLoai);
    cout << "Nhap ngon ngu: ";
    getline(cin, ngonNgu);
    cout << "Nhap nam xuat ban: ";
    cin >> namXuatBan;
    cout << "Nhap so luong: ";
    cin >> soLuong;
    cin.ignore();
    capNhatTinhTrang();
}

void Sach::hienThiThongTin() const {
    cout << "\n--- THONG TIN SACH ---\n";
    cout << "Ma sach: " << maSach << endl;
    cout << "Ten sach: " << tenSach << endl;
    cout << "Tac gia: " << tacGia << endl;
    cout << "Nha xuat ban: " << nhaXuatBan << endl;
    cout << "The loai: " << theLoai << endl;
    cout << "Ngon ngu: " << ngonNgu << endl;
    cout << "Nam xuat ban: " << namXuatBan << endl;
    cout << "So luong: " << soLuong << endl;
    cout << "Tinh trang: " << (tinhTrang ? "Con sach" : "Het sach") << endl;
}

void Sach::capNhatTinhTrang() { tinhTrang = (soLuong > 0); }

void Sach::muonSach() {
    if (soLuong > 0) soLuong--;
    capNhatTinhTrang();
}

void Sach::traSach() {
    soLuong++;
    capNhatTinhTrang();
}

void Sach::ghiFile(ofstream &out) const {
    out << maSach << "," << tenSach << "," << tacGia << ","
        << nhaXuatBan << "," << theLoai << "," << ngonNgu << ","
        << namXuatBan << "," << soLuong << "," << tinhTrang << "\n";
}

bool Sach::docFile(ifstream &in) {
    string line;
    if (!getline(in, line)) return false;
    size_t pos = 0;
    string data[9];
    int i = 0;
    while ((pos = line.find(',')) != string::npos && i < 8) {
        data[i++] = line.substr(0, pos);
        line.erase(0, pos + 1);
    }
    data[i] = line;
    if (i < 8) return false;

    maSach = data[0]; tenSach = data[1]; tacGia = data[2];
    nhaXuatBan = data[3]; theLoai = data[4]; ngonNgu = data[5];
    namXuatBan = stoi(data[6]); soLuong = stoi(data[7]);
    tinhTrang = (data[8] == "1" || data[8] == "true");
    return true;
}
