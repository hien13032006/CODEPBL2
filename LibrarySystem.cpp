#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring> 

// Lưu ý: Book.h đã được include trong LibrarySystem.h
using namespace std;

// --- CÁC HÀM HỖ TRỢ ---

int LibrarySystem::hashFunction(const string& s) {
    int hash = 0;
    for (char c : s) hash = (hash * 31 + c) % TABLE_SIZE;
    return hash;
}

void LibrarySystem::BuildHashTable() {
    // Khởi tạo bảng băm
    for (int i = 0; i < TABLE_SIZE; i++)
        hashTable[i] = nullptr;

    NodeBook* cur = HeadDsSach;

    while (cur != nullptr) {
        Sach* s = cur->data;

        // Ghép tất cả thông tin thành 1 chuỗi để tìm kiếm
        string info = s->getMaSach() + " " +
                      s->getTenSach() + " " +
                      s->getTacGia() + " " +
                      s->getTheLoai() + " " +
                      s->getNhaXuatBan() + " " +
                      to_string(s->getNamXuatBan());

        // Chuyển về lowercase
        for (char &c : info) c = tolower(c);

        stringstream ss(info);
        string word;

        while (ss >> word) {
            int index = hashFunction(word);
            HashNode* node = hashTable[index];
            bool existed = false;

            // Kiểm tra trùng lặp
            while (node != nullptr) {
                if (node->key == word) {
                    NodeBook* nb = new NodeBook(s);
                    nb->next = node->list;
                    node->list = nb;
                    existed = true;
                    break;
                }
                node = node->next;
            }

            if (existed) continue;

            HashNode* newNode = new HashNode();
            newNode->key = word;
            newNode->list = new NodeBook(s);
            newNode->list->next = nullptr;
            newNode->next = hashTable[index];
            hashTable[index] = newNode;
        }
        cur = cur->next;
    }
}

string timeToString(time_t t) {
    tm* ltm = localtime(&t);
    if (!ltm) return "N/A";
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(buffer);
}

// --- CONSTRUCTOR & DESTRUCTOR ---

LibrarySystem::LibrarySystem() {
    HeadDsSach = nullptr;
    HeadDsDocGia = nullptr;
    top10Count = 0;
}

LibrarySystem::~LibrarySystem() {
    // Giải phóng sách
    NodeBook *currentBook = HeadDsSach;
    while (currentBook != nullptr) {
        NodeBook *temp = currentBook;
        currentBook = currentBook->next;
        delete temp; 
    }

    // Giải phóng độc giả
    NodeReader *currentReader = HeadDsDocGia;
    while (currentReader != nullptr) {
        NodeReader *temp = currentReader;
        currentReader = currentReader->next;
        delete temp;
    }
    
    // Giải phóng bảng băm
    for(int i=0; i<TABLE_SIZE; ++i) {
        HashNode* node = hashTable[i];
        while(node) {
            HashNode* temp = node;
            node = node->next;
            // Giải phóng danh sách liên kết sách trong node hash
            NodeBook* list = temp->list;
            while(list) {
                NodeBook* tList = list;
                list = list->next;
                delete tList;
            }
            delete temp;
        }
    }
}

// --- QUẢN LÝ FILE SÁCH ---

void LibrarySystem::DocFileSach(const string& fileName) {
    Sach::docFileInput(fileName, HeadDsSach);
    // Sau khi thêm sách mới, cần đồng bộ lại bộ đếm ID
    if (HeadDsSach != nullptr) {
        Sach::resyncCounters(HeadDsSach);
    }
    cout << " Da nap danh sach sach tu file: " << fileName << endl;
}

void LibrarySystem::GhiFileSach(const string& fileName) const {
    Sach::ghiFile(fileName, HeadDsSach);
    cout << " Da luu danh sach sach vao file: " << fileName << endl;
}

