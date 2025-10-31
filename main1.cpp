#include <iostream>
#include <limits>
#include "LibrarySystem.h"
using namespace std;

void MenuDocGia(LibrarySystem& system, USER*& currentUser);
void MenuThuThu(LibrarySystem& system, USER*& currentUser);

int main() {
    LibrarySystem system;
    USER* currentUser = nullptr;

    cout << "==============================\n";
    cout << "   HE THONG QUAN LY THU VIEN  \n";
    cout << "==============================\n";

    system.DocFileHeThong("DanhSachSach.txt");
    system.DocFileDocGia();

    int luaChonChinh;
    do {
        cout << "\n========= MENU CHINH =========\n";
        cout << "1. Dang nhap / Dang ky (Doc gia)\n";
        cout << "2. Dang nhap (Thu thu)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "==============================\n";
        cout << "Lua chon: ";
        cin >> luaChonChinh;
        cin.ignore();

        switch (luaChonChinh) {
            case 1: {
                int luaChonDG;
                cout << "\n--- DOC GIA ---\n";
                cout << "1. Dang ky tai khoan moi\n";
                cout << "2. Dang nhap\n";
                cout << "Lua chon: ";
                cin >> luaChonDG;
                cin.ignore();

                if (luaChonDG == 1) {
                    system.DangKyDocGia();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();  // đợi người dùng nhấn Enter
                    continue;
                } 
                else if (luaChonDG == 2) {
                    string username, password;
                    cout << "Ten dang nhap: "; getline(cin, username);
                    cout << "Mat khau: "; getline(cin, password);
                    if (system.DangNhapDocGia(username, password, currentUser)) {
                        MenuDocGia(system, currentUser);
                    } else {
                        cout << "Sai tai khoan hoac mat khau!\n";
                    }
                }
                break;
            }

            case 2: {
                string username, password;
                cout << "\n--- THU THU ---\n";
                cout << "Ten dang nhap: "; getline(cin, username);
                cout << "Mat khau: "; getline(cin, password);
                if (system.DangNhapThuThu(username, password, currentUser)) {
                    MenuThuThu(system, currentUser);
                } else {
                    cout << "Sai tai khoan hoac mat khau!\n";
                }
                break;
            }

            case 0: {
                cout << "Thoat chuong trinh!" << endl;
                break;
            }

            default:
                cout << "Lua chon khong hop le!\n";
        }

    } while (luaChonChinh != 0);

    return 0;
}

// ----------------- MENU DOC GIA -----------------
void MenuDocGia(LibrarySystem& system, USER*& currentUser) {
    Reader* docGia = dynamic_cast<Reader*>(currentUser);
    int chonDG;

    do {
        cout << "\n========= MENU DOC GIA =========\n";
        cout << "1. Tim sach\n";
        cout << "2. Muon sach\n";
        cout << "3. Tra sach\n";
        cout << "4. Hien thi sach da muon\n";
        cout << "5. Dang xuat\n";
        cout << "================================\n";
        cout << "Lua chon: ";
        cin >> chonDG;
        cin.ignore();

        switch (chonDG) {
            case 1: system.TimSach(); break;
            case 2: {
                string ma;
                cout << "Nhap ma sach muon: ";
                getline(cin, ma);
                system.MuonSach(docGia, ma);
                break;
            }
            case 3: {
                string ma;
                cout << "Nhap ma sach tra: ";
                getline(cin, ma);
                system.TraSach(docGia, ma);
                break;
            }
            case 4: docGia->HienThiSachDaMuon(); break;
            case 5: system.DangXuat(currentUser); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (currentUser != nullptr);
}

// ----------------- MENU THU THU -----------------
void MenuThuThu(LibrarySystem& system, USER*& currentUser) {
    Librarian* tt = dynamic_cast<Librarian*>(currentUser);
    int chonTT;

    do {
        cout << "\n========= MENU THU THU =========\n";
        cout << "1. Them sach tu file (ThemSach.txt)\n";
        cout << "2. Xoa sach\n";
        cout << "3. Hien thi danh sach sach\n";
        cout << "4. Tim sach\n";
        cout << "5. Cap nhat thong tin sach\n";
        cout << "6. Dang xuat\n";
        cout << "=================================\n";
        cout << "Lua chon: ";
        cin >> chonTT;
        cin.ignore();

        switch (chonTT) {
            case 1:
                system.DocFileSach("ThemSach.txt");
                system.GhiFileSach("DanhSachSach.txt");
                break;
            case 2: {
                string ma;
                cout << "Nhap ma sach can xoa: ";
                getline(cin, ma);
                system.XoaSach(ma);
                break;
            }
            case 3: system.HienThiDanhSachSach(); break;
            case 4: system.TimSach(); break;
            case 5: system.CapNhatThongTinSach(); break;
            case 6: system.DangXuat(currentUser); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (currentUser != nullptr);
}
