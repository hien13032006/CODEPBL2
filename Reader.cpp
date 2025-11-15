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

string layNgayDangKy(time_t ngayDK) {
    tm* timeinfo = localtime(&ngayDK);
    char buffer[15];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
    return string(buffer);
}

tm parseDateTime(const string& dateStr) {
    tm t = {};
    sscanf(dateStr.c_str(), "%d/%d/%d %d:%d:%d",
           &t.tm_mday, &t.tm_mon, &t.tm_year,
           &t.tm_hour, &t.tm_min, &t.tm_sec);

    t.tm_mon -= 1;
    t.tm_year -= 1900;

    return t;
}

string cong3Thang(const string& ngayMuon) {
    tm t = parseDateTime(ngayMuon);

    t.tm_mon += 3;
    mktime(&t);

    char buf[30];
    sprintf(buf, "%02d/%02d/%04d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
    return string(buf);
}

int tinhSoNgayConLai(const string& ngayMuon) {
    tm tmMuon = parseDateTime(ngayMuon);

    // hạn trả
    tm tmHan = tmMuon;
    tmHan.tm_mon += 3;
    time_t tHan = mktime(&tmHan);

    time_t now = time(nullptr);

    double diff = difftime(tHan, now);
    int days = diff / (60 * 60 * 24);

    return days;
}

int Reader::readerCount = 1;

Reader::Reader() : USER() {
    vaiTro = UserRole::Reader;
    gioiHanSachMuon = 5; // Mặc định giới hạn 5 cuốn sách
    stringstream ss;
    ss << "R" << setw(4) << setfill('0') << readerCount++;
    maID = ss.str();
    ngayDangKy = time(nullptr);
    HeadDsMuonSach = nullptr;
    fileLichSu = "LichSu_" + maID + ".txt";
}

Reader::Reader(string ma, string hoTen, string sdt, string email, string username, string password) 
    : USER(ma, hoTen, sdt, email, username, password) {
    vaiTro = UserRole::Reader;
    gioiHanSachMuon = 5;
    ngayDangKy = time(nullptr);
    HeadDsMuonSach = nullptr;
    fileLichSu = "LichSu_" + maID + ".txt";
}

Reader::~Reader() {
    // Giai phong danh sach muon sach
    NodeMuonSach *current = HeadDsMuonSach;
    while (current != nullptr) {
        NodeMuonSach *temp = current;
        current = current->next;
        delete temp->data; // Giai phong thong tin sach
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

bool Reader::coTheMuonSach() const {
    return trangThaiHoatDong && (DemSachDaMuon() < gioiHanSachMuon);
}

void Reader::themSachDaMuon(const Sach* s) {
    NodeMuonSach* newNode = new NodeMuonSach;
    newNode->data = s->clone();
    newNode->ngayMuon = layThoiGianHeThong();
    newNode->next = HeadDsMuonSach;
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

void Reader::HienThiSachDangMuon() {
    docFileSachDangMuonTuFile(); // Đảm bảo đã đọc lịch sử để có dữ liệu mới nhất
    NodeMuonSach *current = HeadDsMuonSach;
    if (current == nullptr) {
        cout << "Chua muon sach nao." << endl;
        return;
    }

    cout << left
         << setw(5)  << "STT"
         << setw(15) << "Ma Sach"
         << setw(30) << "Ten Sach"
         << setw(20) << "Ngay Muon"
         << setw(20) << "Han Tra"
         << setw(20) << "Con Lai"
         << endl;

    cout << string(110, '-') << endl;

    int stt = 1;

    while (current != nullptr) {

        string ngayMuon = current->ngayMuon;
        string hanTra = cong3Thang(ngayMuon);
        int daysLeft = tinhSoNgayConLai(ngayMuon);

        string trangThai;
        if (daysLeft < 0)
            trangThai = "QUA HAN";
        else {
            trangThai = to_string(daysLeft) + " ngay";
        }

        cout << setw(5)  << stt++
             << setw(15) << current->data->getMaSach()
             << setw(30) << current->data->getTenSach()
             << setw(20) << ngayMuon
             << setw(20) << hanTra
             << setw(20) << trangThai
             << endl;

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

void Reader::docFileSachDangMuonTuFile() {
    ifstream in(fileLichSu);
    if(!in.is_open()) return;

    HeadDsMuonSach = nullptr; // reset danh sach hien tai
    string line;
    while(getline(in, line)) {
        if(line.empty()) continue;

        stringstream ss(line);
        string hanhDong, maDG, maSach, tenSach, thoiGian;
        getline(ss, hanhDong, '|');
        getline(ss, maDG, '|');
        getline(ss, maSach, '|');
        getline(ss, tenSach, '|');
        getline(ss, thoiGian, '|');

        if(maDG != maID) continue;

        if(hanhDong == "muon") {
            string maSach, tenSach, tacGia, theLoai, nhaXB;
            int namXB;
            Sach* s = Sach::createFromData(tenSach, tacGia, theLoai, namXB, nhaXB);
            if (!s) continue;

            s->setMaSach(maSach);

            NodeMuonSach* newNode = new NodeMuonSach;
            newNode->data = s;
            newNode->ngayMuon = thoiGian;
            newNode->next = HeadDsMuonSach;
            HeadDsMuonSach = newNode;
        }
        else if(hanhDong == "tra") {
            // xóa sách khỏi HeadDsMuonSach
            NodeMuonSach* current = HeadDsMuonSach;
            NodeMuonSach* prev = nullptr;
            while(current != nullptr) {
                if(current->data->getMaSach() == maSach) {
                    if(prev == nullptr) HeadDsMuonSach = current->next;
                    else prev->next = current->next;
                    delete current->data;
                    delete current;
                    break;
                }
                prev = current;
                current = current->next;
            }
        }
    }
}


void Reader::HienThiThongTin() const {
    USER::HienThiThongTin();
    cout << setw(15) << layNgayDangKy(ngayDangKy);
    cout << DemSachDaMuon() << "/" << gioiHanSachMuon << " cuon" << endl;
}

string Reader::toCSV() const {
    ostringstream oss;
    oss << maID << "|" << hoTen << "|" << SDT << "|" << Email << "|" << username << "|" << password << "|"
        << layNgayDangKy(ngayDangKy);
    return oss.str();
}

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