void LibrarySystem::DocFileHeThong(const string& fileName) {
    ifstream in(fileName);
    if (!in.is_open()) {
        // Nếu không có file thì thôi, không báo lỗi để tránh phiền
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) continue;

        stringstream ss(line);
        string ma, ten, tacGia, theLoai, nhaXB, diemTBStr, soDanhGiaStr, imagePath;
        int namXB = 0, soLuong = 0; // Khởi tạo giá trị mặc định

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');
        string namStr; // Đọc chuỗi trước rồi chuyển số sau
        getline(ss, namStr, '|'); 
        getline(ss, nhaXB, '|');
        string slStr;
        getline(ss, slStr, '|');
        
        getline(ss, diemTBStr, '|');
        getline(ss, soDanhGiaStr, '|');
        getline(ss, imagePath);  
        // [SỬA LỖI] Dùng try-catch để bắt lỗi nếu dữ liệu không phải số
        try {
            if (!namStr.empty()) namXB = stoi(namStr);
        } catch (...) { namXB = 0; }

        try {
            if (!slStr.empty()) soLuong = stoi(slStr);
        } catch (...) { soLuong = 0; }

        double tong = 0; 
        int soDG = 0;
        
        try {
            if (!diemTBStr.empty()) tong = stod(diemTBStr);
        } catch (...) { tong = 0.0; }

        try {
            if (!soDanhGiaStr.empty()) soDG = stoi(soDanhGiaStr);
        } catch (...) { soDG = 0; }
        Sach* sachMoi = Sach::createFromData(ten, tacGia, theLoai, namXB, nhaXB);
        
        // Ghi đè lại ID cũ từ file
        if(!ma.empty()) sachMoi->setMaSach(ma);
        
        sachMoi->setSoLuong(soLuong);
        sachMoi->setDanhGia(tong, soDG);
        sachMoi->setImagePath(imagePath);

        NodeBook* newNode = new NodeBook(sachMoi);
        newNode->next = nullptr;

        if (HeadDsSach == nullptr) HeadDsSach = newNode; 
        else {
            NodeBook* temp = HeadDsSach;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode;
        }
    }
    in.close();
    if (HeadDsSach != nullptr) {
        Sach::resyncCounters(HeadDsSach);
    }

    cout << " Doc du lieu tu file he thong thanh cong!\n";
}
void LibrarySystem::GhiFileHeThong(const string& fileName) const {
    ofstream out(fileName);
    if (!out.is_open()) {
        cerr << "Khong the mo file " << fileName << " de ghi.\n";
        return;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        out << current->data->toCSV() << "\n";
        current = current->next;
    }
    out.close();
    cout << "Da cap nhat file he thong.\n";
}

// --- QUẢN LÝ ĐỘC GIẢ ---

