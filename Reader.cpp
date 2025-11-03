#include "Reader.h"
#include "Book.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Node.h"
using namespace std;

string layThoiGianHeThong() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

int Reader::readerCount = 1;

Reader::Reader() {
    stringstream ss;
    ss << "R" << setw(4) << setfill('0') << readerCount++;
    maID = ss.str();
    fileLichSu = "LichSu_" + maID + ".txt";
}

Reader::Reader(string ma, string hoTen, string sdt, string email, string username, string password) {
    maID = ma;
    setHoTen(hoTen);
    setSDT(sdt);
    setEmail(email);
    setUsername(username);
    setPassword(password);
    fileLichSu = "LichSu_" + maID + ".txt";
}

Reader::~Reader() {
    // Giai phong danh sach muon sach
    NodeMuonSach *current = HeadDsMuonSach;
    while (current != nullptr) {
        NodeMuonSach *temp = current;
        current = current->next;
        delete temp;
    }
}

void Reader::SignUp(string hoTen, string SDT, string Email, string username, string password) {
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    cout << "Dang ky thanh cong! Ma ID cua ban la: " << maID << endl;
}

//Kiem tra sach da muon
bool Reader::DaMuonSach(const string maSach) const {
    NodeMuonSach* current = HeadDsMuonSach;
        while (current != nullptr) {
            if (current->data->getMaSach() == maSach)
                return true;
            current = current->next;
        }
        return false;
}

//Dem so sach da muon
int Reader::DemSachDaMuon() const {
    int count = 0;
    NodeMuonSach* current = HeadDsMuonSach;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void Reader::themSachDaMuon(const Sach* s) {
    NodeMuonSach* newNode = new NodeMuonSach{s->clone(), HeadDsMuonSach};
    HeadDsMuonSach = newNode;
}

void Reader::xoaSachDaMuon(const string& maSach) {
    NodeMuonSach* current = HeadDsMuonSach;
    NodeMuonSach* prev = nullptr;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            if (prev == nullptr)
                HeadDsMuonSach = current->next;
            else
                prev->next = current->next;
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void Reader::HienThiSachDaMuon() const {
    NodeMuonSach *current = HeadDsMuonSach;
    if (current == nullptr) {
        cout << "Chua muon sach nao." << endl;
        return;
    }
    while (current != nullptr) {
        current->data->hienThiThongTin();
        current = current->next;
    }
}

void Reader::ghiLichSu(const string& hanhDong, const Sach* s) { 
    ofstream out(fileLichSu, ios::app); // Ghi vào file riêng
    if (out.is_open()) {
        out << hanhDong << "|" << maID << "|" << s->getMaSach() 
            << "|" << s->getTenSach() 
            << "|" << layThoiGianHeThong() << "\n";
        out.close();
    } else {
        cout << "Khong the mo file " << fileLichSu << " de ghi lich su muon/tra.\n";
    }
}

void Reader::HienThiThongTin() const {
    USER::HienThiThongTin();
    cout << "Sach da muon: " << endl;
    HienThiSachDaMuon();
}

string Reader::toCSV() const {
    ostringstream oss;
    oss << maID << "|" << hoTen << "|" << SDT << "|" << Email << "|" << username << "|" << password;
    return oss.str();
}

#include <iomanip> // để dùng setw, left, right

void Reader::HienThiLichSuMuonTra() const {
    ifstream in(fileLichSu);
    if (!in.is_open()) {
        cout << " Khong tim thay file lich su cho doc gia.\n";
        return;
    }

    string line;
    bool hasData = false;

    cout << " =====LICH SU MUON - TRA SACH =====\n";

    cout << left
         << setw(5)  << "STT"
         << setw(15) << "Hanh dong"
         << setw(12) << "Ma sach"
         << setw(35) << "Ten sach"
         << setw(25) << "Thoi gian"
         << endl;
    cout << string(90, '-') << endl;

    int stt = 1;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string hanhDong, maDG, maSach, tenSach, thoiGian;
        getline(ss, hanhDong, '|');
        getline(ss, maDG, '|');
        getline(ss, maSach, '|');
        getline(ss, tenSach, '|');
        getline(ss, thoiGian, '|');

        if (maDG == maID) {
            cout << left
                 << setw(5)  << stt++
                 << setw(15) << hanhDong
                 << setw(12) << maSach
                 << setw(35) << tenSach
                 << setw(25) << thoiGian
                 << endl;
            hasData = true;
        }
    }

    if (!hasData)
        cout << "Khong co lich su muon/tra nao duoc ghi lai.\n";

    cout << string(90, '-') << endl;

    in.close();
}


