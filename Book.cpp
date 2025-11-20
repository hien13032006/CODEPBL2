#include "Book.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Node.h"

string Sach::toCSV() const {
    ostringstream oss;
    oss << maSach << "|" << tenSach << "|" << tacGia << "|"
        << theLoai << "|" << namXuatBan << "|" << nhaXuatBan << "|"
        << soLuong;
    return oss.str();
}

void Sach::hienThiThongTin() const {
    cout << left << setw(15) << maSach
         <<  setw(25) << tenSach
         <<  setw(20) << tacGia
         <<  setw(15) << theLoai
         <<  setw(8)  << namXuatBan
         <<  setw(25) << nhaXuatBan
         <<  setw(13) << soLuong
         << fixed << setprecision(1) << getDiemTrungBinh()
         << "\n";
    
}

//Tạo đúng lớp con dựa trên thể loại
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
    ostringstream oss;
    oss << setw(5) << setfill('0') << stt;
    string id = s->prefix() + oss.str();
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
    while (getline(in, line)) {
        if (line.empty()) continue;

        string ten, tg, tl, nxb, namStr, soLuongStr, imagePath;
        stringstream ss(line);
        getline(ss, ten, '|');
        getline(ss, tg, '|');
        getline(ss, tl, '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');
        getline(ss, soLuongStr, '|');
        getline(ss, imagePath, '|');

        int nam = stoi(namStr);
        int sl = stoi(soLuongStr);

        // Kiểm tra xem sách đã tồn tại chưa
        NodeBook* p = head;
        bool found = false;
        while (p) {
            Sach* s = p->data;
            if (s->getTenSach() == ten &&
                s->getTacGia() == tg &&
                s->getNamXuatBan() == nam &&
                s->getNhaXuatBan() == nxb)
            {
                // Trùng sách -> chỉ tăng số lượng
                s->setSoLuong(s->getSoLuong() + sl);
                found = true;
                break;
            }
            p = p->next;
        }

        // Nếu chưa tồn tại -> tạo node mới
        if (!found) {
            Sach* s = Sach::createFromData(ten, tg, tl, nam, nxb);
            s->setSoLuong(sl);

            NodeBook* newNode = new NodeBook(s);
            if (!head) {
                head = newNode;
            } else {
                NodeBook* tail = head;
                while (tail->next) tail = tail->next;
                tail->next = newNode;
            }
        }
    }

    in.close();
    cout << "Doc file va cap nhat danh sach sach thanh cong: " << fileName << endl;
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