void LibrarySystem::DocFileDocGia() {
    ifstream in("DocGia.txt");
    if (!in.is_open()) {
        cerr << "Khong tim thay file DocGia.txt.\n";
        return;
    }

    string line;
    int maxID = 0;
    int count = 0;

    while (getline(in, line)) {
        if (line.empty() || line == "\r" || line == "\n") continue;

        stringstream ss(line);
        string ma, hoten, sdt, email, user, pass, ngayDKStr;
        getline(ss, ma, '|');
        getline(ss, hoten, '|');
        getline(ss, sdt, '|');
        getline(ss, email, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');
        getline(ss, ngayDKStr);

        std::tm tm = {};
        std::istringstream ssNgay(ngayDKStr);
        ssNgay >> std::get_time(&tm, "%d/%m/%Y"); 
        time_t ngayDK = mktime(&tm);

        Reader* newReader = new Reader(ma, hoten, sdt, email, user, pass, ngayDK);
        NodeReader* newNode = new NodeReader(newReader);
        newNode->next = nullptr;

        if (HeadDsDocGia == nullptr) HeadDsDocGia = newNode; 
        else {
            NodeReader* temp = HeadDsDocGia;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode; 
        }

        try {
            int idNum = stoi(ma.substr(1));
            if (idNum > maxID) maxID = idNum;
        } catch (...) {}
        count++;
    }
    in.close();
    Reader::setReaderCount(maxID + 1);
    cout << " Da nap danh sach doc gia (" << count << " doc gia).\n";
}

// Trả về danh sách liên kết thay vì vector
NodeBorrowerInfo* LibrarySystem::TimNguoiMuonSach(const std::string& maSach) const {
    NodeBorrowerInfo* head = nullptr; 
    
    NodeReader* currentReader = HeadDsDocGia;
    while (currentReader != nullptr) {
        Reader* docGia = currentReader->data;
        
        NodeMuonSach* currentPhieu = docGia->getDanhSachPhieuMuon();
        while (currentPhieu != nullptr) {
            PhieuMuonSach* phieu = currentPhieu->phieu;
            if (phieu->sach->getMaSach() == maSach) {
                BorrowerInfo info;
                info.maDocGia = docGia->getMaID();
                info.tenDocGia = docGia->getHoTen();
                info.ngayMuon = phieu->ngayMuon;
                info.ngayHetHan = phieu->ngayHetHan;
                info.trangThai = phieu->trangThaiHan();
                info.daQuaHan = phieu->daQuaHan();

                NodeBorrowerInfo* newNode = new NodeBorrowerInfo(info);
                newNode->next = head;
                head = newNode;
            }
            currentPhieu = currentPhieu->next;
        }
        currentReader = currentReader->next;
    }
    return head;
}

// [SỬA LỖI] Xóa danh sách cũ trước khi đọc
void LibrarySystem::DocDanhSachMuonCuaDocGia(Reader* docGia) {
    NodeMuonSach* oldHead = docGia->getDanhSachPhieuMuon();
    while (oldHead != nullptr) {
        NodeMuonSach* temp = oldHead;
        oldHead = oldHead->next;
        if(temp->phieu) {
            delete temp->phieu->sach; 
            delete temp->phieu;
        }
        delete temp;
    }
    docGia->setDanhSachPhieuMuon(nullptr); 

    string fileName = "muon_tra/MuonSach_" + docGia->getMaID() + ".txt";
    ifstream file(fileName);
    if (!file.is_open()) return;

    string line;
    NodeMuonSach* head = nullptr;
    NodeMuonSach* tail = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string maSach;
        time_t ngayMuon, ngayHetHan;

        getline(ss, maSach, '|');
        ss >> ngayMuon; ss.ignore();
        ss >> ngayHetHan;

        NodeBook* current = HeadDsSach;
        while (current != nullptr) {
            if (current->data->getMaSach() == maSach) {
                PhieuMuonSach* phieu = new PhieuMuonSach(current->data->clone(), ngayMuon, ngayHetHan);
                NodeMuonSach* newNode = new NodeMuonSach(phieu);
                if (head == nullptr) head = tail = newNode;
                else { tail->next = newNode; tail = newNode; }
                break;
            }
            current = current->next;
        }
    }
    docGia->setDanhSachPhieuMuon(head);
    file.close();
}

// [SỬA LỖI] Dùng ios::trunc
void LibrarySystem::GhiDanhSachMuonCuaDocGia(Reader* docGia) {
    string fileName = "muon_tra/MuonSach_" + docGia->getMaID() + ".txt";
    ofstream file(fileName, ios::trunc); 
    
    if (!file.is_open()) return;

    NodeMuonSach* current = docGia->getDanhSachPhieuMuon();
    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        file << phieu->sach->getMaSach() << "|"
             << phieu->ngayMuon << "|"
             << phieu->ngayHetHan << "\n";
        current = current->next;
    }
    file.close();
}

// --- TÌM KIẾM & XÓA & CẬP NHẬT ---

void LibrarySystem::TimSach(const string& keyword) {
    string key = keyword;
    for (char &c : key) c = tolower(c);
    int index = hashFunction(key);
    bool found = false;
    HashNode* node = hashTable[index];
    while (node != nullptr) {
        if (node->key == key) {
            cout << "=== Ket qua tu bang bam ===\n";
            NodeBook* p = node->list;
            while (p != nullptr) {
                p->data->hienThiThongTin();
                p = p->next;
            }
            found = true;
            break;
        }
        node = node->next;
    }
    if (!found) cout << "Khong tim thay sach.\n";
}

