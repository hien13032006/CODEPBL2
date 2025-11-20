#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

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

        // Ghép tất cả thông tin thành 1 chuỗi
        string info = s->getMaSach() + " " +
                      s->getTenSach() + " " +
                      s->getTacGia() + " " +
                      s->getTheLoai() + " " +
                      s->getNhaXuatBan() + " " +
                      to_string(s->getNamXuatBan());

        // Chuyển về lowercase
        for (char &c : info) c = tolower(c);

        // Tách thành các từ
        stringstream ss(info);
        string word;

        while (ss >> word) {
            int index = hashFunction(word);
            HashNode* node = hashTable[index];

            bool existed = false;

            // Kiểm tra xem key đã tồn tại chưa
            while (node != nullptr) {
                if (node->key == word) {
                    // Thêm sách vào danh sách
                    NodeBook* nb = new NodeBook(s);
                    nb->next = node->list;
                    node->list = nb;

                    existed = true;
                    break;
                }
                node = node->next;
            }

            if (existed) continue;  // đi tới từ tiếp theo

             // Nếu không tồn tại → tạo mới
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
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(buffer);
}

LibrarySystem::LibrarySystem() {
    HeadDsSach = nullptr;
    HeadDsDocGia = nullptr;
}

LibrarySystem::~LibrarySystem() {
    // Giai phong danh sach sach
    NodeBook *currentBook = HeadDsSach;
    while (currentBook != nullptr) {
        NodeBook *temp = currentBook;
        currentBook = currentBook->next;
        delete temp;
    }

    // Giai phong danh sach doc gia
    NodeReader *currentReader = HeadDsDocGia;
    while (currentReader != nullptr) {
        NodeReader *temp = currentReader;
        currentReader = currentReader->next;
        delete temp;
    }
}


void LibrarySystem::DocFileSach(const string& fileName) {
    Sach::docFileInput(fileName, HeadDsSach);
    cout << " Da nap danh sach sach tu file: " << fileName << endl;
}

void LibrarySystem::GhiFileSach(const string& fileName) const {
    Sach::ghiFile(fileName, HeadDsSach);
    cout << " Da luu danh sach sach vao file: " << fileName << endl;
}

void LibrarySystem::DocFileHeThong(const string& fileName) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cerr << "Khong the mo file " << fileName << endl;
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string ma, ten, tacGia, theLoai, nhaXB, diemTBStr, soDanhGiaStr;
        int namXB, soLuong;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');
        ss >> namXB;
        ss.ignore();
        getline(ss, nhaXB, '|');
        ss >> soLuong;
        ss.ignore();
        getline(ss, diemTBStr, '|');
        getline(ss, soDanhGiaStr);  


        double tong = 0;
        int soDG = 0;
        if (!diemTBStr.empty()) tong = stod(diemTBStr);
        if (!soDanhGiaStr.empty()) soDG = stoi(soDanhGiaStr);

        // Tạo đối tượng phù hợp với thể loại
        Sach* sachMoi = Sach::createFromData(ten, tacGia, theLoai, namXB, nhaXB);
        sachMoi->setMaSach(ma);
        sachMoi->setSoLuong(soLuong);
        sachMoi->setDanhGia(tong, soDG);

        // Thêm vào cuối danh sách liên kết
        NodeBook* newNode = new NodeBook(sachMoi);
        newNode->next = nullptr;

        if (HeadDsSach == nullptr) {
            HeadDsSach = newNode; 
        } else {
            NodeBook* temp = HeadDsSach;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode; // thêm vào cuối danh sách
        }

    }

    in.close();
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
        out << current->data->getMaSach() << "|"
            << current->data->getTenSach() << "|"
            << current->data->getTacGia() << "|"
            << current->data->getTheLoai() << "|"
            << current->data->getNamXuatBan() << "|"
            << current->data->getNhaXuatBan() << "|"
            << current->data->getSoLuong() << "|"
            << current->data->getTongDiem() << "|"
            << current->data->getSoDanhGia()
            << "\n";
        current = current->next;
    }

    out.close();
    cout << "Da cap nhat file he thong sau khi muon/tra sach.\n";
}


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


        // Tạo đối tượng Reader bằng new (trên heap)
        Reader* newReader = new Reader(ma, hoten, sdt, email, user, pass);


        NodeReader* newNode = new NodeReader(newReader);
        newNode->next = nullptr;

        if (HeadDsDocGia == nullptr) {
            HeadDsDocGia = newNode; 
        } else {
            NodeReader* temp = HeadDsDocGia;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode; // thêm vào cuối danh sách
        }

        try {
            int idNum = stoi(ma.substr(1));
            if (idNum > maxID) maxID = idNum;
        } catch (...) {}

        count++;
    }

    in.close();

    Reader::setReaderCount(maxID + 1);

    cout << " Da nap danh sach doc gia tu file (" << count << " doc gia).\n";


    cin.clear();
}

