#include "Book.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Node.h"

string Sach::toCSV() const {
    ostringstream oss;
    oss << maSach << "|" << tenSach << "|" << tacGia << "|"
        << theLoai << "|" << namXuatBan << "|" << nhaXuatBan << "|"
        << tinhTrang;
    return oss.str();
}

void Sach::hienThiThongTin() const {
    cout <<"-----------------------------------------\n";
    cout <<"Ma sach: " << maSach << endl;
    cout <<"Ten sach: " << tenSach << endl;
    cout <<"Tac gia: " << tacGia << endl;
    cout <<"The loai: " << theLoai << endl;
    cout <<"Nam xuat ban: " << namXuatBan << endl;
    cout << "Nha xuat ban: " << nhaXuatBan << endl;
    cout << "Tinh trang: " << tinhTrang << endl;
}

// 🏭 Factory: Tạo đúng lớp con dựa trên thể loại
Sach* Sach::createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb) {
    Sach* s = nullptr;
    if (tl == "Giao trinh") s = new GiaoTrinh(ten, tg, tl, nam, nxb);
    else if (tl == "Tham khao") s = new ThamKhao(ten, tg, tl, nam, nxb);
    else if (tl == "Tieu thuyet") s = new TieuThuyet(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen ngan") s = new TruyenNgan(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen tranh") s = new TruyenTranh(ten, tg, tl, nam, nxb);
    else if (tl == "Sach ki nang") s = new SachKiNang(ten, tg, tl, nam, nxb);
    else s = new GiaoTrinh(ten, tg, tl, nam, nxb); // mặc định

    // Tự tạo mã ID (dựa prefix)
    static int stt = 1;
    string id = s->prefix() + (stt < 10 ? "0" + to_string(stt) : to_string(stt));
    s->setMaSach(id);
    stt++;
    return s;
}

void Sach::docFileInput(const string& fileName, NodeBook*& head) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cerr << " Khong mo duoc file: " << fileName << endl;
        return;
    }

    string line;
    NodeBook* tail = nullptr;
    while (getline(in, line)) {
        if (line.empty()) continue;
        string ten, tg, tl, nxb, namStr;
        stringstream ss(line);
        getline(ss, ten, '|');
        getline(ss, tg, '|');
        getline(ss, tl, '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');

        int nam = stoi(namStr);
        Sach* s = Sach::createFromData(ten, tg, tl, nam, nxb);

        NodeBook* newNode = new NodeBook(s);
        newNode->next = head;
        head = newNode;
    }
    in.close();
    cout << "Doc file thanh cong: " << fileName << endl;
}

void Sach::ghiFile(const string& fileName, NodeBook* head) {
    ofstream out(fileName);
    if (!out.is_open()) {
        cerr << " Khong mo duoc file de ghi: " << fileName << endl;
        return;
    }

    NodeBook* p = head;
    while (p) {
        out << p->data->toCSV() << endl;
        p = p->next;
    }

    out.close();
    cout << " Da ghi danh sach sach vao: " << fileName << endl;
}