void LibrarySystem::XoaSach(const string &maSach) {
    NodeBook *current = HeadDsSach;
    NodeBook *prev = nullptr;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            if (prev == nullptr) HeadDsSach = current->next;
            else prev->next = current->next;
            delete current;
            cout << "Da xoa sach: " << maSach << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void LibrarySystem::CapNhatThongTinSach() {
    // Logic cập nhật trên console (giữ nguyên nếu cần hoặc để trống nếu dùng GUI hoàn toàn)
    if (!HeadDsSach) { cout << "Khong co sach.\n"; return; }
    string maSach; cout << "Nhap ma sach: "; getline(cin, maSach);
    NodeBook* current = HeadDsSach;
    while (current && current->data->getMaSach() != maSach) current = current->next;
    if (!current) { cout << "Khong tim thay.\n"; return; }
    // ... (code nhập liệu console cũ, có thể giữ hoặc bỏ)
}

// --- MƯỢN TRẢ & ĐÁNH GIÁ ---

// [MỚI] Hàm đếm tổng số sách đang được mượn
int LibrarySystem::DemTongSachDangMuon(const string& maSach) {
    int count = 0;
    NodeReader* readerNode = HeadDsDocGia;
    while (readerNode != nullptr) {
        Reader* docGia = readerNode->data;
        // Đảm bảo đã load danh sách mượn
        if (docGia->getDanhSachPhieuMuon() == nullptr) {
            DocDanhSachMuonCuaDocGia(docGia);
        }

        NodeMuonSach* borrowNode = docGia->getDanhSachPhieuMuon();
        while (borrowNode != nullptr) {
            if (borrowNode->phieu->sach->getMaSach() == maSach) {
                count++;
            }
            borrowNode = borrowNode->next;
        }
        readerNode = readerNode->next;
    }
    return count;
}

void LibrarySystem::MuonSach(Reader* docGia, const string& maSach) {
    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            current->data->muonSach(); 
            docGia->themPhieuMuonSach(current->data);
            docGia->ghiLichSu("Muon", current->data);

            GhiDanhSachMuonCuaDocGia(docGia);
            GhiFileHeThong("DanhSachSach.txt");
            cout << "\nMuon thanh cong: " << current->data->getTenSach() << endl;
            return;
        }
        current = current->next;
    }
}