void LibrarySystem::DocDanhSachMuonCuaDocGia(Reader* docGia) {
    string fileName = "MuonSach_" + docGia->getMaID() + ".txt";
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
        ss >> ngayMuon;
        ss.ignore();
        ss >> ngayHetHan;

        // Tìm sách trong hệ thống
        NodeBook* current = HeadDsSach;
        while (current != nullptr) {
            if (current->data->getMaSach() == maSach) {

                PhieuMuonSach* phieu = new PhieuMuonSach(
                    current->data->clone(),
                    ngayMuon,
                    ngayHetHan
                );

                NodeMuonSach* newNode = new NodeMuonSach(phieu);

                if (head == nullptr) {
                    head = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
                break;
            }
            current = current->next;
        }
    }
    // GÁN CHÍNH XÁC DANH SÁCH VỪA ĐỌC CHO ĐỘC GIẢ
    docGia->setDanhSachPhieuMuon(head);

    file.close();
}


//Ghi danh sách sách đang mượn của độc giả
void LibrarySystem::GhiDanhSachMuonCuaDocGia(Reader* docGia) {
    string fileName = "MuonSach_" + docGia->getMaID() + ".txt";
    ofstream file(fileName, ios::trunc); // ghi đè toàn bộ, nhưng chỉ các sách đang mượn còn trong bộ nhớ
    
    if (!file.is_open()) {
        cout << "Khong the ghi file muon sach!\n";
        return;
    }

    NodeMuonSach* current = docGia->getDanhSachPhieuMuon();
    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        // chỉ ghi sách đang mượn
        file << phieu->sach->getMaSach() << "|"
             << phieu->ngayMuon << "|"
             << phieu->ngayHetHan << "\n";
        current = current->next;
    }

    file.close();
}

void LibrarySystem::TimSach(const string& keyword) {
    string key = keyword;
    for (char &c : key) c = tolower(c);

    int index = hashFunction(key);

    bool found = false;

    // Tìm kiếm nhanh bằng bảng băm
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

    if (!found)
        cout << "Khong tim thay sach.\n";
}


void LibrarySystem::XoaSach(const string &maSach) {
    NodeBook *current = HeadDsSach;
    NodeBook *prev = nullptr;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            if (prev == nullptr) {
                HeadDsSach = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            cout << "Da xoa sach voi ma: " << maSach << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Khong tim thay sach voi ma: " << maSach << endl;
}

void LibrarySystem::CapNhatThongTinSach() {
    if (!HeadDsSach) {
        cout << "Hien tai khong co sach nao trong thu vien.\n";
        return;
    }

    string maSach;
    cout << "Nhap ma sach can cap nhat: ";
    cin >> maSach;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    NodeBook* current = HeadDsSach;
    while (current && current->data->getMaSach() != maSach) {
        current = current->next;
    }

    if (!current) {
        cout << " Khong tim thay sach voi ma: " << maSach << endl;
        return;
    }

    Sach* sach = current->data;
    cout << "Thong tin sach hien tai:\n";
    sach->hienThiThongTin();

    bool tiepTuc = true;
    while (tiepTuc) {
        cout << "\n========== CAP NHAT THONG TIN ==========\n";
        cout << "1. Ten sach\n";
        cout << "2. Tac gia\n";
        cout << "3. The loai\n";
        cout << "4. Nam xuat ban\n";
        cout << "5. Nha xuat ban\n";
        cout << "0. Huy bo cap nhat\n";
        cout << "========================================\n";
        cout << "Nhap lua chon: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string tenMoi;
                cout << "Nhap ten sach moi: ";
                getline(cin, tenMoi);
                sach->setTenSach(tenMoi);
                cout << "Da cap nhat ten sach thanh cong!\n";
                break;
            }
            case 2: {
                string tacGiaMoi;
                cout << "Nhap tac gia moi: ";
                getline(cin, tacGiaMoi);
                sach->setTacGia(tacGiaMoi);
                cout << "Da cap nhat tac gia thanh cong!\n";
                break;
            }
            case 3: {
                string theLoaiMoi;
                cout << "Nhap the loai moi: ";
                getline(cin, theLoaiMoi);
                sach->setTheLoai(theLoaiMoi);
                cout << "Da cap nhat the loai thanh cong!\n";
                break;
            }
            case 4: {
                int namMoi;
                cout << "Nhap nam xuat ban moi: ";
                cin >> namMoi;
                sach->setNamXuatBan(namMoi);
                cout << "Da cap nhat nam xuat ban thanh cong!\n";
                break;
            }
            case 5: {
                string nhaXBMoi;
                cout << "Nhap nha xuat ban moi: ";
                getline(cin, nhaXBMoi);
                sach->setNhaXuatBan(nhaXBMoi);
                cout << "Da cap nhat nha xuat ban thanh cong!\n";
                break;
            }
        
            case 0:
                cout << "Huy bo cap nhat.\n";
                tiepTuc = false;
                continue;
            default:
                cout << " Lua chon khong hop le!\n";
                continue;
        }

        char ans;
        cout << "\nBan co muon sua thong tin khac khong? (y/n): ";
        cin >> ans;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (ans == 'n' || ans == 'N') tiepTuc = false;
    }

    Sach::ghiFile("DanhSachSach.txt", HeadDsSach);
    cout << "\nDa cap nhat va luu thay doi vao file he thong.\n";
}


