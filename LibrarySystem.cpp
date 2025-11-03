#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
using namespace std;

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
        string ma, ten, tacGia, theLoai, nhaXB, tinhTrangStr, diemTBStr, soDanhGiaStr;
        int namXB;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');
        ss >> namXB;
        ss.ignore();
        getline(ss, nhaXB, '|');
        getline(ss, tinhTrangStr,'|');
        getline(ss, diemTBStr, '|');
        getline(ss, soDanhGiaStr);

        bool tinhTrang = (tinhTrangStr == "Dang con");

        double tong = 0;
        int soDG = 0;
        if (!diemTBStr.empty()) tong = stod(diemTBStr);
        if (!soDanhGiaStr.empty()) soDG = stoi(soDanhGiaStr);

        // Tạo đối tượng phù hợp với thể loại
        Sach* sachMoi = Sach::createFromData(ten, tacGia, theLoai, namXB, nhaXB);
        sachMoi->setMaSach(ma);
        sachMoi->setTinhTrang(tinhTrang ? "Dang con" : "Da muon");
        sachMoi->setDanhGia(tong, soDG);
        // Thêm vào danh sách liên kết
        NodeBook* newNode = new NodeBook(sachMoi);
        newNode->next = HeadDsSach;
        HeadDsSach = newNode;
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
            << (current->data->getTinhTrang()) << "|"
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
        string ma, hoten, sdt, email, user, pass;
        getline(ss, ma, '|');
        getline(ss, hoten, '|');
        getline(ss, sdt, '|');
        getline(ss, email, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');

        // Tạo đối tượng Reader bằng new (trên heap)
        Reader* newReader = new Reader(ma, hoten, sdt, email, user, pass);

        // Tạo node mới trỏ đến Reader này
        NodeReader* newNode = new NodeReader(*newReader);
        newNode->next = HeadDsDocGia;
        HeadDsDocGia = newNode;

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
    cout << "\n📘 Thong tin sach hien tai:\n";
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


void LibrarySystem::TimSach() {
    cout << "Chon tieu chi tim kiem:\n";
    cout << "1. Ma sach\n";
    cout << "2. Ten sach\n";
    cout << "3. Tac gia\n"; 
    cout << "4. Nam xuat ban\n";
    cout << "5. The loai\n";
    cout << "Nhap lua chon cua ban: ";
    int choice;
    cin >> choice;
    cin.ignore();
    NodeBook *current = HeadDsSach;
    bool found = false;
    
    switch(choice) {
        case 1: {
            string maSach;
            cout << "Nhap ma sach can tim: ";
            getline(cin, maSach);
            while (current != nullptr) {
                if (current->data->getMaSach() == maSach) {
                    current->data->hienThiThongTin();
                    found = true;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            string tenSach;
            cout << "Nhap ten sach can tim: ";
            getline(cin, tenSach);
            while (current != nullptr) {
                if (current->data->getTenSach() == tenSach) {
                    current->data->hienThiThongTin();
                    found = true;
                }
                current = current->next;
            }
            break;
        }
        case 3: {
            string tacGia;
            cout << "Nhap tac gia can tim: ";
            getline(cin, tacGia);
            while (current != nullptr) {
                if (current->data->getTacGia() == tacGia) {
                    current->data->hienThiThongTin();
                    found = true;
                }
                current = current->next;
            }
            break;
        }
        case 4: {
            int namXB;
            cout << "Nhap nam xuat ban can tim: ";
            cin >> namXB;
            cin.ignore();
            while (current != nullptr) {
                if (current->data->getNamXuatBan() == namXB) {
                    current->data->hienThiThongTin();
                    found = true;
                }
                current = current->next;
            }
            break;
        }
        case 5: {
            string theLoai;
            cout << "Nhap the loai can tim: ";
            getline(cin, theLoai);
            while (current != nullptr) {
                if (current->data->getTheLoai() == theLoai) {
                    current->data->hienThiThongTin();
                    found = true;
                }
                current = current->next;
            }
            break;
        }
        default:
            cout << "Lua chon khong hop le." << endl;
            return;
    }
    if (!found) {
        cout << "Khong tim thay sach." << endl;
    }
}


void LibrarySystem::MuonSach(Reader* docGia, const string& maSach) {
    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            if (current->data->getTinhTrang() == "Da muon") {
                cout << "Sach da het khong the muon\n";
                return;
            }

            if (docGia->DaMuonSach(maSach)) {
                cout << "Ban da muon sach nay roi.\n";
                return;
            }

            if (docGia->DemSachDaMuon() >= 5) {
                cout << "Ban da muon toi da so sach cho phep.\n";
                return;
            }

            current->data->muonSach(); 

            docGia->themSachDaMuon(current->data); 
            docGia->ghiLichSu("Muon", current->data);

            GhiFileHeThong("DanhSachSach.txt");

            cout << "Muon sach thanh cong " << current->data->getTenSach() << endl;
            return;
        }
        current = current->next;
    }

    cout << "Khong tim thay sach voi ma: " << maSach << endl;
}

void LibrarySystem::TraSach(Reader* docGia, const string& maSach) {
    if (!docGia->DaMuonSach(maSach)) {
        cout << "Ban chua muon sach co ma: " << maSach << endl;
        return;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {

            current->data->traSach(); // Tang so luong cua sach nay trong thu vien

            docGia->xoaSachDaMuon(maSach);// xoa khoi danh sach da muon cua doc gia

            docGia->ghiLichSu("Tra", current->data);


            cout << "Tra sach thanh cong: " << current->data->getTenSach() << endl;
            
            int luaChon;
            cout << "\nBan co muon danh gia cuon sach vua tra khong? (1: Co, 0: Khong): ";
            cin >> luaChon;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (luaChon == 1) {
                DanhGiaSach(docGia, current->data);
            }

            GhiFileHeThong("DanhSachSach.txt");

            return;
        }
        current = current->next;
    }

    cout << "Khong tim thay sach trong he thong.\n";
}

void LibrarySystem::DanhGiaSach(Reader* docGia, Sach* sach) {
    cout << "\n===== DANH GIA SACH =====\n";
    cout << "Ban cam thay cuon \"" << sach->getTenSach() << "nhu the nao:\n";
    cout << "1. Rat hay!\n";
    cout << "2. Hay!\n";
    cout << "3. Binh thuong!\n";
    cout << "4. Khong hay!\n";
    cout << "5. Rat te!\n";
    cout << "==========================\n";
    cout << "Lua chon cua ban: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int diem = 0;
    string danhGiaText;

    switch (choice) {
        case 1: diem = 10; danhGiaText = "Rat hay"; break;
        case 2: diem = 8; danhGiaText = "Hay"; break;
        case 3: diem = 5; danhGiaText = "Binh thuong"; break;
        case 4: diem = 4; danhGiaText = "Khong hay"; break;
        case 5: diem = 2; danhGiaText = "Rat te"; break;
        default:
            cout << "Lua chon khong hop le! Bo qua danh gia.\n";
            return;
    }

    // Cập nhật điểm trung bình của sách
    sach->themDanhGia(diem);
    GhiFileHeThong("DanhSachSach.txt");

    // Ghi vào file đánh giá
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
           << diem << endl;
        out.close();
    }

    cout << " Cam on ban da danh gia cuon sach nay!\n";
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
    DocFileHeThong("DanhSachSach.txt");
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
        if (current->data.getUsername() == tenDangNhap)
            return true;
        current = current->next;
    }
    return false;
}

