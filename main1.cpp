#include <iostream>
#include <limits>
#include "LibrarySystem.h"
using namespace std;

void MenuDocGia(LibrarySystem& system, USER*& currentUser);
void MenuThuThu(LibrarySystem& system, USER*& currentUser);

int main() {
    LibrarySystem system;
    Librarian *tt;
    Reader *docGia;
    USER* currentUser = nullptr;

    cout << "==============================\n";
    cout << "   HE THONG QUAN LY THU VIEN  \n";
    cout << "==============================\n";

    system.DocFileHeThong("DanhSachSach.txt");
    system.DocFileDocGia();
    system.BuildHashTable();

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
                
     int luaChonDG = -1;
    while (luaChonDG != 0) {
        cout << "\n--- MENU DOC GIA ---\n";
        cout << "1. Dang ky tai khoan moi\n";
        cout << "2. Dang nhap\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "Lua chon: ";
        cin >> luaChonDG;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (luaChonDG) {
            case 1:
                system.DangKyDocGia();
                break;
            case 2:
                if (system.DangNhapDocGia(currentUser)) {
                    MenuDocGia(system, currentUser);
                } else {
                    cout << "Sai tai khoan hoac mat khau!\n";
                }
                break;

            case 0:
                cout << "Quay lai menu chinh...\n";
                break;

            default:
                cout << "Lua chon khong hop le!\n";
        }
    }
    break;

            }

            case 2: {
                string username, password;
                cout << "\n--- THU THU ---\n";
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
        cout << "5. Hien thi danh sach sach\n";
        cout << "6. Hien thi lich su muon tra\n";
        cout << "7. Xem top 10 sach duoc danh gia cao nhat\n";
        cout << "8. Dang xuat\n";
        cout << "================================\n";
        cout << "Lua chon: ";
        cin >> chonDG;
        cin.ignore();

        switch (chonDG) {
            case 1: {
                string keyword;
                cout << "Nhap tu khoa tim kiem: ";
                getline(cin, keyword);
                system.TimSach(keyword); 
                break;
            }
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
            case 4: docGia->HienThiSachDangMuon(); break;
            case 5: system.HienThiDanhSachSach(); break;
            case 6: docGia->HienThiLichSuMuonTra(); break;
            case 7: system.XepHangSach(); break;
            case 8: system.DangXuat(currentUser); break;
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
        cout << "6. Hien thi danh sach doc gia\n";
        cout << "7. Xem top 10 sach duoc danh gia cao nhat\n";
        cout << "8. Dang xuat\n";
        cout << "=================================\n";
        cout << "Lua chon: ";
        cin >> chonTT;
        cin.ignore();

        switch (chonTT) {
            case 1:
                tt->ThemSach(system);
                break;
            case 2: {
                tt->XoaSach(system);
                break;
            }
            case 3: system.HienThiDanhSachSach(); break;
            case 4: {
                string keyword;
                cout << "Nhap tu khoa tim kiem: ";
                getline(cin, keyword);
                system.TimSach(keyword); 
                break;
            }
            case 5: tt->CapNhatThongTinSach(system); break;
            case 6: system.HienThiTatCaDocGia(); break;
            case 7: system.XepHangSach(); break;
            case 8: system.DangXuat(currentUser); break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (currentUser != nullptr);
}