BorrowResult LibrarySystem::MuonSach(Reader* docGia, const string& maSach) {
    BorrowResult result;
    result.status = BorrowStatus::NOT_FOUND;
    result.book = nullptr;

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            result.book = current->data;

            if (current->data->getSoLuong() == 0) {
                result.status = BorrowStatus::OUT_OF_STOCK;
                return result;
            }
            if (docGia->DaMuonSach(maSach)) {
                result.status = BorrowStatus::ALREADY_BORROWED;
                return result;
            }
            if (docGia->DemSachDaMuon() >= docGia->getGioiHanMuon()) {
                result.status = BorrowStatus::LIMIT_REACHED;
                return result;
            }
            if (docGia->DemSachQuaHan() > 0) {
                result.status = BorrowStatus::OVERDUE;
                return result;
            }

            // Cho phép mượn
            current->data->muonSach(); 
            docGia->themPhieuMuonSach(current->data);
            docGia->ghiLichSu("Muon", current->data);
            GhiDanhSachMuonCuaDocGia(docGia);
            GhiFileHeThong("DanhSachSach.txt");

            // Lấy ngày mượn và ngày trả từ phiếu mới
            NodeMuonSach* phieuMoi = docGia->getDanhSachPhieuMuon();
            if (phieuMoi != nullptr) {
                PhieuMuonSach* p = phieuMoi->phieu;
                result.borrowDate = timeToString(p->ngayMuon);
                result.returnDate = timeToString(p->ngayHetHan);
            }

            result.status = BorrowStatus::SUCCESS;
            return result;
        }
        current = current->next;
    }

    result.status = BorrowStatus::NOT_FOUND;
    return result;
}

Sach* LibrarySystem::TraSach(Reader* docGia, const string& maSach, bool &quaHan, int &soNgayQuaHan) {
    quaHan = false;
    soNgayQuaHan = 0;

    NodeReader* tmp = HeadDsDocGia;
       while (tmp != nullptr) {
       DocDanhSachMuonCuaDocGia(tmp->data);
       tmp = tmp->next;
    }

    if (!docGia->DaMuonSach(maSach)) {
        return nullptr;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {

            // kiểm tra quá hạn
            NodeMuonSach* phieuNode = docGia->getDanhSachPhieuMuon();
            while (phieuNode != nullptr) {
                if (phieuNode->phieu->sach->getMaSach() == maSach) {
                    if (phieuNode->phieu->daQuaHan()) {
                        quaHan = true;
                        soNgayQuaHan = abs(phieuNode->phieu->soNgayConLai());
                    }
                    break;
                }
                phieuNode = phieuNode->next;
            }

            // cập nhật kho
            current->data->traSach();

            // xóa phiếu
            docGia->xoaPhieuMuonSach(maSach);
            docGia->ghiLichSu("Tra", current->data);

            // ghi file
            GhiDanhSachMuonCuaDocGia(docGia);
            GhiFileHeThong("DanhSachSach.txt");

            return current->data;   // ✔ trả về sách
        }
        current = current->next;
    }

    return nullptr;
}