void LibrarySystem::DangKyDocGia() {
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string hoTen, sdt, email, user, pass;
    cout << "Nhap ho ten: "; getline(cin, hoTen);
    while (true) {
        cout << "Nhap SDT: ";
        getline(cin, sdt);
        if (kiemTraSDT(sdt)) break;
        cout << "So dien thoai khong hop le. Vui long nhap lai.\n";
    }

    while (true) {
        cout << "Nhap Email: ";
        getline(cin, email);
        if (kiemTraEmail(email)) break;
        cout << "Email khong hop le. Vui long nhap lai.\n";
    }

    while (true) {
        cout << "Nhap username: ";
        getline(cin, user);
        if (!KiemTraDocGiaDaDangKy(user)) break;
        cout << "Username da ton tai. Vui long chon ten khac.\n";
    }

    while (true) {
        cout << "Nhap password (Hay nhap dung 8 ki tu): ";
        getline(cin, pass);
        if (kiemTraMatKhau(pass)) break;
        cout << "Password khong hop le. Vui long nhap lai.\n";
    }

    Reader dg;
    dg.SignUp(hoTen, sdt, email, user, pass);

    NodeReader* newNode = new NodeReader(dg);
    newNode->next = HeadDsDocGia;
    HeadDsDocGia = newNode;
    
    ofstream out("DocGia.txt", ios::app); // mở file ở chế độ ghi thêm
    if (out.is_open()) {
        out << dg.toCSV() << endl;
        out.close();
        cout << "Da luu vao file.\n";
    } else {
        cout << "Khong the mo file de luu doc gia.\n";
    }

}


bool LibrarySystem::DangNhapDocGia(USER* &currentUser) {
    string username, password;

    while (true) {
        cout << "\n===== DANG NHAP DOC GIA =====\n";
        cout << "Nhap username: ";
        getline(cin, username);

        cout << "Nhap password: ";
        getline(cin, password);

        NodeReader *current = HeadDsDocGia;
        bool found = false;

        while (current != nullptr) {
            if (current->data.Login(username, password)) {
                currentUser = &current->data;
                cout << "Doc gia \"" << username << "\" da dang nhap thanh cong.\n";
                return true;
            }
            current = current->next;
        }

        cout << " Sai ten dang nhap hoac mat khau. Vui long nhap lai.\n";
    }
}


bool LibrarySystem::DangNhapThuThu(const string &usernameInput, const string &passwordInput, USER* &currentUser) {
    string username = usernameInput;
    string password = passwordInput;

    while (true) {
        ifstream in("ThuThu.txt");
        if (!in.is_open()) {
            cout << "Khong the mo file ThuThu.txt\n";
            return false;
        }

        bool found = false;
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
                cout << "Dang nhap thanh cong!\n";
                found = true;
                break;
            }
        }
        in.close();

        if (found) return true;

        cout << "Ten dang nhap: ";
        getline(cin, username);
        cout << "Mat khau: ";
        getline(cin, password);
    }
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
                 << "\n";  

    while (current != nullptr) {
        current->data.HienThiThongTin();
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



