#include "LibrarySystem.h"
#include <iostream>
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

void LibrarySystem::ThemSach(Sach &SachMoi) {
    SachMoi.nhapThongTin();
    NodeBook *newNode = new NodeBook(SachMoi);
    newNode->next = HeadDsSach;
    HeadDsSach = newNode;
    cout << "Da them sach: " << SachMoi.getTenSach() << endl;
}

void LibrarySystem::XoaSach(const string &maSach) {
    NodeBook *current = HeadDsSach;
    NodeBook *prev = nullptr;
    while (current != nullptr) {
        if (current->data.getMaSach() == maSach) {
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
                if (current->data.getMaSach() == maSach) {
                    current->data.hienThiThongTin();
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
                if (current->data.getTenSach() == tenSach) {
                    current->data.hienThiThongTin();
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
                if (current->data.getTacGia() == tacGia) {
                    current->data.hienThiThongTin();
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
                if (current->data.getNamXB() == namXB) {
                    current->data.hienThiThongTin();
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
                if (current->data.getTheLoai() == theLoai) {
                    current->data.hienThiThongTin();
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
        current->data.hienThiThongTin();
        current = current->next;
    }
}

void LibrarySystem::DangKyDocGia() {
    
    string hoTen, sdt, email, user, pass;
    cout << "Nhap ho ten: "; getline(cin, hoTen);
    cout << "Nhap SDT: "; getline(cin, sdt);
    cout << "Nhap Email: "; getline(cin, email);
    cout << "Nhap ten dang nhap: "; getline(cin, user);

    if (KiemTraDocGiaDaDangKy(user)) {
        cout << "Ten dang nhap da ton tai. Vui long chon ten khac\n";
        return;
    }

    cout << "Nhap mat khau: "; getline(cin, pass);

    Reader dg;
    dg.SignUp(hoTen, sdt, email, user, pass);

    NodeReader* newNode = new NodeReader(dg);
    newNode->next = HeadDsDocGia;
    HeadDsDocGia = newNode;

    cout << "Dang ky doc gia thanh cong.\n";

}

bool LibrarySystem::KiemTraDocGiaDaDangKy(const string& tenDangNhap) const {
    NodeReader* current = HeadDsDocGia;
    while (current != nullptr) {
        if (current->data.getUsername() == tenDangNhap)
            return true;
        current = current->next;
    }
    return false;
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

void LibrarySystem::DangKyThuThu() {
    
    string hoTen, sdt, email, user, pass;
    cout << "Nhap ho ten: "; getline(cin, hoTen);
    cout << "Nhap SDT: "; getline(cin, sdt);
    cout << "Nhap Email: "; getline(cin, email);
    cout << "Nhap ten dang nhap: "; getline(cin, user);

    if (KiemTraThuThuDaDangKy(user)) {
        cout << "Ten dang nhap da ton tai. Vui long chon ten khac\n";
        return;
    }

    cout << "Nhap mat khau: "; getline(cin, pass);

    Librarian tt;
    tt.SignUp(hoTen, sdt, email, user, pass);

    NodeLibrarian* newNode = new NodeLibrarian(tt);
    newNode->next = HeadDsTThu;
    HeadDsTThu = newNode;

    cout << "Dang ky thu thu thanh cong.\n";

}

bool LibrarySystem::KiemTraThuThuDaDangKy(const string& tenDangNhap) const {
    NodeLibrarian* current = HeadDsTThu;
    while (current != nullptr) {
        if (current->data.getUsername() == tenDangNhap)
            return true;
        current = current->next;
    }
    return false;
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
    NodeLibrarian *current = HeadDsTThu;
    while (current != nullptr) {
        if (current->data.Login(username, password)) {
            currentUser = &current->data;
            cout << "Thu thu " << username << " da dang nhap thanh cong." << endl;
            return true;
        }
        current = current->next;
    }
    cout << "Dang nhap that bai. Vui long kiem tra lai thong tin." << endl;
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

void LibrarySystem::LuuDuLieu() const {
    // Luu danh sach sach vao file
    ofstream outFile("DanhSachSach.txt");
    if(!outFile) {
        cout << "Khong the mo file DanhSachSach\n";
        return;
    }
    NodeBook *currentBook = HeadDsSach;
    while (currentBook != nullptr) {
        currentBook->data.ghiFile(outFile);
        currentBook = currentBook->next;
    }
    outFile.close();
    cout << "Da luu danh sach sach vao file!\n";

    // Luu danh sach doc gia vao file
    ofstream outReaderFile("DanhSachReader.txt");
    if(!outReaderFile) {
        cout << "Khong the mo file DsReader\n";
        return;
    }
    NodeReader *currentReader = HeadDsDocGia;
    while (currentReader != nullptr) {
        currentReader->data.HienThiThongTin();
        currentReader = currentReader->next;
    }
    outReaderFile.close();
    cout << "Da luu danh sach doc gia vao file!\n";

    // Luu danh sach thu thu vao file
    ofstream outLibrarianFile("DanhSachLibrarian.txt");
    if(!outLibrarianFile) {
        cout << "Khong the mo file DsLibrarian\n";
        return;
    }
    NodeLibrarian *currentLibrarian = HeadDsTThu;
    while (currentLibrarian != nullptr) {
        currentLibrarian->data.HienThiThongTin();
        currentLibrarian = currentLibrarian->next;
    }
    outLibrarianFile.close();
    cout << "Da luu danh sach thu thu vao file!\n";
}