void LibrarySystem::DanhGiaSach(Reader* docGia, Sach* sach, int diem, const string& text) {
    sach->themDanhGia(diem);
    GhiFileHeThong("DanhSachSach.txt");

    ofstream out("DanhGia.txt", ios::app);
    if (out.is_open()) {
        out << sach->getMaSach() << "|"
            << sach->getTenSach() << "|"
            << sach->getTacGia() << "|"
            << sach->getNamXuatBan() << "|"
            << sach->getNhaXuatBan() << "|"
            << docGia->getMaID() << "|"
            << docGia->getUsername() << "|"
            << text << "|"
            << diem << endl;
        out.close();
    }
}


double LibrarySystem::TinhDiemTrungBinhTuFile(const string& tenSach,
                                              const string& tacGia,
                                              int namXB,
                                              const string& nhaXB) {
    ifstream in("DanhGia.txt");
    if (!in.is_open()) return 0.0;

    string line;
    double tong = 0;
    int dem = 0;

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string ma, ten, maDG, tenDG, diemStr, dgText, tg, namStr, nxb;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tg, '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');
        getline(ss, maDG, '|');
        getline(ss, tenDG, '|');
        getline(ss, dgText, '|');
        getline(ss, diemStr, '|');

        int diem = stoi(diemStr);
        int nam = stoi(namStr);

        if (ten == tenSach && tg == tacGia && nam == namXB && nxb == nhaXB) {
            tong += diem;
            dem++;
        }
    }
    in.close();

    return dem == 0 ? 0.0 : tong / dem;
}

void LibrarySystem::HienThiDanhSachSach()  {
    NodeBook *current = HeadDsSach;
    if (current == nullptr) {
        cout << "Khong co sach trong thu vien." << endl;
        return;
    }

    cout << "+-------------------------------------------------------------------------------------------------------------------------------+\n";
    cout << left << setw(15) << "Ma sach"
         << setw(25) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(15) << "The loai"
         << setw(10)  << "NamXB"
         << setw(21) << "Nha xuat ban"
         << setw(15) << "Tinh trang"
         << setw(10) << "Diem danh gia"
         << "|\n";
    cout << "+-----------------------------------------------------------------------------------------------------------------------------+\n";

    while (current != nullptr) {
        current->data->hienThiThongTin();
        current = current->next;
    }

    cout << "+--------------------------------------------------------------------------------------------------------------------------+\n";
    
}

void LibrarySystem::HienThiDocGiaQuaHan() {
    cout << "\n===== DANH SACH DOC GIA CO SACH QUA HAN =====\n";
    cout << left
         << setw(12) << "Ma doc gia"
         << setw(25) << "Ho ten"
         << setw(15) << "SDT"
         << setw(30) << "Email"
         << setw(15) << "So sach QH"
         << endl;
    cout << string(97, '-') << endl;

    NodeReader* current = HeadDsDocGia;
    bool found = false;
    
    while (current != nullptr) {
        Reader* docGia = current->data;
        int soQuaHan = docGia->DemSachQuaHan();
        
        if (soQuaHan > 0) {
            cout << left
                 << setw(12) << docGia->getMaID()
                 << setw(25) << docGia->getHoTen()
                 << setw(15) << docGia->getSDT()
                 << setw(30) << docGia->getEmail()
                 << "\033[1;31m" << setw(15) << soQuaHan << "\033[0m"
                 << endl;
            found = true;
        }
        
        current = current->next;
    }
    
    if (!found) {
        cout << "Khong co doc gia nao qua han tra sach.\n";
    }
    
    cout << string(97, '-') << endl;
}

//Thống kê sách quá hạn
void LibrarySystem::ThongKeSachQuaHan() {
    cout << "\n===== THONG KE SACH QUA HAN =====\n";
    
    int tongDocGiaQuaHan = 0;
    int tongSachQuaHan = 0;
    
    NodeReader* current = HeadDsDocGia;
    while (current != nullptr) {
        int soQuaHan = current->data->DemSachQuaHan();
        if (soQuaHan > 0) {
            tongDocGiaQuaHan++;
            tongSachQuaHan += soQuaHan;
        }
        current = current->next;
    }
    
    cout << "Tong so doc gia co sach qua han: " << tongDocGiaQuaHan << endl;
    cout << "Tong so sach qua han: " << tongSachQuaHan << endl;
    cout << "==================================\n";
}

