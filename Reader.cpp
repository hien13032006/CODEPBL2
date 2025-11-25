#include "Reader.h"
#include "Book.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Node.h"
#include<cstdio>
#include<ctime>
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

int Reader::readerCount = 1;

Reader::Reader() : USER() {
    vaiTro = UserRole::READER;
    gioiHanSachMuon = 5; // Mặc định giới hạn 5 cuốn sách
    stringstream ss;
    ss << "R" << setw(4) << setfill('0') << readerCount++;
    maID = ss.str();
    ngayDangKy = time(nullptr);
    HeadDsMuonSach = nullptr;
    fileLichSu = "history/LichSu_" + maID + ".txt";
}

Reader::Reader(string ma, string hoTen, string sdt, string email, string username, string password) 
    : USER(ma, hoTen, sdt, email, username, password) {
    vaiTro = UserRole::READER;
    gioiHanSachMuon = 5;
    ngayDangKy = time(nullptr);
    HeadDsMuonSach = nullptr;
    fileLichSu = "history/LichSu_" + maID + ".txt";
}

Reader::~Reader() {
    // Giai phong danh sach muon sach
    NodeMuonSach *current = HeadDsMuonSach;
    while (current != nullptr) {
        NodeMuonSach *temp = current;
        current = current->next;
        delete temp->phieu; // Giai phong thong tin sach
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
            if (current->phieu->sach->getMaSach() == maSach)
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
    return DemSachDaMuon() < gioiHanSachMuon;
}

int Reader::getSoSachConCoTheMuon() const {
    return gioiHanSachMuon - DemSachDaMuon();
}

void Reader::themPhieuMuonSach(const Sach* s) {
    PhieuMuonSach* phieu = new PhieuMuonSach(s->clone());
    NodeMuonSach* newNode = new NodeMuonSach(phieu);
    newNode->next = HeadDsMuonSach;
    HeadDsMuonSach = newNode;
    
}

void Reader::themPhieuMuonSachTuFile(const Sach* s, time_t ngayMuon, time_t ngayHetHan) {
    PhieuMuonSach* phieu = new PhieuMuonSach(s->clone(), ngayMuon, ngayHetHan);
    NodeMuonSach* newNode = new NodeMuonSach(phieu);
    
    // Thêm vào cuối danh sách để giữ đúng thứ tự
    if (HeadDsMuonSach == nullptr) {
        HeadDsMuonSach = newNode;
    } else {
        NodeMuonSach* temp = HeadDsMuonSach;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void Reader::xoaPhieuMuonSach(const string& maSach) {
    NodeMuonSach* current = HeadDsMuonSach;
    NodeMuonSach* prev = nullptr;
    
    while (current != nullptr) {
        if (current->phieu->sach->getMaSach() == maSach) {
            if (prev == nullptr)
                HeadDsMuonSach = current->next;
            else
                prev->next = current->next;
            
            delete current->phieu->sach;
            delete current->phieu;
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

int Reader::DemSachQuaHan() const {
    int count = 0;
    NodeMuonSach* current = HeadDsMuonSach;
    while (current != nullptr) {
        if (current->phieu->daQuaHan()) {
            count++;
        }
        current = current->next;
    }
    return count;
}

void Reader::HienThiSachDangMuon() const {
    NodeMuonSach* current = HeadDsMuonSach;
    if (current == nullptr) {
        cout << "  Chua muon sach nao." << endl;
        return;
    }
    
    cout << "\n===== SACH DANG MUON =====\n";
    cout << left
         << setw(5)  << "STT"
         << setw(12) << "Ma sach"
         << setw(30) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(15) << "Ngay muon"
         << setw(15) << "Ngay tra"
         << setw(15) << "Con lai"
         << endl;
    cout << string(112, '-') << endl;
    
    int stt = 1;
    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        // Màu sắc cho trạng thái
        string trangThai = phieu->trangThaiHan();
        
        cout << left
             << setw(5)  << stt++
             << setw(12) << phieu->sach->getMaSach()
             << setw(30) << phieu->sach->getTenSach()
             << setw(20) << phieu->sach->getTacGia()
             << setw(15) << timeString(phieu->ngayMuon)
             << setw(15) << timeString(phieu->ngayHetHan);
        
        // Hiển thị trạng thái (quá hạn sẽ nổi bật)
        if (phieu->daQuaHan()) {
            cout << "\033[1;31m" << setw(15) << "*** " + trangThai + " ***" << "\033[0m";  // Đỏ
        } else if (phieu->soNgayConLai() <= 7) {
            cout << "\033[1;33m" << setw(15) << trangThai << "\033[0m";  // Vàng (sắp hết hạn)
        } else {
            cout << setw(15) << trangThai;  // Bình thường
        }
        
        cout << endl;
        current = current->next;
    }
    
    cout << string(112, '-') << endl;
    
    // Thống kê
    int soQuaHan = DemSachQuaHan();
    if (soQuaHan > 0) {
        cout << "\033[1;31m Canh bao: Ban co " << soQuaHan << " sach qua han!\033[0m\n";
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
    cout << setw(15) << layNgayDangKy(ngayDangKy);
    cout << DemSachDaMuon() << "/" << gioiHanSachMuon << " cuon";
    int soQuaHan = DemSachQuaHan();
    if (soQuaHan > 0) {
        cout << setw(15) << "\033[1;31m" << soQuaHan << " cuon\033[0m";
    }
    cout << "\n";
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