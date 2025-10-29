#include <iostream>
#include "LibrarySystem.h"
using namespace std;

int main() {
    LibrarySystem system;
    USER* currentUser = nullptr;
    int luaChonChinh;
    system.DocFileHeThong("DanhSachSach.txt");
    system.DocFileDocGia();

    do {
        cout << "\n--- CHAO MUNG DEN HE THONG THU VIEN ---\n";
        cout << "1. Vao voi tu cach DOC GIA\n";
        cout << "2. Vao voi tu cach THU THU\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "Lua chon: ";
        cin >> luaChonChinh;
        cin.ignore();

        if (luaChonChinh == 1) {
            int luaChonDG;
            cout << "\n--- DOC GIA ---\n";
            cout << "1. Dang ky\n";
            cout << "2. Dang nhap\n";
            cout << "Lua chon: ";
            cin >> luaChonDG;
            cin.ignore();

            if (luaChonDG == 1) {
                system.DangKyDocGia();
            } else if (luaChonDG == 2) {
                string username, password;
                cout << "Ten dang nhap: "; getline(cin, username);
                cout << "Mat khau: "; getline(cin, password);
                if (system.DangNhapDocGia(username, password, currentUser)) {
                    int chonDG;
                    do {
                        cout << "\n--- MENU DOC GIA ---\n";
                        cout << "1. Tim sach\n";
                        cout << "2. Muon sach\n";
                        cout << "3. Tra sach\n";
                        cout << "4. Hien thi sach da muon\n";
                        cout << "5. Dang xuat\n";
                        cout << "Lua chon: ";
                        cin >> chonDG;
                        cin.ignore();

                        Reader* docGia = dynamic_cast<Reader*>(currentUser);
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
            }
        } else if (luaChonChinh == 2) {
            string username, password;
            cout << "\n--- THU THU ---\n";
            cout << "Ten dang nhap: "; getline(cin, username);
            cout << "Mat khau: "; getline(cin, password);
            if (system.DangNhapThuThu(username, password, currentUser)) {
                int chonTT;
                do {
                    cout << "\n--- MENU THU THU ---\n";
                    cout << "1. Them sach\n";
                    cout << "2. Xoa sach\n";
                    cout << "3. Hien thi danh sach sach\n";
                    cout << "4. Tim sach\n";
                    cout << "5. Dang xuat\n";
                    cout << "Lua chon: ";
                    cin >> chonTT;
                    cin.ignore();

                    Librarian* tt = dynamic_cast<Librarian*>(currentUser);
                    switch (chonTT) {
                        case 1: {
                            system.DocFileSach("ThemSach.txt");
                            system.GhiFileSach("DanhSachSach.txt");
                            break;
                        }
                        case 2: {
                            string ma;
                            cout << "Nhap ma sach can xoa: ";
                            getline(cin, ma);
                            system.XoaSach(ma);
                            break;
                        }
                        case 3: system.HienThiDanhSachSach(); break;
                        case 4: system.TimSach(); break;
                        case 5: system.DangXuat(currentUser); break;
                        default: cout << "Lua chon khong hop le!\n";
                    }
                } while (currentUser != nullptr);
            }
        }

    } while (luaChonChinh != 0);

    cout << "Tam biet!\n";
    return 0;
}