//kiểm tra tính hợp lệ của SĐT
bool kiemTraSDT(const string& sdt) {
    if (sdt.length() != 10 || sdt[0] != '0') return false; // phải đủ 10 số và bắt đầu bằng số 0
    // số thứ 2 phải trong các số 3,5,7,8,9
    char dauSo = sdt[1];
    if (dauSo != '3' && dauSo != '5' && dauSo != '7' && dauSo != '8' && dauSo != '9') {
        return false;
    }
    // Kiểm tra tất cả là số
    for (char c : sdt) {
        if (!isdigit(c)) return false;
    }
    return true;
}

//kiểm tra tính hợp lệ của email với đuôi @gmail.com
bool kiemTraEmail(const string& email) {
    const string duoi = "@gmail.com";
    if (email.length() <= duoi.length()) return false;
    return email.substr(email.length() - duoi.length()) == duoi;
}

//Kiểm tra độ dài của mật khẩu: giới hạn = 8
bool kiemTraMatKhau(const string& pass) {
    return pass.length() == 8;
}

//kiểm tra xem username đã tồn tại hay chưa
bool LibrarySystem::KiemTraDocGiaDaDangKy(const string& tenDangNhap) const {
    NodeReader* current = HeadDsDocGia;
    while (current != nullptr) {
        if (current->data->getUsername() == tenDangNhap)
            return true;
        current = current->next;
    }
    return false;
}

bool LibrarySystem::DangKyDocGia(const string &hoTen,
                                 const string &sdt,
                                 const string &email,
                                 const string &user,
                                 const string &pass)
{
    // Kiểm tra hợp lệ
    if (!kiemTraSDT(sdt)) return false;
    if (!kiemTraEmail(email)) return false;
    if (KiemTraDocGiaDaDangKy(user)) return false;
    if (!kiemTraMatKhau(pass)) return false;

    // Tạo Reader mới
    Reader* dg = new Reader();
    dg->SignUp(hoTen, sdt, email, user, pass);

    // Thêm vào danh sách liên kết
    NodeReader* newNode = new NodeReader(dg);
    newNode->next = HeadDsDocGia;
    HeadDsDocGia = newNode;

    // Lưu file
    ofstream out("DocGia.txt", ios::app);
    if (!out.is_open()) return false;

    out << dg->toCSV() << endl;
    out.close();

    return true;
}


bool LibrarySystem::DangNhapDocGia(const string &username, 
                                   const string &password, 
                                   USER* &currentUser) 
{
    NodeReader *current = HeadDsDocGia;

    while (current != nullptr) {
        if (current->data->Login(username, password)) {
            currentUser = current->data;
            return true;
        }
        current = current->next;
    }

    return false;   // Sai username/password
}

bool LibrarySystem::DangNhapThuThu(const string &username, const string &password, USER* &currentUser) 
{
    ifstream in("ThuThu.txt");
    if (!in.is_open()) return false;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string maID, hoTen, sdt, email, user, pass;

        getline(ss, maID, '|');
        getline(ss, hoTen, '|');
        getline(ss, sdt, '|');
        getline(ss, email, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');

        if (user == username && pass == password) {
            Librarian* tt = new Librarian();
            tt->SetThongTin(maID, hoTen, sdt, email, user, pass);
            currentUser = tt;
            return true;
        }
    }

    return false;
}


bool LibrarySystem::DangXuat(USER* &currentUser) {
    if (currentUser != nullptr) {
        currentUser->Logout();
        currentUser = nullptr;
        return true;
    }
    cout << "Khong co nguoi dung nao dang dang nhap." << endl;
    return false;
}

void LibrarySystem::HienThiTatCaDocGia() const {
    NodeReader *current = HeadDsDocGia;
    if (current == nullptr) {
        cout << "Khong co doc gia nao dang ky." << endl;
        return;
    }
    cout << "=====THONG TIN TAT CA NGUOI DUNG=====\n";
    cout << left << setw(13) << "Ma ID"      
                 << setw(23) << "Ho Ten"     
                 << setw(17) << "So DT"      
                 << setw(20) << "Email"      
                 << setw(15) << "Username"
                 << setw(15) << "Ngay DK"
                 << setw(10) << "Da muon"
                 << "\n";  

    while (current != nullptr) {
        current->data->HienThiThongTin();
        current = current->next;
    }
}