// [CẬP NHẬT] Không hỏi đánh giá console
void LibrarySystem::TraSach(Reader* docGia, const string& maSach) {
    if (!docGia->DaMuonSach(maSach)) return;

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            current->data->traSach();
            docGia->xoaPhieuMuonSach(maSach);
            docGia->ghiLichSu("Tra", current->data);

            GhiDanhSachMuonCuaDocGia(docGia);
            GhiFileHeThong("DanhSachSach.txt");
            cout << "\nTra thanh cong: " << current->data->getTenSach() << endl;
            return;
        }
        current = current->next;
    }
}
void LibrarySystem::DanhGiaSach(Reader* docGia, Sach* sach, int diemMoi) {
    int diemCu = LayDiemDanhGia(docGia->getMaID(), sach->getMaSach());

    string danhGiaText;
    if (diemMoi >= 9) danhGiaText = "Xuat sac";
    else if (diemMoi >= 8) danhGiaText = "Rat hay";
    else if (diemMoi >= 6) danhGiaText = "Hay";
    else if (diemMoi >= 4) danhGiaText = "Tam duoc";
    else danhGiaText = "Te";
    // TRƯỜNG HỢP 1: CHƯA TỪNG ĐÁNH GIÁ -> THÊM MỚI
    if (diemCu == 0) {
        sach->themDanhGia(diemMoi); // Cộng dồn vào sách
        
        // Ghi nối đuôi vào file
        ofstream out("DanhGia.txt", ios::app);
        if (out.is_open()) {
            out << sach->getMaSach() << "|" 
                << sach->getTenSach() << "|" 
                << sach->getTacGia() << "|" 
                << sach->getNamXuatBan() << "|" 
                << sach->getNhaXuatBan() << "|"
                << docGia->getMaID() << "|" 
                << docGia->getUsername() << "|" 
                << danhGiaText << "|" 
                << diemMoi << endl;
            out.close();
        }
        cout << "Da them danh gia moi (" << diemMoi << " diem).\n";
    }
    // TRƯỜNG HỢP 2: ĐÃ ĐÁNH GIÁ -> CẬP NHẬT (GHI ĐÈ)
    else {
        sach->suaDanhGia(diemCu, diemMoi); // Sửa điểm trong RAM của sách

        // Đọc toàn bộ file đánh giá vào bộ nhớ để sửa dòng tương ứng
        vector<string> lines;
        ifstream in("DanhGia.txt");
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string mSach, tSach, tGia, nam, nxb, mDG, tDG, text, diem;
            // Tách các thành phần để kiểm tra ID
            getline(ss, mSach, '|');
            size_t pos1 = line.find('|'); // Sau MaSach
            // ... Logic tách chuỗi hơi phức tạp, ta dùng cách check string đơn giản hơn:
            
            // Kiểm tra xem dòng này có phải của User và Sách này không
            // Format: MaSach|...|MaDG|...
            // Ta sẽ parse lại dòng này để check chính xác
            stringstream ssCheck(line);
            string checkMS, temp, checkMDG;
            getline(ssCheck, checkMS, '|');
            for(int i=0; i<4; ++i) getline(ssCheck, temp, '|'); // Skip 4 field
            getline(ssCheck, checkMDG, '|');

            if (checkMS == sach->getMaSach() && checkMDG == docGia->getMaID()) {
                // ĐÂY LÀ DÒNG CẦN SỬA -> Tạo dòng mới thay thế
                stringstream newLine;
                newLine << sach->getMaSach() << "|" 
                        << sach->getTenSach() << "|" 
                        << sach->getTacGia() << "|" 
                        << sach->getNamXuatBan() << "|" 
                        << sach->getNhaXuatBan() << "|"
                        << docGia->getMaID() << "|" 
                        << docGia->getUsername() << "|" 
                        << danhGiaText << "|" 
                        << diemMoi;
                lines.push_back(newLine.str());
            } else {
                // Không phải dòng cần sửa -> Giữ nguyên
                lines.push_back(line);
            }
        }
        in.close();
        ofstream out("DanhGia.txt", ios::trunc);
        for (const string& l : lines) {
            out << l << endl;
        }
        out.close();
        cout << "Da cap nhat danh gia tu " << diemCu << " -> " << diemMoi << " diem.\n";
    }
    GhiFileHeThong("DanhSachSach.txt");
}
// [MỚI] Kiểm tra và lấy điểm đánh giá
bool LibrarySystem::KiemTraDaDanhGia(const string& maDocGia, const string& maSach) {
    ifstream in("DanhGia.txt");
    if (!in.is_open()) return false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string mSach, temp, mDG;
        getline(ss, mSach, '|');
        for(int i=0; i<4; ++i) getline(ss, temp, '|'); 
        getline(ss, mDG, '|');
        
        if (mSach == maSach && mDG == maDocGia) { in.close(); return true; }
    }
    in.close();
    return false;
}

int LibrarySystem::LayDiemDanhGia(const string& maDocGia, const string& maSach) {
    ifstream in("DanhGia.txt");
    if (!in.is_open()) return 0;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string mSach, temp, mDG, diemStr;
        getline(ss, mSach, '|');
        for(int i=0; i<4; ++i) getline(ss, temp, '|'); 
        getline(ss, mDG, '|');
        getline(ss, temp, '|'); getline(ss, temp, '|'); 
        getline(ss, diemStr);
        
        if (mSach == maSach && mDG == maDocGia) {
            in.close();
            try { return stoi(diemStr); } catch(...) { return 0; }
        }
    }
    in.close();
    return 0;
}

double LibrarySystem::TinhDiemTrungBinhTuFile(const string& tenSach, const string& tacGia, int namXB, const string& nhaXB) {
    // Logic tính điểm trung bình từ file DanhGia.txt (giữ nguyên nếu cần dùng)
    return 0.0; 
}

// --- AUTHENTICATION & KHÁC ---

bool LibrarySystem::KiemTraDocGiaDaDangKy(const string& tenDangNhap) const {
    NodeReader* current = HeadDsDocGia;
    while (current != nullptr) {
        if (current->data->getUsername() == tenDangNhap) return true;
        current = current->next;
    }
    return false;
}

