#include "LibrarySystem.h"
#include <iostream>
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

void LibrarySystem::HienThiDanhSachSach() const {
    NodeBook *current = HeadDsSach;
    if (current == nullptr) {
        cout << "Khong co sach trong thu vien." << endl;
        return;
    }
    while (current != nullptr) {
        current->data->hienThiThongTin();
        current = current->next;
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

            GhiFileHeThong("DanhSachSach.txt");

            cout << "Tra sach thanh cong: " << current->data->getTenSach() << endl;
            return;
        }
        current = current->next;
    }

    cout << "Khong tim thay sach trong he thong.\n";
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

void LibrarySystem::HienThiTatCaDocGia() const {
    NodeReader *current = HeadDsDocGia;
    if (current == nullptr) {
        cout << "Khong co doc gia nao dang ky." << endl;
        return;
    }
    while (current != nullptr) {
        current->data.HienThiThongTin();
        current = current->next;
    }
}

bool LibrarySystem::DangNhapDocGia(const string &username, const string &password, USER* &currentUser) {
    NodeReader *current = HeadDsDocGia;
    while (current != nullptr) {
        if (current->data.Login(username, password)) {
            currentUser = &current->data;
            cout << "Doc gia " << username << " da dang nhap thanh cong." << endl;
            return true;
        }
        current = current->next;
    }
    cout << "Dang nhap that bai. Vui long kiem tra lai thong tin." << endl;
    return false;
}

bool LibrarySystem::DangNhapThuThu(const string &username, const string &password, USER* &currentUser) {
    ifstream in("ThuThu.txt");
    if (!in) {
        cout << "Khong the mo file ThuThu.txt\n";
        return false;
    }

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
            tt->SetThongTin(maID,hoTen, sdt, email, user, pass); // dùng lại hàm SignUp để gán thông tin
            currentUser = tt;
            cout << "Dang nhap thanh cong!\n";
            return true;
        }
    }
    cout << "Dang nhap that bai. Vui long kiem tra lai thong tin.\n";
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
        string ma, ten, tacGia, theLoai, nhaXB, tinhTrangStr;
        int namXB;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');
        ss >> namXB;
        ss.ignore();
        getline(ss, nhaXB, '|');
        getline(ss, tinhTrangStr);

        bool tinhTrang = (tinhTrangStr == "Dang con");

        // Tạo đối tượng phù hợp với thể loại
        Sach* sachMoi = Sach::createFromData(ten, tacGia, theLoai, namXB, nhaXB);
        sachMoi->setMaSach(ma);
        sachMoi->setTinhTrang(tinhTrang ? "Dang con" : "Da muon");
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
            << (current->data->getTinhTrang())
            << "\n";
        current = current->next;
    }

    out.close();
    cout << "Da cap nhat file he thong sau khi muon/tra sach.\n";
}


void LibrarySystem::DocFileDocGia() {
    ifstream in("DocGia.txt");
    if (!in.is_open()) {
        cerr << "Khong tim thay file DanhSachDocGia.txt.\n";
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string ma, hoten, sdt, email, user, pass;
        getline(ss, ma, '|');
        getline(ss, hoten, '|');
        getline(ss, sdt, '|');
        getline(ss, email, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');
        

        Reader newReader;
        newReader.setMaReader(ma);
        newReader.setHoTen(hoten);
        newReader.setSDT(sdt);
        newReader.setEmail(email);
        newReader.setUsername(user);
        newReader.setPassword(pass);
        
        NodeReader* newNode = new NodeReader(newReader);
        newNode->next = HeadDsDocGia;
        HeadDsDocGia = newNode;
    }

    in.close();
    cout << "Da nap danh sach doc gia tu file.\n";
}