void LibrarySystem::XepHangSach() {
    if (HeadDsSach == nullptr) {
        cout << "Thu vien chua co sach nao!\n";
        return;
    }

    //Danh sách thống kê trung gian
    struct NodeThongKe {
        string tenSach;
        string tacGia;
        int namXB;
        string nhaXB;
        double tongDiem;
        int soDanhGia;
        NodeThongKe* next;
        NodeThongKe(string ten, string tg, int nxb, string nxbx,
                    double tong, int so)
            : tenSach(ten), tacGia(tg), namXB(nxb), nhaXB(nxbx),
              tongDiem(tong), soDanhGia(so), next(nullptr) {}
        double diemTB() const {
            return soDanhGia == 0 ? 0 : tongDiem / soDanhGia;
        }
    };

    NodeThongKe* headTK = nullptr;

    //Gom nhóm sách giống nhau
    for (NodeBook* cur = HeadDsSach; cur != nullptr; cur = cur->next) {
        Sach* s = cur->data;

        NodeThongKe* p = headTK;
        bool found = false;
        while (p != nullptr) {
            if (p->tenSach == s->getTenSach() &&
                p->tacGia == s->getTacGia() &&
                p->namXB == s->getNamXuatBan() &&
                p->nhaXB == s->getNhaXuatBan()) {
                p->tongDiem += s->getTongDiem();
                p->soDanhGia += s->getSoDanhGia();
                found = true;
                break;
            }
            p = p->next;
        }

        if (!found) {
            NodeThongKe* newNode = new NodeThongKe(
                s->getTenSach(),
                s->getTacGia(),
                s->getNamXuatBan(),
                s->getNhaXuatBan(),
                s->getTongDiem(),
                s->getSoDanhGia()
            );
            newNode->next = headTK;
            headTK = newNode;
        }
    }

    if (headTK == nullptr) {
        cout << "Chua co sach nao duoc danh gia.\n";
        return;
    }

    //Sắp xếp giảm dần theo điểm trung bình
    for (NodeThongKe* i = headTK; i != nullptr; i = i->next) {
        for (NodeThongKe* j = i->next; j != nullptr; j = j->next) {
            if (i->diemTB() < j->diemTB()) {
                swap(i->tenSach, j->tenSach);
                swap(i->tacGia, j->tacGia);
                swap(i->namXB, j->namXB);
                swap(i->nhaXB, j->nhaXB);
                swap(i->tongDiem, j->tongDiem);
                swap(i->soDanhGia, j->soDanhGia);
            }
        }
    }

    cout << "\n=====TOP 10 SACH DUOC DANH GIA CAO NHAT =====\n";
    cout << left << setw(5) << "STT"
         << setw(30) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(8)  << "NamXB"
         << setw(20) << "NhaXB"
         << setw(10) << "DiemTB" << endl;
    cout << string(95, '-') << endl;

    int stt = 1;
    for (NodeThongKe* p = headTK; p != nullptr && stt <= 10; p = p->next) {
        cout << setw(5) << stt++
             << setw(30) << p->tenSach
             << setw(20) << p->tacGia
             << setw(8)  << p->namXB
             << setw(20) << p->nhaXB
             << setw(10) << fixed << setprecision(1) << p->diemTB()
             << endl;
    }
    cout << string(95, '-') << endl;

    // Giải phóng bộ nhớ tạm 
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

void LibrarySystem::XemThongKe() {
        int tongSoSach = 0;
        int tongSoLuong = 0;
        int soSachDangMuon = 0;
        
        NodeBook* current = HeadDsSach;
        while (current != nullptr) {
            tongSoSach++;
            tongSoLuong += current->data->getSoLuong();
            current = current->next;
        }

        int soDocGia = 0;
        NodeReader* cur = HeadDsDocGia;
        while (cur != nullptr){
            soDocGia++;
            soSachDangMuon += cur->data->DemSachDaMuon();
            cur = cur->next;
        }
        
        cout << "\n========== THONG KE HE THONG ==========\n";
        cout << "\nSACH:\n";
        cout << "  - Tong so dau sach: " << tongSoSach << endl;
        cout << "  - Tong so luong: " << tongSoLuong << endl;
        cout << "  - Dang duoc muon: " << soSachDangMuon << " cuon" << endl;
        cout << "  - Con lai: " << (tongSoLuong - soSachDangMuon) << " cuon" << endl;
        
        cout << "  - So doc gia: " << soDocGia << endl;
        cout << "========================================\n";
}

Sach* LibrarySystem::TimSachTheoMa(const std::string& maSach) {
    NodeBook* current = HeadDsSach;
    while (current) {
        if (current->data->getMaSach() == maSach) return current->data;
        current = current->next;
    }
    return nullptr;
}