void LibrarySystem::DangKyDocGia() { /* Logic console cũ... */ }
bool LibrarySystem::DangNhapDocGia(USER* &currentUser) { /* Logic console cũ... */ return false; }
bool LibrarySystem::DangXuat(USER* &currentUser) { /* ... */ return true; }
void LibrarySystem::HienThiTatCaDocGia() const { /* ... */ }
void LibrarySystem::HienThiDanhSachSach() { /* ... */ }
void LibrarySystem::HienThiDocGiaQuaHan() { /* ... */ }
void LibrarySystem::ThongKeSachQuaHan() { /* ... */ }

bool LibrarySystem::DangNhapThuThu(const string &usernameInput, const string &passwordInput, USER* &currentUser) {
    ifstream in("ThuThu.txt");
    if (!in.is_open()) return false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string maID, hoTen, sdt, email, user, pass, chucVu;
        getline(ss, maID, '|'); getline(ss, hoTen, '|'); getline(ss, sdt, '|'); 
        getline(ss, email, '|'); getline(ss, user, '|'); getline(ss, pass, '|'); getline(ss, chucVu);
        if (user == usernameInput && pass == passwordInput) {
            Librarian* tt = new Librarian();
            tt->SetThongTin(maID, hoTen, sdt, email, user, pass); 
            currentUser = tt;
            in.close(); return true;
        }
    }
    in.close(); return false;
}

// --- THỐNG KÊ & XẾP HẠNG (MẢNG TĨNH) ---

void LibrarySystem::XepHangSach() {
    if (HeadDsSach == nullptr) return;

    NodeThongKe* headTK = nullptr;

    // 1. Gom nhóm
    for (NodeBook* cur = HeadDsSach; cur != nullptr; cur = cur->next) {
        Sach* s = cur->data;
        NodeThongKe* p = headTK;
        bool found = false;
        while (p != nullptr) {
            if (p->tenSach == s->getTenSach() && p->tacGia == s->getTacGia() && p->namXB == s->getNamXuatBan()) {
                p->tongDiem += s->getTongDiem();
                p->soDanhGia += s->getSoDanhGia();
                found = true; break;
            }
            p = p->next;
        }
        if (!found) {
            NodeThongKe* newNode = new NodeThongKe(s->getTenSach(), s->getTacGia(), s->getNamXuatBan(), s->getNhaXuatBan(), s->getTongDiem(), s->getSoDanhGia());
            newNode->next = headTK; headTK = newNode;
        }
    }

    // 2. Sắp xếp
    if (headTK != nullptr) {
        for (NodeThongKe* i = headTK; i != nullptr; i = i->next) {
            for (NodeThongKe* j = i->next; j != nullptr; j = j->next) {
                if (i->diemTB() < j->diemTB()) {
                    std::swap(i->tenSach, j->tenSach); std::swap(i->tacGia, j->tacGia);
                    std::swap(i->namXB, j->namXB); std::swap(i->nhaXB, j->nhaXB);
                    std::swap(i->tongDiem, j->tongDiem); std::swap(i->soDanhGia, j->soDanhGia);
                }
            }
        }
    }

    // 3. Lưu vào mảng tĩnh
    top10Count = 0;
    int stt = 1;
    for (NodeThongKe* p = headTK; p != nullptr && stt <= 10; p = p->next) {
        NodeBook* cur = HeadDsSach;
        while (cur != nullptr) {
            if (cur->data->getTenSach() == p->tenSach && cur->data->getTacGia() == p->tacGia && cur->data->getNamXuatBan() == p->namXB) {
                 top10BookIDs[top10Count] = cur->data->getMaSach();
                 top10Count++;
                 break; 
            }
            cur = cur->next;
        }
        stt++;
    }

    while (headTK != nullptr) {
        NodeThongKe* temp = headTK;
        headTK = headTK->next;
        delete temp;
    }
}

void LibrarySystem::DocTatCaDanhSachMuon() {
    NodeReader* cur = HeadDsDocGia;
    while (cur != nullptr) {
        DocDanhSachMuonCuaDocGia(cur->data);
        cur = cur->next;
    }
}
