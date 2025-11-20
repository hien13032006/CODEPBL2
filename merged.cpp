/*/#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <iostream>
#include <sstream>
#include "Node.h"
using namespace std;

class Sach {
protected:
    string maSach;
    string tenSach;
    string tacGia;
    string theLoai;
    int namXuatBan;
    string nhaXuatBan;
    int soLuong;
    string imagePath; // Đường dẫn đến ảnh bìa sách
    double tongDiemDanhGia = 0;
    int soLuotDanhGia = 0;

public:
    Sach() : namXuatBan(0), soLuong(0) {}
    Sach(string ten, string tg, string tl, int nam, string nxb, int sl = 0)
        : tenSach(ten), tacGia(tg), theLoai(tl), namXuatBan(nam), nhaXuatBan(nxb), soLuong(sl) {}

    virtual ~Sach() = default;

    // Các phương thức ảo
    virtual string prefix() const = 0;     // Mã loại, ví dụ: "GT", "TT"
    virtual Sach* clone() const = 0;       // Tạo bản sao (dùng khi thêm vào danh sách)
    virtual string toCSV() const;          // Xuất ra file
    virtual void hienThiThongTin() const;

    bool muonSach() {
        if (soLuong > 0) {
            soLuong--;
            return true;
        }
        return false; // hết sách
    }
    void traSach() {
        soLuong++;
    }

    void themDanhGia(int diem) {
        tongDiemDanhGia += diem;
        soLuotDanhGia++;
    }

    // Getter / Setter
    string getMaSach() const { return maSach; }
    string getTenSach() const { return tenSach; }
    string getTacGia() const { return tacGia; }
    string getTheLoai() const { return theLoai; }
    int getNamXuatBan() const { return namXuatBan; }
    string getNhaXuatBan() const { return nhaXuatBan; }
    int getSoLuong() const { return soLuong; }
    double getTongDiem() const { return tongDiemDanhGia; }
    int getSoDanhGia() const { return soLuotDanhGia; }
    double getDiemTrungBinh() const {
        return soLuotDanhGia == 0 ? 0 : tongDiemDanhGia / soLuotDanhGia;
    }

    void setMaSach(string ma) { maSach = ma; }
    void setTenSach(string ten) { tenSach = ten; }
    void setTacGia(string tg) { tacGia = tg; }
    void setTheLoai(string tl) { theLoai = tl; }
    void setNamXuatBan(int nxb) { namXuatBan = nxb; }
    void setNhaXuatBan(string nhxb) { nhaXuatBan = nhxb; }
    void setSoLuong(int sl) { soLuong = sl; }


    void setDanhGia(double tong, int so) {
        tongDiemDanhGia = tong;
        soLuotDanhGia = so;
    }

    // Factory method: tạo lớp con phù hợp từ dữ liệu file
    static Sach* createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb);
    static void docFileInput(const string& fileName, NodeBook*& head);
    static void ghiFile(const string& fileName, NodeBook* head);

};

class GiaoTrinh : public Sach {
public:
    GiaoTrinh() {}
    GiaoTrinh(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}

    string prefix() const override { return "GT"; }
    Sach* clone() const override { return new GiaoTrinh(*this); }
};

class ThamKhao : public Sach {
public:
    ThamKhao() {}
    ThamKhao(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}

    string prefix() const override { return "TK"; }
    Sach* clone() const override { return new ThamKhao(*this); }
};

class TieuThuyet : public Sach {
public:
    TieuThuyet() {}
    TieuThuyet(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}

    string prefix() const override { return "TT"; }
    Sach* clone() const override { return new TieuThuyet(*this); }
};

class TruyenNgan : public Sach {
    public:
        TruyenNgan(string ten, string tg, string tl, int nam, string nxb)
            : Sach(ten, tg, tl, nam, nxb) {}
        string prefix() const override { return "TN";}
        Sach* clone() const override { return new TruyenNgan(*this); }
};

class TapChi : public Sach {
    public:
        TapChi(string ten, string tg, string tl, int nam, string nxb)
            : Sach(ten, tg, tl, nam, nxb) {}
        string prefix() const override { return "TC";}
        Sach* clone() const override { return new TapChi(*this); }
};

class TruyenTranh : public Sach {
    public:
        TruyenTranh(string ten, string tg, string tl, int nam, string nxb)
            : Sach(ten, tg, tl, nam, nxb) {}
        string prefix() const override { return "TTR";}
        Sach* clone() const override { return new TruyenTranh(*this); }
};

class SachKiNang : public Sach {
    public:
        SachKiNang(string ten, string tg, string tl, int nam, string nxb)
            : Sach(ten, tg, tl, nam, nxb) {}
        string prefix() const override { return "KN";}
        Sach* clone() const override { return new SachKiNang(*this); }
};

#endif
#ifndef Librarian_h
#define Librarian_h
#include "USER.h"

class LibrarySystem;

class Librarian : public USER {
    private:
        string chucVu;
    public:
        Librarian();
        Librarian(string maID, string hoTen, string SDT, string Email, string username, string password, string chucVu);

        void SetThongTin(string maID, string hoTen, string SDT, string Email, string username, string password);
        void HienThiThongTin() const override;

        void ThemSach(LibrarySystem &L);
        void XoaSach(LibrarySystem &L);
        void CapNhatThongTinSach(LibrarySystem &L);

        void XemThongKe(LibrarySystem &L);
        void QuanLyDocGia(LibrarySystem &L);
};
#endif 
#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "Book.h"
#include "USER.h"
#include "Librarian.h"
#include "Reader.h"
#include "Node.h"

const int TABLE_SIZE = 1000;

struct HashNode {
    string key;        // từ khóa
    NodeBook* list;    // danh sách các sách có chứa từ khóa
    HashNode* next;
};


class LibrarySystem {
    private:
        NodeBook *HeadDsSach; //con tro den dau danh sach sach
        NodeReader *HeadDsDocGia; //con tro den dau danh sach doc gia
        NodeLibrarian *HeadDsTThu; //con tro den dau danh sach thu thu
        HashNode* hashTable[TABLE_SIZE];

    public:
        LibrarySystem();
        ~LibrarySystem();

        int hashFunction(const string& s);

        void DocFileSach(const string& fileName);//them sach tu file
        void GhiFileSach(const string& fileName) const;//luu thong tin sach vua them vao file sau khi cap maSach
        void GhiFileHeThong(const string& fileName) const;
        void DocFileHeThong(const string& fileName);
        void DocFileDocGia();
        void DocDanhSachMuonCuaDocGia(Reader* docGia);
        void GhiDanhSachMuonCuaDocGia(Reader* docGia);

        void XoaSach(const string &maSach);
        void CapNhatThongTinSach();
        void TimSach(const string& keyword);
        void BuildHashTable();
        void MuonSach(Reader* docGia, const string& maSach);
        void TraSach(Reader* docGia, const string& maSach);

        void DanhGiaSach(Reader* docGia, Sach* sach);
        double TinhDiemTrungBinhTuFile(const string& tenSach,const string& tacGia,int namXB,const string& nhaXB);
        void HienThiDanhSachSach() ;
        void HienThiDocGiaQuaHan();
        void ThongKeSachQuaHan();

        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        void DangKyDocGia();
        bool DangNhapDocGia(USER* &currentUser);
        bool DangNhapThuThu(const string &usernameInput, const string &passwordInput, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void HienThiTatCaDocGia() const;
        void XepHangSach();

        NodeBook* getDanhSachSach() { return HeadDsSach; }
        NodeReader* getDanhSachDocGia() { return HeadDsDocGia; }

        void DocTatCaDanhSachMuon();
        void XemThongKe();

};
        
#endif
#ifndef LICHSUMUONTRA_H
#define LICHSUMUONTRA_H

#include <ctime>
#include <string>
#include <iostream>
using namespace std;

struct LichSuMuonTra {
    string hanhDong; //muon hoac tra 
    string maSach;
    string tenSach;
    string thoiGian;
    string maDocGia;

    string toCSV() const {
        return hanhDong + "|" + maDocGia + "|" + maSach + "|" + tenSach + "|" + thoiGian;
}

};

struct NodeLichSu {
    LichSuMuonTra data;
    NodeLichSu* next;
};


class StackLichSu {
    private:
        NodeLichSu* top;
    public:
        StackLichSu() : top(nullptr) {}
        ~StackLichSu();
        void push(const LichSuMuonTra& ls);
        bool isEmpty() const;
        void hienThiLichSu() const;
};

#endif
#ifndef Node_h
#define Node_h
#include "Librarian.h"

class Sach;
class Reader;

struct NodeBook {
    Sach* data;
    NodeBook *next;

    NodeBook(Sach *s) {
        data = s;
        next = nullptr;
    }

};

struct NodeReader {
    Reader* data;
    NodeReader* next;

    NodeReader(Reader* r) {
        data = r;
        next = nullptr;
    }
};


struct NodeLibrarian {
    Librarian data;
    NodeLibrarian *next;

    NodeLibrarian(const Librarian& l) {
        data = l;
        next = nullptr;
    }
};

struct PhieuMuonSach {
    Sach* sach;
    time_t ngayMuon;      // Thời gian mượn
    time_t ngayHetHan;    // Thời gian hết hạn (3 tháng sau ngày mượn)
    
    PhieuMuonSach(Sach* s) : sach(s) {
        ngayMuon = time(nullptr);  // Lấy thời gian hiện tại
        
        // Tính ngày hết hạn: 3 tháng = 90 ngày
        ngayHetHan = ngayMuon + (90 * 24 * 60 * 60);
    }

    // Constructor khi đọc từ file (có ngày mượn sẵn)
    PhieuMuonSach(Sach* s, time_t ngayM, time_t ngayHH) 
        : sach(s), ngayMuon(ngayM), ngayHetHan(ngayHH) {}
    
    // Tính số ngày còn lại
    int soNgayConLai() const {
        time_t now = time(nullptr);
        double seconds = difftime(ngayHetHan, now);
        return (int)(seconds / (24 * 60 * 60));
    }
    
    // Kiểm tra đã quá hạn chưa
    bool daQuaHan() const {
        return soNgayConLai() < 0;
    }
    
    // Lấy chuỗi trạng thái
    string trangThaiHan() const {
        if (daQuaHan()) {
            return "QUA HAN";
        } else {
            int ngay = soNgayConLai();
            return to_string(ngay) + " ngay";
        }
    }
};

struct NodeMuonSach {
    PhieuMuonSach* phieu;
    NodeMuonSach* next;
    
    NodeMuonSach(PhieuMuonSach* p) : phieu(p), next(nullptr) {}
};


#endif
#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "LichSuMuonTra.h"
#include "Node.h"
#include <string>
#include <ctime>

class Sach;

class Reader : public USER {
    private:
        time_t ngayDangKy;
        int gioiHanSachMuon;
        static int readerCount;
        NodeMuonSach *HeadDsMuonSach; //con tro den dau danh sach sach da muon
        StackLichSu LichSu;
        string fileLichSu;

    public:
        Reader();
        Reader(string ma, string hoTen, string sdt, string email, string username, string password);
        ~Reader();

        void HienThiThongTin() const override;
        UserRole getVaiTro() const override { return UserRole::READER; }

        void SignUp(string hoTen, string SDT, string Email, string username, string password);

        bool coTheMuonSach() const;
        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        int getSoSachConCoTheMuon() const;
        
        void themPhieuMuonSach(const Sach* s);  //Tạo phiếu mượn
        void themPhieuMuonSachTuFile(const Sach* s, time_t ngayMuon, time_t ngayHetHan);
        void xoaPhieuMuonSach(const string& maSach); //Xóa phiếu mượn khi trả sách

        void HienThiSachDangMuon() const;
        int DemSachQuaHan() const; 

        void docFileSachDangMuonTuFile();
        void ghiLichSu(const string& hanhDong, const Sach* s);
        void HienThiLichSuMuonTra() const;

        string toCSV() const;
        
        time_t getNgayDangKy() const { return ngayDangKy; }
        int getGioiHanMuon() const { return gioiHanSachMuon; }
        string getFileLichSu() const { return fileLichSu; }
        NodeMuonSach* getDanhSachPhieuMuon() const { return HeadDsMuonSach; } 

        void setDanhSachPhieuMuon(NodeMuonSach* head) {  HeadDsMuonSach = head;}
        void setGioiHanMuon(int limit) { gioiHanSachMuon = limit; }
        static void setReaderCount(int n) { readerCount = n; }
        static int getReaderCount() { return readerCount; }
};
#endif#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

enum class UserRole {
    NONE,
    READER,
    LIBRARIAN   
};

class USER {
    protected:
        string maID;
        string hoTen;
        string SDT;
        string Email;
        string username;
        string password;
        UserRole vaiTro;
        bool trangThaiHoatDong;
    public:
        USER();
        USER(string maID, string hoTen, string SDT, string Email, string username, string password);
        virtual ~USER();

        virtual void HienThiThongTin() const;
        virtual void CapNhatThongTin();
        virtual UserRole getVaiTro() const { return vaiTro; }

        bool isLibrarian() const { return vaiTro == UserRole::LIBRARIAN; }
        bool isReader() const { return vaiTro == UserRole::READER; }

        bool Login(string username, string password);
        void Logout();

        string getMaID() const { return maID; }
        string getHoTen() const { return hoTen; }
        string getUsername() const { return username; }
        string getEmail() const { return Email; }
        string getSDT() const { return SDT; }
        string getPassword() const { return password; }
        string getTenVaiTro() const {
            return (vaiTro == UserRole::LIBRARIAN) ? "LIBRARIAN" : "READER";
    }

        void setMaID(string newMaID) { maID = newMaID; }
        void setHoTen(string newHoTen) { hoTen = newHoTen; }
        void setSDT(string newSDT)  { SDT = newSDT; }
        void setEmail(string newEmail) { Email = newEmail; }
        void setUsername(string newUsername) { username = newUsername; }
        void setPassword(string newPassword) { password = newPassword; }
        void setTrangThai(bool active) { trangThaiHoatDong = active; }
};
#endif// ============================================
// app/App.hpp - FIXED VERSION
// ============================================
#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "State.hpp"
#include "Modal.hpp"
#include "SplashScreen.hpp"
#include "HomeScreen.hpp"
#include "CategoryScreen.hpp"
#include "SearchScreen.hpp"
#include "BookDetailScreen.hpp"
#include "LoginChoiceScreen.hpp"
#include "LoginLibrarianScreen.hpp"
#include "LoginReaderScreen.hpp"
#include "RegisterReaderScreen.hpp"
#include "BorrowedBooksScreen.hpp"
#include "HistoryScreen.hpp"
#include "ManageBooksScreen.hpp"
#include "ManageReadersScreen.hpp"
#include "StatisticsScreen.hpp"
#include "OverdueReadersScreen.hpp"
#include "LibrarySystem.h"
#include "Reader.h"
#include "Librarian.h"

class App {
private:
    sf::RenderWindow window;
    AppState currentState;
    UserRole currentUserRole;
    sf::Font mainFont;
    
    LibrarySystem* libSystem;
    USER* currentUser;
    Reader* currentReader;
    Librarian* currentLibrarian;
    
    // Screens
    SplashScreen* splashScreen;
    HomeScreen* homeScreen;
    CategoryScreen* categoryScreen;
    SearchScreen* searchScreen;
    BookDetailScreen* bookDetailScreen;
    LoginChoiceScreen* loginChoiceScreen;
    LoginLibrarianScreen* loginLibrarianScreen;
    LoginReaderScreen* loginReaderScreen;
    RegisterReaderScreen* registerReaderScreen;
    BorrowedBooksScreen* borrowedBooksScreen;
    HistoryScreen* historyScreen;
    ManageBooksScreen* manageBooksScreen;
    ManageReadersScreen* manageReadersScreen;
    StatisticsScreen* statisticsScreen;
    OverdueReadersScreen* overdueReadersScreen;
    
    Modal* modal;
    Modal* detailModal;

public:
    App() : window(sf::VideoMode(1400, 900), "Quan Ly Thu Vien") {
        window.setFramerateLimit(60);
        currentState = AppState::SPLASH;
        currentUserRole = UserRole::NONE;
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        
        std::cout << "Khoi tao ung dung...\n";
        
        initResources();
        initLibrarySystem();
        initScreens();
        
        std::cout << "Khoi tao thanh cong!\n";
    }

    ~App() {
        delete splashScreen;
        delete homeScreen;
        delete categoryScreen;
        delete searchScreen;
        delete bookDetailScreen;
        delete loginChoiceScreen;
        delete loginLibrarianScreen;
        delete loginReaderScreen;
        delete registerReaderScreen;
        delete borrowedBooksScreen;
        delete historyScreen;
        delete manageBooksScreen;
        delete manageReadersScreen;
        delete statisticsScreen;
        delete overdueReadersScreen;
        delete modal;
        delete detailModal;
        delete libSystem;
    }

    void initResources() {
        if (!mainFont.loadFromFile("NotoSerif-Regular.ttf")) {
            std::cerr << "Loi: Khong the load font!" << std::endl;
        }
    }

    void initLibrarySystem() {
        libSystem = new LibrarySystem();
        
        std::cout << "====================================\n";
        std::cout << "   HE THONG QUAN LY THU VIEN\n";
        std::cout << "====================================\n";
        
        libSystem->DocFileHeThong("DanhSachSach.txt");
        libSystem->DocFileDocGia();
        libSystem->DocTatCaDanhSachMuon();
        libSystem->BuildHashTable();
        
        std::cout << "Da nap du lieu tu file thanh cong!\n";
        std::cout << "====================================\n";
    }

    void initScreens() {
        splashScreen = new SplashScreen(mainFont);
        homeScreen = new HomeScreen(mainFont, libSystem);
        categoryScreen = new CategoryScreen(mainFont, libSystem);
        searchScreen = new SearchScreen(mainFont, libSystem);
        
        modal = new Modal(mainFont);
        detailModal = new Modal(mainFont);
        
        bookDetailScreen = new BookDetailScreen(mainFont, detailModal);
        loginChoiceScreen = new LoginChoiceScreen(mainFont, modal);
        loginLibrarianScreen = new LoginLibrarianScreen(mainFont, modal);
        loginReaderScreen = new LoginReaderScreen(mainFont, modal);
        registerReaderScreen = new RegisterReaderScreen(mainFont, modal);
        
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, nullptr);
        historyScreen = new HistoryScreen(mainFont, nullptr);
        
        manageBooksScreen = new ManageBooksScreen(mainFont, libSystem);
        manageReadersScreen = new ManageReadersScreen(mainFont, libSystem);
        statisticsScreen = new StatisticsScreen(mainFont, libSystem);
        overdueReadersScreen = new OverdueReadersScreen(mainFont, libSystem);
    }

    void changeState(AppState newState) {
        currentState = newState;
    }

    Sach* findBookById(const std::string& bookId) {
        if (!libSystem) return nullptr;
        
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            if (current->data->getMaSach() == bookId) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    // FIX: Xử lý navigation sidebar đúng
    void handleSidebarNavigation(int menuId, Sidebar* sidebar) {
        std::string menuLabel = sidebar->getMenuLabel(menuId);
        
        std::cout << "Menu clicked: " << menuLabel << std::endl;
        
        if (menuLabel == "Home") {
            changeState(AppState::HOME);
        } 
        else if (menuLabel == "Top 10 sach") {
            // Scroll về đầu trang để thấy top 10
            changeState(AppState::HOME);
        }
        else if (menuLabel == "Tat ca sach") {
            // Scroll xuống phần tất cả sách
            changeState(AppState::HOME);
        }
        else if (menuLabel == "The loai") {
            changeState(AppState::CATEGORY);
        } 
        else if (menuLabel == "Tim kiem") {
            changeState(AppState::SEARCH);
        } 
        else if (menuLabel == "Thanh vien" && currentUserRole == UserRole::NONE) {
            modal->show();
            changeState(AppState::LOGIN_CHOICE);
        } 
        else if (menuLabel == "Sach dang muon" && currentUserRole == UserRole::READER) {
            if (currentReader) {
                libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                delete borrowedBooksScreen;
                borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
                borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
            }
            changeState(AppState::BORROWED_BOOKS);
        } 
        else if (menuLabel == "Lich su" && currentUserRole == UserRole::READER) {
            changeState(AppState::HISTORY);
        } 
        else if (menuLabel == "Quan ly sach" && currentUserRole == UserRole::LIBRARIAN) {
            changeState(AppState::MANAGE_BOOKS);
        } 
        else if (menuLabel == "Quan ly doc gia" && currentUserRole == UserRole::LIBRARIAN) {
            manageReadersScreen->loadReaders(mainFont);
            changeState(AppState::MANAGE_READERS);
        } 
        else if (menuLabel == "Doc gia qua han" && currentUserRole == UserRole::LIBRARIAN) {
            overdueReadersScreen->loadOverdueReaders(mainFont);
            changeState(AppState::OVERDUE_READERS);
        } 
        else if (menuLabel == "Thong ke" && currentUserRole == UserRole::LIBRARIAN) {
            statisticsScreen->loadStatistics();
            changeState(AppState::STATISTICS);
        } 
        else if (menuLabel == "Dang xuat") {
            handleLogout();
        }
    }

    void handleLibrarianLogin() {
        std::string username = loginLibrarianScreen->getUsername();
        std::string password = loginLibrarianScreen->getPassword();
        
        if (username.empty() || password.empty()) {
            std::cout << "Loi: Vui long nhap username va password!" << std::endl;
            return;
        }
        
        USER* tempUser = nullptr;
        if (libSystem->DangNhapThuThu(username, password, tempUser)) {
            currentUser = tempUser;
            currentLibrarian = dynamic_cast<Librarian*>(tempUser);
            currentUserRole = UserRole::LIBRARIAN;
            
            homeScreen->setUserRole(currentUserRole, mainFont);
            categoryScreen->setUserRole(currentUserRole, mainFont);
            searchScreen->setUserRole(currentUserRole, mainFont);
            manageBooksScreen->setUserRole(currentUserRole, mainFont);
            manageReadersScreen->setUserRole(currentUserRole, mainFont);
            statisticsScreen->setUserRole(currentUserRole, mainFont);
            overdueReadersScreen->setUserRole(currentUserRole, mainFont);
            
            std::cout << "Dang nhap thanh cong! Chao mung Thu Thu: " 
                      << currentLibrarian->getHoTen() << std::endl;
            
            loginLibrarianScreen->clearFields();
            modal->hide();
            changeState(AppState::HOME);
        } else {
            std::cout << "Loi: Sai username hoac password!" << std::endl;
        }
    }

    void handleReaderLogin() {
        std::string username = loginReaderScreen->getUsername();
        std::string password = loginReaderScreen->getPassword();
        
        if (username.empty() || password.empty()) {
            std::cout << "Loi: Vui long nhap username va password!" << std::endl;
            return;
        }
        
        NodeReader* current = libSystem->getDanhSachDocGia();
        bool found = false;
        
        while (current != nullptr) {
            if (current->data->getUsername() == username && 
                current->data->getPassword() == password) {
                currentUser = current->data;
                currentReader = current->data;
                currentUserRole = UserRole::READER;
                found = true;
                
                libSystem->DocDanhSachMuonCuaDocGia(currentReader);
                
                homeScreen->setUserRole(currentUserRole, mainFont);
                categoryScreen->setUserRole(currentUserRole, mainFont);
                searchScreen->setUserRole(currentUserRole, mainFont);
                
                delete borrowedBooksScreen;
                borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
                borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
                
                delete historyScreen;
                historyScreen = new HistoryScreen(mainFont, currentReader);
                historyScreen->setUserRole(currentUserRole, mainFont);
                
                std::cout << "Dang nhap thanh cong! Chao mung Doc Gia: " 
                          << currentReader->getHoTen() << std::endl;
                
                loginReaderScreen->clearFields();
                modal->hide();
                changeState(AppState::HOME);
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            std::cout << "Loi: Sai username hoac password!" << std::endl;
        }
    }

    void handleReaderRegister() {
        std::string name = registerReaderScreen->getName();
        std::string phone = registerReaderScreen->getPhone();
        std::string email = registerReaderScreen->getEmail();
        std::string username = registerReaderScreen->getUsername();
        std::string password = registerReaderScreen->getPassword();
        std::string confirmPass = registerReaderScreen->getConfirmPassword();
        
        if (name.empty() || phone.empty() || email.empty() || 
            username.empty() || password.empty()) {
            std::cout << "Loi: Vui long dien day du thong tin!" << std::endl;
            return;
        }
        
        if (password != confirmPass) {
            std::cout << "Loi: Mat khau khong khop!" << std::endl;
            return;
        }
        
        if (password.length() != 8) {
            std::cout << "Loi: Mat khau phai co dung 8 ki tu!" << std::endl;
            return;
        }
        
        if (libSystem->KiemTraDocGiaDaDangKy(username)) {
            std::cout << "Loi: Username da ton tai!" << std::endl;
            return;
        }
        
        Reader* newReader = new Reader();
        newReader->SignUp(name, phone, email, username, password);
        
        std::ofstream out("DocGia.txt", std::ios::app);
        if (out.is_open()) {
            out << newReader->toCSV() << std::endl;
            out.close();
        }
        
        libSystem->DocFileDocGia();
        
        std::cout << "Dang ky thanh cong! Ma doc gia: " << newReader->getMaID() << std::endl;
        
        registerReaderScreen->clearFields();
        changeState(AppState::LOGIN_READER);
    }

    void handleLogout() {
        if (currentUser) {
            std::cout << "Da dang xuat: " << currentUser->getHoTen() << std::endl;
        }
        
        currentUser = nullptr;
        currentReader = nullptr;
        currentLibrarian = nullptr;
        currentUserRole = UserRole::NONE;
        
        homeScreen->setUserRole(currentUserRole, mainFont);
        categoryScreen->setUserRole(currentUserRole, mainFont);
        searchScreen->setUserRole(currentUserRole, mainFont);
        
        changeState(AppState::HOME);
    }

    void handleBorrowBook() {
        if (!currentReader || !bookDetailScreen->getCurrentBook()) return;
        
        Sach* book = bookDetailScreen->getCurrentBook();
        std::string maSach = book->getMaSach();
        
        if (book->getSoLuong() == 0) {
            std::cout << "Loi: Sach da het!" << std::endl;
            return;
        }
        
        if (currentReader->DaMuonSach(maSach)) {
            std::cout << "Loi: Ban da muon sach nay roi!" << std::endl;
            return;
        }
        
        if (currentReader->DemSachDaMuon() >= currentReader->getGioiHanMuon()) {
            std::cout << "Loi: Ban da muon toi da sach!" << std::endl;
            return;
        }
        
        int soQuaHan = currentReader->DemSachQuaHan();
        if (soQuaHan > 0) {
            std::cout << "Loi: Ban co sach qua han!" << std::endl;
            return;
        }
        
        libSystem->MuonSach(currentReader, maSach);
        
        delete borrowedBooksScreen;
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
        borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        homeScreen->loadBooks(mainFont);
        
        std::cout << "Muon sach thanh cong!" << std::endl;
        
        detailModal->hide();
        changeState(AppState::HOME);
    }

    void handleReturnBook(const std::string& bookId) {
        if (!currentReader) return;
        
        NodeMuonSach* phieuNode = currentReader->getDanhSachPhieuMuon();
        bool quaHan = false;
        int soNgayQuaHan = 0;
        std::string tenSach = "";
        
        while (phieuNode != nullptr) {
            if (phieuNode->phieu->sach->getMaSach() == bookId) {
                tenSach = phieuNode->phieu->sach->getTenSach();
                if (phieuNode->phieu->daQuaHan()) {
                    quaHan = true;
                    soNgayQuaHan = abs(phieuNode->phieu->soNgayConLai());
                }
                break;
            }
            phieuNode = phieuNode->next;
        }
        
        if (quaHan) {
            std::cout << "CANH BAO: Sach qua han " << soNgayQuaHan << " ngay!" << std::endl;
        }
        
        libSystem->TraSach(currentReader, bookId);
        
        delete borrowedBooksScreen;
        borrowedBooksScreen = new BorrowedBooksScreen(mainFont, currentReader);
        borrowedBooksScreen->setUserRole(currentUserRole, mainFont);
        homeScreen->loadBooks(mainFont);
        
        std::cout << "Da tra sach: " << tenSach << std::endl;
    }

    void handleDeleteBook() {
        if (!currentLibrarian || !bookDetailScreen->getCurrentBook()) return;
        
        Sach* book = bookDetailScreen->getCurrentBook();
        std::string maSach = book->getMaSach();
        std::string tenSach = book->getTenSach();
        
        libSystem->XoaSach(maSach);
        homeScreen->loadBooks(mainFont);
        categoryScreen->loadCategories(mainFont);
        
        std::cout << "Da xoa sach: " << tenSach << std::endl;
        
        detailModal->hide();
        changeState(AppState::HOME);
    }

    void handleUpdateBook() {
        if (!currentLibrarian) return;
        
        libSystem->CapNhatThongTinSach();
        homeScreen->loadBooks(mainFont);
        
        std::cout << "Da cap nhat thong tin sach!" << std::endl;
        
        detailModal->hide();
        changeState(AppState::HOME);
    }

    void handleEvents() {
        sf::Event event;
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            switch (currentState) {
                case AppState::SPLASH:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (splashScreen->handleClick(mousePos)) {
                            changeState(AppState::HOME);
                        }
                    }
                    break;

                case AppState::HOME:
                    homeScreen->handleSearchEvent(event, mousePos);
                    homeScreen->handleScrollEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = homeScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, homeScreen->getSidebar());
                        }
                        
                        std::string bookId = homeScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                bookDetailScreen->setBook(book, currentUserRole);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::CATEGORY:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = categoryScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, categoryScreen->getSidebar());
                        }
                        
                        categoryScreen->handleCategoryButtonClick(mousePos, mainFont);
                        categoryScreen->handleClick(mousePos, mainFont);
                        
                        std::string bookId = categoryScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                bookDetailScreen->setBook(book, currentUserRole);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::SEARCH:
                    searchScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = searchScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, searchScreen->getSidebar());
                        }
                        
                        searchScreen->handleSearchClick(mousePos, mainFont);
                        
                        std::string bookId = searchScreen->handleCardClick(mousePos);
                        if (!bookId.empty()) {
                            Sach* book = findBookById(bookId);
                            if (book) {
                                bookDetailScreen->setBook(book, currentUserRole);
                                detailModal->show();
                                changeState(AppState::BOOK_DETAIL);
                            }
                        }
                    }
                    break;

                case AppState::BOOK_DETAIL:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (detailModal->handleClose(mousePos)) {
                            detailModal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = bookDetailScreen->handleClick(mousePos);
                            if (action == 0) {
                                detailModal->hide();
                                changeState(AppState::HOME);
                            } else if (action == 1) {
                                handleBorrowBook();
                            } else if (action == 2) {
                                handleDeleteBook();
                            } else if (action == 3) {
                                handleUpdateBook();
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_CHOICE:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int choice = loginChoiceScreen->handleClick(mousePos);
                            if (choice == 1) {
                                changeState(AppState::LOGIN_LIBRARIAN);
                            } else if (choice == 2) {
                                changeState(AppState::LOGIN_READER);
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_LIBRARIAN:
                    loginLibrarianScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = loginLibrarianScreen->handleClick(mousePos);
                            if (action == 1) {
                                handleLibrarianLogin();
                            } else if (action == 2) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::LOGIN_READER:
                    loginReaderScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = loginReaderScreen->handleClick(mousePos);
                            if (action == 1) {
                                handleReaderLogin();
                            } else if (action == 2) {
                                changeState(AppState::REGISTER_READER);
                            } else if (action == 3) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::REGISTER_READER:
                    registerReaderScreen->handleEvent(event, mousePos);
                    
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (modal->handleClose(mousePos)) {
                            modal->hide();
                            changeState(AppState::HOME);
                        } else {
                            int action = registerReaderScreen->handleClick(mousePos);
                            if (action == 1) {
                                handleReaderRegister();
                            } else if (action == 2) {
                                changeState(AppState::LOGIN_READER);
                            } else if (action == 3) {
                                changeState(AppState::LOGIN_CHOICE);
                            }
                        }
                    }
                    break;

                case AppState::BORROWED_BOOKS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = borrowedBooksScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, borrowedBooksScreen->getSidebar());
                        }
                        
                        std::string bookId = borrowedBooksScreen->handleReturnClick(mousePos);
                        if (!bookId.empty() && bookId.find("BOOK_ID_") == 0) {
                            // TODO: Get actual book ID
                        }
                    }
                    break;

                case AppState::HISTORY:
                    historyScreen->handleScrollEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = historyScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, historyScreen->getSidebar());
                        }
                    }
                    break;

                case AppState::MANAGE_BOOKS:
                    manageBooksScreen->handleEvent();
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageBooksScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, manageBooksScreen->getSidebar());
                        }
                        
                        int action = manageBooksScreen->handleClick(mousePos);
                        if (action == 1) {
                            manageBooksScreen->handleAddBook();
                            homeScreen->loadBooks(mainFont);
                        } else if (action == 3) {
                            changeState(AppState::HOME);
                        } else if (action == 4) {
                            std::string bookId = manageBooksScreen->getSearchText();
                            if (!bookId.empty()) {
                                Sach* book = findBookById(bookId);
                                if (book) {
                                    bookDetailScreen->setBook(book, currentUserRole);
                                    detailModal->show();
                                    changeState(AppState::BOOK_DETAIL);
                                }
                            }
                        }
                    }
                    break;

                case AppState::MANAGE_READERS:
                    manageReadersScreen->handleEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = manageReadersScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, manageReadersScreen->getSidebar());
                        }
                        manageReadersScreen->handleClick(mousePos, mainFont);
                    }
                    break;

                case AppState::STATISTICS:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = statisticsScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, statisticsScreen->getSidebar());
                        }
                    }
                    break;

                case AppState::OVERDUE_READERS:
                    overdueReadersScreen->handleScrollEvent(event, mousePos);
                    if (event.type == sf::Event::MouseButtonPressed) {
                        int menuId = overdueReadersScreen->handleSidebarClick(mousePos);
                        if (menuId >= 0) {
                            handleSidebarNavigation(menuId, overdueReadersScreen->getSidebar());
                        }
                    }
                    break;
            }
        }
    }

    void update() {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        switch (currentState) {
            case AppState::SPLASH:
                splashScreen->update(mousePos);
                break;
            case AppState::HOME:
                homeScreen->update(mousePos);
                break;
            case AppState::CATEGORY:
                categoryScreen->update(mousePos);
                break;
            case AppState::SEARCH:
                searchScreen->update(mousePos);
                break;
            case AppState::BOOK_DETAIL:
                bookDetailScreen->update(mousePos);
                break;
            case AppState::LOGIN_CHOICE:
                loginChoiceScreen->update(mousePos);
                break;
            case AppState::LOGIN_LIBRARIAN:
                loginLibrarianScreen->update(mousePos);
                break;
            case AppState::LOGIN_READER:
                loginReaderScreen->update(mousePos);
                break;
            case AppState::REGISTER_READER:
                registerReaderScreen->update(mousePos);
                break;
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->update(mousePos);
                break;
            case AppState::HISTORY:
                historyScreen->update(mousePos);
                break;
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->update();
                break;
            case AppState::MANAGE_READERS:
                manageReadersScreen->update(mousePos);
                break;
            case AppState::STATISTICS:
                statisticsScreen->update(mousePos);
                break;
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->update(mousePos);
                break;
        }
    }

    void render() {
        window.clear(sf::Color(245, 245, 250)); // Light background

        switch (currentState) {
            case AppState::SPLASH:
                splashScreen->render(window);
                break;
                
            case AppState::HOME:
                homeScreen->render(window);
                break;
                
            case AppState::CATEGORY:
                categoryScreen->render(window);
                break;
                
            case AppState::SEARCH:
                searchScreen->render(window);
                break;
                
            case AppState::BOOK_DETAIL:
                homeScreen->render(window);
                detailModal->draw(window);
                bookDetailScreen->render(window);
                break;
                
            case AppState::LOGIN_CHOICE:
                homeScreen->render(window);
                modal->draw(window);
                loginChoiceScreen->render(window);
                break;
                
            case AppState::LOGIN_LIBRARIAN:
                homeScreen->render(window);
                modal->draw(window);
                loginLibrarianScreen->render(window);
                break;
                
            case AppState::LOGIN_READER:
                homeScreen->render(window);
                modal->draw(window);
                loginReaderScreen->render(window);
                break;
                
            case AppState::REGISTER_READER:
                homeScreen->render(window);
                modal->draw(window);
                registerReaderScreen->render(window);
                break;
                
            case AppState::BORROWED_BOOKS:
                borrowedBooksScreen->render(window);
                break;
                
            case AppState::HISTORY:
                historyScreen->render(window);
                break;
                
            case AppState::MANAGE_BOOKS:
                manageBooksScreen->render();
                break;
                
            case AppState::MANAGE_READERS:
                manageReadersScreen->render(window);
                break;
                
            case AppState::STATISTICS:
                statisticsScreen->render(window);
                break;
                
            case AppState::OVERDUE_READERS:
                overdueReadersScreen->render(window);
                break;
        }

        window.display();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};

#endif // APP_HPP// ============================================
// screens/BookDetailScreen.hpp - Chi tiết sách
// ============================================
#ifndef BOOK_DETAIL_SCREEN_HPP
#define BOOK_DETAIL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"
#include "Book.h"
#include "State.hpp"
#include "USER.h"

class BookDetailScreen {
private:
    Modal* modal;
    sf::RectangleShape detailBox;
    sf::RectangleShape coverBox;
    sf::Text titleText;
    sf::Text infoLabels;
    sf::Text infoValues;
    Button* actionButton;
    Button* deleteButton;
    Button* updateButton;
    Button* closeButton;
    Sach* currentBook;
    UserRole userRole;

public:
    BookDetailScreen(sf::Font& font, Modal* modalRef) : modal(modalRef), currentBook(nullptr) {
        detailBox.setSize(sf::Vector2f(800, 550));
        detailBox.setPosition(300, 175);
        detailBox.setFillColor(sf::Color(30, 35, 50));

        coverBox.setSize(sf::Vector2f(250, 350));
        coverBox.setPosition(330, 220);
        coverBox.setFillColor(sf::Color(60, 100, 180));

        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color(255, 193, 94));
        titleText.setPosition(620, 220);

        infoLabels.setFont(font);
        infoLabels.setCharacterSize(15);
        infoLabels.setFillColor(sf::Color(150, 150, 150));
        infoLabels.setPosition(620, 270);

        infoValues.setFont(font);
        infoValues.setCharacterSize(15);
        infoValues.setFillColor(sf::Color::White);
        infoValues.setPosition(750, 270);

        actionButton = new Button(sf::Vector2f(350, 620), sf::Vector2f(180, 50),
                                 "Muon Sach", font, 0, sf::Color(100, 180, 100));
        
        deleteButton = new Button(sf::Vector2f(350, 620), sf::Vector2f(180, 50),
                                 "Xoa Sach", font, 0, sf::Color(200, 80, 80));
        
        updateButton = new Button(sf::Vector2f(560, 620), sf::Vector2f(180, 50),
                                 "Cap Nhat", font, 0, sf::Color(100, 150, 200));
        
        closeButton = new Button(sf::Vector2f(870, 620), sf::Vector2f(150, 50),
                                "Dong", font, 0, sf::Color(100, 100, 120));

        userRole = UserRole::NONE;
    }

    ~BookDetailScreen() {
        delete actionButton;
        delete deleteButton;
        delete updateButton;
        delete closeButton;
    }

    void setBook(Sach* book, UserRole role) {
    currentBook = book;
    userRole = role;

    if (!book) {
        std::cout << "Loi: Book pointer la nullptr!" << std::endl;
        return;
    }

    std::cout << "====================================\n";
    std::cout << "Hien thi thong tin sach:\n";
    std::cout << "Ma: " << book->getMaSach() << "\n";
    std::cout << "Ten: " << book->getTenSach() << "\n";
    std::cout << "Tac gia: " << book->getTacGia() << "\n";
    std::cout << "====================================\n";

    // Màu cover ngẫu nhiên
    std::vector<sf::Color> colors = {
        sf::Color(200, 80, 60), sf::Color(60, 140, 200),
        sf::Color(100, 180, 100), sf::Color(200, 150, 60),
        sf::Color(140, 80, 180)
    };
    coverBox.setFillColor(colors[rand() % 5]);

    // Tiêu đề
    std::string title = book->getTenSach();
    if (title.length() > 30) title = title.substr(0, 30) + "...";
    titleText.setString(title);

    // Labels
    std::string labels = 
        "Ma sach:\n"
        "Tac gia:\n"
        "The loai:\n"
        "Nam XB:\n"
        "Nha XB:\n"
        "So luong:\n"
        "Danh gia:";
    infoLabels.setString(labels);

    // Values
    char ratingStr[50];
    sprintf(ratingStr, "%.1f/10 (%d luot)", 
            book->getDiemTrungBinh(), book->getSoDanhGia());
    
    std::string values = 
        book->getMaSach() + "\n" +
        book->getTacGia() + "\n" +
        book->getTheLoai() + "\n" +
        std::to_string(book->getNamXuatBan()) + "\n" +
        book->getNhaXuatBan() + "\n" +
        std::to_string(book->getSoLuong()) + " cuon\n" +
        std::string(ratingStr);
    infoValues.setString(values);
}

    void update(sf::Vector2f mousePos) {
        if (userRole == UserRole::READER) {
            actionButton->update(mousePos);
        } else if (userRole == UserRole::LIBRARIAN) {
            deleteButton->update(mousePos);
            updateButton->update(mousePos);
        }
        closeButton->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (closeButton->handleClick(mousePos)) {
            return 0;
        }
        
        if (userRole == UserRole::READER) {
            if (actionButton->handleClick(mousePos)) {
                return 1;
            }
        } else if (userRole == UserRole::LIBRARIAN) {
            if (deleteButton->handleClick(mousePos)) {
                return 2;
            }
            if (updateButton->handleClick(mousePos)) {
                return 3;
            }
        }
        
        return -1;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(detailBox);
            window.draw(coverBox);
            window.draw(titleText);
            window.draw(infoLabels);
            window.draw(infoValues);

            if (userRole == UserRole::READER) {
                actionButton->draw(window);
            } else if (userRole == UserRole::LIBRARIAN) {
                deleteButton->draw(window);
                updateButton->draw(window);
            }
            
            closeButton->draw(window);
        }
    }

    Sach* getCurrentBook() { return currentBook; }
};

#endif#ifndef BORROWED_BOOKS_SCREEN_HPP
#define BORROWED_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "Reader.h"
#include "Book.h"

struct BorrowedBookItem {
    sf::RectangleShape box;
    sf::Text titleText;
    sf::Text infoText;
    sf::Text statusText;
    Button* returnButton;
};

class BorrowedBooksScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    std::vector<BorrowedBookItem*> bookItems;
    Reader* currentReader;

public:
    BorrowedBooksScreen(sf::Font& font, Reader* reader) : currentReader(reader) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Sach Dang Muon");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Ban chua muon sach nao!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        
        loadBorrowedBooks(font);
    }

    ~BorrowedBooksScreen() {
        delete sidebar;
        for (auto item : bookItems) {
            delete item->returnButton;
            delete item;
        }
    }

    void loadBorrowedBooks(sf::Font& font) {
    for (auto item : bookItems) {
        delete item->returnButton;
        delete item;
    }
    bookItems.clear();

    if (!currentReader) return;

    // Đọc danh sách phiếu mượn từ Reader
    NodeMuonSach* current = currentReader->getDanhSachPhieuMuon();
    
    float yPos = 120;
    int index = 0;

    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        BorrowedBookItem* item = new BorrowedBookItem();
        
        item->box.setSize(sf::Vector2f(1000, 100));
        item->box.setPosition(300, yPos);
        item->box.setFillColor(sf::Color(25, 28, 40));
        item->box.setOutlineThickness(1);
        item->box.setOutlineColor(sf::Color(40, 43, 55));

        item->titleText.setFont(font);
        item->titleText.setString(phieu->sach->getTenSach());
        item->titleText.setCharacterSize(18);
        item->titleText.setFillColor(sf::Color::White);
        item->titleText.setPosition(320, yPos + 15);

        // Format ngày mượn và hạn trả
        char dateBuffer[50];
        tm* tmMuon = localtime(&phieu->ngayMuon);
        tm* tmHan = localtime(&phieu->ngayHetHan);
        sprintf(dateBuffer, "Ma: %s | Ngay muon: %02d/%02d/%04d | Han tra: %02d/%02d/%04d",
                phieu->sach->getMaSach().c_str(),
                tmMuon->tm_mday, tmMuon->tm_mon + 1, tmMuon->tm_year + 1900,
                tmHan->tm_mday, tmHan->tm_mon + 1, tmHan->tm_year + 1900);

        item->infoText.setFont(font);
        item->infoText.setString(dateBuffer);
        item->infoText.setCharacterSize(13);
        item->infoText.setFillColor(sf::Color(150, 150, 150));
        item->infoText.setPosition(320, yPos + 40);

        item->statusText.setFont(font);
        std::string status = phieu->trangThaiHan();
        item->statusText.setString(status);
        item->statusText.setCharacterSize(14);
        
        if (phieu->daQuaHan()) {
            item->statusText.setFillColor(sf::Color(255, 100, 100));
            item->box.setOutlineColor(sf::Color(200, 80, 80));
        } else {
            item->statusText.setFillColor(sf::Color(100, 180, 100));
        }
        item->statusText.setPosition(320, yPos + 65);

        item->returnButton = new Button(
            sf::Vector2f(1100, yPos + 25),
            sf::Vector2f(150, 50),
            "Tra Sach",
            font,
            index,
            sf::Color(100, 150, 200)
        );
        
        // Lưu mã sách vào button
        item->returnButton->setOnClick([this, phieu]() {
            // Callback sẽ được xử lý ở App
        });

        bookItems.push_back(item);
        yPos += 120;
        index++;
        current = current->next;
    }
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        for (auto item : bookItems) {
            item->returnButton->update(mousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    std::string handleReturnClick(sf::Vector2f mousePos) {
        for (auto item : bookItems) {
            if (item->returnButton->handleClick(mousePos)) {
                // Trả về ID sách để xử lý
                return "BOOK_ID_" + std::to_string(item->returnButton->getId());
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (bookItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : bookItems) {
                window.draw(item->box);
                window.draw(item->titleText);
                window.draw(item->infoText);
                window.draw(item->statusText);
                item->returnButton->draw(window);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "RoundedRectangle.hpp"

class Button {
private:
    RoundedRectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovered;
    std::function<void()> onClick;
    int id;

public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label,
           sf::Font& font, int buttonId = 0, sf::Color normal = sf::Color(33, 150, 243)) {
        
        shape.setSize(size);
        shape.setCornerRadius(8.0f);
        shape.setPosition(position);
        
        normalColor = normal;
        // Hover: làm sáng hơn
        hoverColor = sf::Color(
            std::min(255, (int)normal.r + 30),
            std::min(255, (int)normal.g + 30),
            std::min(255, (int)normal.b + 30)
        );
        shape.setFillColor(normalColor);
        isHovered = false;
        id = buttonId;

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(position.x + size.x / 2, position.y + size.y / 2 - 3);
    }

    void setOnClick(std::function<void()> callback) { onClick = callback; }
    int getId() const { return id; }

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                shape.setFillColor(hoverColor);
            }
        } else {
            if (isHovered) {
                isHovered = false;
                shape.setFillColor(normalColor);
            }
        }
    }

    bool handleClick(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            if (onClick) onClick();
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(pos.x + shape.getSize().x / 2, pos.y + shape.getSize().y / 2 - 3);
    }
};

#endif// ============================================
// ui/Card.hpp - Thẻ sách với bo góc
// ============================================
#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"

class Card {
private:
    RoundedRectangleShape cardShape;
    sf::RectangleShape coverImage;
    sf::Text titleText;
    sf::Text authorText;
    sf::Text yearText;
    sf::Text ratingText;
    bool isHovered;
    sf::Vector2f originalPos;
    std::string bookId;

public:
    Card(sf::Vector2f position, sf::Vector2f size, const std::string& id,
         const std::string& title, const std::string& author, 
         const std::string& year, float rating, sf::Color coverColor, sf::Font& font) {
        
        bookId = id;
        
        cardShape.setSize(size);
        cardShape.setCornerRadius(12.0f); // Bo góc 12px
        cardShape.setPosition(position);
        cardShape.setFillColor(sf::Color(25, 28, 40));
        
        originalPos = position;
        isHovered = false;

        coverImage.setSize(sf::Vector2f(size.x - 20, 160));
        coverImage.setPosition(position.x + 10, position.y + 10);
        coverImage.setFillColor(coverColor);

        titleText.setFont(font);
        std::string displayTitle = title.length() > 20 ? title.substr(0, 20) + "..." : title;
        titleText.setString(displayTitle);
        titleText.setCharacterSize(14);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(position.x + 15, position.y + 180);

        authorText.setFont(font);
        std::string displayAuthor = author.length() > 18 ? author.substr(0, 18) + "..." : author;
        authorText.setString(displayAuthor);
        authorText.setCharacterSize(12);
        authorText.setFillColor(sf::Color(150, 150, 150));
        authorText.setPosition(position.x + 15, position.y + 202);

        yearText.setFont(font);
        yearText.setString(year);
        yearText.setCharacterSize(11);
        yearText.setFillColor(sf::Color(100, 100, 100));
        yearText.setPosition(position.x + 15, position.y + 222);

        ratingText.setFont(font);
        char ratingStr[20];
        sprintf(ratingStr, "%.1f/10", rating);
        ratingText.setString(ratingStr);
        ratingText.setCharacterSize(12);
        ratingText.setFillColor(sf::Color(255, 193, 94));
        ratingText.setPosition(position.x + size.x - 50, position.y + 222);
    }

    void update(sf::Vector2f mousePos) {
        if (cardShape.getGlobalBounds().contains(mousePos)) {
            if (!isHovered) {
                isHovered = true;
                cardShape.setPosition(originalPos.x, originalPos.y - 8);
                updateTextPositions();
                cardShape.setOutlineThickness(2);
                cardShape.setOutlineColor(sf::Color(255, 193, 94, 150));
            }
        } else {
            if (isHovered) {
                isHovered = false;
                cardShape.setPosition(originalPos);
                updateTextPositions();
                cardShape.setOutlineThickness(0);
            }
        }
    }

    void updateTextPositions() {
        sf::Vector2f pos = cardShape.getPosition();
        coverImage.setPosition(pos.x + 10, pos.y + 10);
        titleText.setPosition(pos.x + 15, pos.y + 180);
        authorText.setPosition(pos.x + 15, pos.y + 202);
        yearText.setPosition(pos.x + 15, pos.y + 222);
        ratingText.setPosition(pos.x + cardShape.getSize().x - 50, pos.y + 222);
    }

    bool isClicked(sf::Vector2f mousePos) {
        return cardShape.getGlobalBounds().contains(mousePos);
    }

    std::string getBookId() const { return bookId; }

    void draw(sf::RenderWindow& window) {
        window.draw(cardShape);
        window.draw(coverImage);
        window.draw(titleText);
        window.draw(authorText);
        window.draw(yearText);
        window.draw(ratingText);
    }
};

#endif// ============================================
// screens/CategoryScreen.hpp - Thể loại sách
// ============================================
#ifndef CATEGORY_SCREEN_HPP
#define CATEGORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <string>
#include "Button.hpp"
#include "Card.hpp"
#include "Sidebar.hpp"
#include "LibrarySystem.h"
#include "Node.h"

class CategoryScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    Button* backButton;
    std::vector<Button*> categoryButtons;
    std::vector<Card*> categoryBookCards;
    LibrarySystem* libSystem;
    std::string selectedCategory;

public:
    CategoryScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("The Loai Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        selectedCategory = "";

        backButton = new Button(sf::Vector2f(280, 80), sf::Vector2f(150, 45),
                               "< Quay Lai", font, 0, sf::Color(100, 100, 120));

        loadCategories(font);
    }

    ~CategoryScreen() {
        delete sidebar;
        delete backButton;
        for (auto btn : categoryButtons) delete btn;
        for (auto card : categoryBookCards) delete card;
    }

    void loadCategories(sf::Font& font) {
        for (auto btn : categoryButtons) delete btn;
        categoryButtons.clear();

        if (!libSystem) return;

        std::set<std::string> categories;
        NodeBook* current = libSystem->getDanhSachSach();
        
        while (current != nullptr) {
            categories.insert(current->data->getTheLoai());
            current = current->next;
        }

        float btnX = 280;
        float btnY = 150;
        int col = 0;

        for (const auto& cat : categories) {
            Button* btn = new Button(
                sf::Vector2f(btnX, btnY),
                sf::Vector2f(200, 50),
                cat,
                font,
                0,
                sf::Color(60, 100, 180)
            );
            
            categoryButtons.push_back(btn);

            col++;
            if (col == 5) {
                col = 0;
                btnX = 280;
                btnY += 70;
            } else {
                btnX += 220;
            }
        }
    }

    void loadBooksOfCategory(const std::string& category, sf::Font& font) {
        selectedCategory = category;
        titleText.setString("The Loai: " + category);

        for (auto card : categoryBookCards) delete card;
        categoryBookCards.clear();

        if (!libSystem) return;

        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60),
            sf::Color(140, 80, 180)
        };

        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 150;
        int col = 0;
        int count = 0;

        while (current != nullptr) {
            if (current->data->getTheLoai() == category) {
                Card* card = new Card(
                    sf::Vector2f(cardX, cardY),
                    sf::Vector2f(180, 250),
                    current->data->getMaSach(),
                    current->data->getTenSach(),
                    current->data->getTacGia(),
                    std::to_string(current->data->getNamXuatBan()),
                    current->data->getDiemTrungBinh(),
                    colors[count % 5],
                    font
                );
                categoryBookCards.push_back(card);

                col++;
                if (col == 5) {
                    col = 0;
                    cardX = 280;
                    cardY += 270;
                } else {
                    cardX += 200;
                }
                count++;
            }
            current = current->next;
        }
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        backButton->update(mousePos);
        
        for (auto btn : categoryButtons) btn->update(mousePos);
        for (auto card : categoryBookCards) card->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (backButton->handleClick(mousePos)) {
            selectedCategory = "";
            titleText.setString("The Loai Sach");
            return;
        }
    }

    bool handleCategoryButtonClick(sf::Vector2f mousePos, sf::Font& font) {
        if (!selectedCategory.empty()) return false;
        
        std::set<std::string> categories;
        NodeBook* current = libSystem->getDanhSachSach();
        while (current != nullptr) {
            categories.insert(current->data->getTheLoai());
            current = current->next;
        }
        
        int index = 0;
        for (const auto& cat : categories) {
            if (index < (int)categoryButtons.size()) {
                if (categoryButtons[index]->handleClick(mousePos)) {
                    loadBooksOfCategory(cat, font);
                    return true;
                }
            }
            index++;
        }
        return false;
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        for (auto card : categoryBookCards) {
            if (card->isClicked(mousePos)) {
                return card->getBookId();
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (selectedCategory.empty()) {
            for (auto btn : categoryButtons) {
                btn->draw(window);
            }
        } else {
            backButton->draw(window);
            for (auto card : categoryBookCards) {
                card->draw(window);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef HISTORY_SCREEN_HPP
#define HISTORY_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "Reader.h"

struct HistoryItem {
    sf::RectangleShape box;
    sf::Text actionText;
    sf::Text bookText;
    sf::Text timeText;
};

class HistoryScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    std::vector<HistoryItem*> historyItems;
    Reader* currentReader;

public:
    HistoryScreen(sf::Font& font, Reader* reader) : currentReader(reader) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Lich Su Muon - Tra Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Chua co lich su nao!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadHistory(font);
    }

    ~HistoryScreen() {
        delete sidebar;
        delete scrollView;
        for (auto item : historyItems) delete item;
    }

    void loadHistory(sf::Font& font) {
        // Xóa items cũ
        for (auto item : historyItems) delete item;
        historyItems.clear();

        if (!currentReader) return;

        // TODO: Đọc từ file lịch sử
        // currentReader->HienThiLichSuMuonTra();

        float yPos = 120;
        for (int i = 0; i < 15; i++) {
            HistoryItem* item = new HistoryItem();
            
            item->box.setSize(sf::Vector2f(1000, 70));
            item->box.setPosition(300, yPos);
            item->box.setFillColor(sf::Color(25, 28, 40));
            item->box.setOutlineThickness(1);
            item->box.setOutlineColor(sf::Color(40, 43, 55));

            item->actionText.setFont(font);
            bool isBorrow = (i % 2 == 0);
            item->actionText.setString(isBorrow ? "MUON" : "TRA");
            item->actionText.setCharacterSize(16);
            item->actionText.setFillColor(isBorrow ? 
                sf::Color(255, 193, 94) : sf::Color(100, 180, 100));
            item->actionText.setPosition(320, yPos + 25);

            item->bookText.setFont(font);
            item->bookText.setString("Co So Du Lieu - Vo Duc Hoang");
            item->bookText.setCharacterSize(15);
            item->bookText.setFillColor(sf::Color::White);
            item->bookText.setPosition(420, yPos + 25);

            item->timeText.setFont(font);
            item->timeText.setString("15/11/2025 14:30:00");
            item->timeText.setCharacterSize(13);
            item->timeText.setFillColor(sf::Color(120, 120, 120));
            item->timeText.setPosition(1000, yPos + 27);

            historyItems.push_back(item);
            yPos += 80;
        }

        scrollView->setMaxScroll(std::max(0.0f, yPos - 700.0f));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (historyItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : historyItems) {
                window.draw(item->box);
                window.draw(item->actionText);
                window.draw(item->bookText);
                window.draw(item->timeText);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif// ============================================
// screens/HomeScreen.hpp - LIGHT THEME + FULL DISPLAY
// ============================================
#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "Card.hpp"
#include "InputField.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"

class HomeScreen {
private:
    sf::RectangleShape background;
    sf::Text titleTop10;
    sf::Text titleAllBooks;
    Sidebar* sidebar;
    InputField* searchBox;
    std::vector<Card*> top10Cards;
    std::vector<Card*> allBookCards;
    LibrarySystem* libSystem;
    ScrollView* scrollView;

public:
    HomeScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        // LIGHT THEME: Background sáng
        background.setFillColor(sf::Color(245, 245, 250));

        titleTop10.setFont(font);
        titleTop10.setString("Top 10 Sach Noi Bat");
        titleTop10.setCharacterSize(28);
        titleTop10.setFillColor(sf::Color(30, 30, 30)); // Text đen
        titleTop10.setPosition(280, 30);

        titleAllBooks.setFont(font);
        titleAllBooks.setString("Tat Ca Sach");
        titleAllBooks.setCharacterSize(28);
        titleAllBooks.setFillColor(sf::Color(30, 30, 30));
        titleAllBooks.setPosition(280, 350);

        sidebar = new Sidebar(font);
        searchBox = new InputField(sf::Vector2f(280, 80), sf::Vector2f(400, 45), 
                                   "Tim kiem sach...", font);

        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadBooks(font);
    }

    ~HomeScreen() {
        delete sidebar;
        delete searchBox;
        delete scrollView;
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
    }

    void loadBooks(sf::Font& font) {
        // Xóa cards cũ
        for (auto card : top10Cards) delete card;
        for (auto card : allBookCards) delete card;
        top10Cards.clear();
        allBookCards.clear();

        if (!libSystem) return;

        // LIGHT THEME: Màu sắc pastel sáng hơn
        std::vector<sf::Color> colors = {
            sf::Color(255, 182, 193), // Light pink
            sf::Color(173, 216, 230), // Light blue
            sf::Color(144, 238, 144), // Light green
            sf::Color(255, 218, 185), // Peach
            sf::Color(221, 160, 221), // Plum
            sf::Color(255, 228, 181), // Moccasin
            sf::Color(176, 224, 230), // Powder blue
            sf::Color(240, 230, 140), // Khaki
            sf::Color(255, 192, 203), // Pink
            sf::Color(175, 238, 238)  // Pale turquoise
        };

        // Top 10 sách
        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 140;
        int count = 0;

        // Top 10 - 2 hàng
        while (current != nullptr && count < 10) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardY),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[count % 10],
                font
            );
            top10Cards.push_back(card);
            
            count++;
            if (count == 5) {
                cardX = 280;
                cardY = 420; // Hàng thứ 2
            } else {
                cardX += 200;
            }
            current = current->next;
        }

        // TẤT CẢ SÁCH - Hiển thị đầy đủ
        current = libSystem->getDanhSachSach();
        cardX = 280;
        cardY = 750; // Bắt đầu sau top 10
        int col = 0;

        std::cout << "Loading all books...\n";
        int totalBooks = 0;
        
        while (current != nullptr) {
            Card* card = new Card(
                sf::Vector2f(cardX, cardY),
                sf::Vector2f(180, 250),
                current->data->getMaSach(),
                current->data->getTenSach(),
                current->data->getTacGia(),
                std::to_string(current->data->getNamXuatBan()),
                current->data->getDiemTrungBinh(),
                colors[totalBooks % 10],
                font
            );
            allBookCards.push_back(card);

            col++;
            totalBooks++;
            
            if (col == 5) {
                col = 0;
                cardX = 280;
                cardY += 270; // Next row
            } else {
                cardX += 200;
            }
            
            current = current->next;
        }

        std::cout << "Total books loaded: " << totalBooks << "\n";

        // Set scroll để có thể cuộn xem hết
        scrollView->setMaxScroll(std::max(0.0f, cardY - 400.0f));
        scrollView->reset();
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchBox->update();
        
        float scrollOffset = scrollView->getScrollOffset();
        
        for (auto card : top10Cards) {
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            card->update(adjustedMousePos);
        }
        
        for (auto card : allBookCards) {
            sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
            card->update(adjustedMousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleSearchEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchBox->handleEvent(event, mousePos);
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        float scrollOffset = scrollView->getScrollOffset();
        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
        
        for (auto card : top10Cards) {
            if (card->isClicked(adjustedMousePos)) {
                return card->getBookId();
            }
        }
        for (auto card : allBookCards) {
            if (card->isClicked(adjustedMousePos)) {
                return card->getBookId();
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        
        float scrollOffset = scrollView->getScrollOffset();
        
        // Title Top 10 (không scroll)
        window.draw(titleTop10);
        searchBox->draw(window);
        
        // Vẽ top 10 cards (không scroll - luôn ở trên)
        for (auto card : top10Cards) {
            card->draw(window);
        }
        
        // Title "Tất cả sách" với scroll
        sf::Text tempTitle = titleAllBooks;
        tempTitle.setPosition(280, 700);
        window.draw(tempTitle);
        
        // Vẽ all book cards với scroll
        sf::View originalView = window.getView();
        sf::View scrolledView = originalView;
        scrolledView.move(0, -scrollOffset);
        window.setView(scrolledView);
        
        for (auto card : allBookCards) {
            card->draw(window);
        }
        
        window.setView(originalView);
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef INPUT_FIELD_HPP
#define INPUT_FIELD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "RoundedRectangle.hpp"

class InputField {
private:
    RoundedRectangleShape box;
    sf::Text text;
    sf::Text placeholder;
    std::string inputText;
    bool isFocused;
    bool isPassword;
    sf::Clock cursorClock;
    bool showCursor;
    sf::Vector2f position;
    sf::Vector2f size;

public:
    InputField(sf::Vector2f pos, sf::Vector2f sz, const std::string& placeholderText,
               sf::Font& font, bool password = false) {
        position = pos;
        size = sz;
        isPassword = password;
        
        box.setSize(sz);
        box.setCornerRadius(8.0f);
        box.setPosition(pos);
        // LIGHT THEME: Background trắng
        box.setFillColor(sf::Color(255, 255, 255));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(220, 220, 220)); // Border xám nhạt

        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color(30, 30, 30)); // Text đen
        text.setPosition(pos.x + 15, pos.y + (sz.y - 20) / 2);

        placeholder.setFont(font);
        placeholder.setString(placeholderText);
        placeholder.setCharacterSize(16);
        placeholder.setFillColor(sf::Color(150, 150, 150)); // Gray placeholder
        placeholder.setPosition(pos.x + 15, pos.y + (sz.y - 20) / 2);

        inputText = "";
        isFocused = false;
        showCursor = false;
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (box.getGlobalBounds().contains(mousePos)) {
                isFocused = true;
                box.setOutlineColor(sf::Color(33, 150, 243)); // Blue khi focus
                box.setOutlineThickness(3);
            } else {
                isFocused = false;
                box.setOutlineColor(sf::Color(220, 220, 220));
                box.setOutlineThickness(2);
            }
        }

        if (isFocused && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && inputText.size() > 0) { 
                inputText.pop_back();
            } 
            else if (event.text.unicode == 13 || event.text.unicode == 9) {
                return;
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                inputText += static_cast<char>(event.text.unicode);
            }
            
            if (isPassword) {
                std::string displayText(inputText.size(), '*');
                text.setString(displayText);
            } else {
                text.setString(inputText);
            }
        }
    }

    void update() {
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        
        if (inputText.empty() && !isFocused) {
            window.draw(placeholder);
        } else {
            window.draw(text);
            
            if (isFocused && showCursor) {
                sf::RectangleShape cursor(sf::Vector2f(2, 20));
                cursor.setFillColor(sf::Color(30, 30, 30));
                sf::FloatRect textBounds = text.getGlobalBounds();
                cursor.setPosition(textBounds.left + textBounds.width + 2, 
                                 text.getPosition().y);
                window.draw(cursor);
            }
        }
    }

    std::string getText() const { return inputText; }
    void clear() { 
        inputText = ""; 
        text.setString("");
    }
    void setFocus(bool focus) { 
        isFocused = focus;
        if (focus) {
            box.setOutlineColor(sf::Color(33, 150, 243));
            box.setOutlineThickness(3);
        } else {
            box.setOutlineColor(sf::Color(220, 220, 220));
            box.setOutlineThickness(2);
        }
    }
    bool hasFocus() const { return isFocused; }
};

#endif// ============================================
// screens/LoginChoiceScreen.hpp - Chọn loại đăng nhập
// ============================================
#ifndef LOGIN_CHOICE_SCREEN_HPP
#define LOGIN_CHOICE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "Button.hpp"

class LoginChoiceScreen {
private:
    Modal* modal;
    sf::Text titleText;
    sf::Text descText;
    Button* librarianButton;
    Button* readerButton;

public:
    LoginChoiceScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        titleText.setFont(font);
        titleText.setString("Chon Loai Nguoi Dung");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 300);

        descText.setFont(font);
        descText.setString("Ban muon dang nhap voi tu cach nao?");
        descText.setCharacterSize(16);
        descText.setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect descBounds = descText.getLocalBounds();
        descText.setOrigin(descBounds.width / 2, descBounds.height / 2);
        descText.setPosition(700, 350);

        librarianButton = new Button(sf::Vector2f(450, 420), sf::Vector2f(220, 60),
                                     "Thu Thu", font, 0, sf::Color(100, 150, 255));
        
        readerButton = new Button(sf::Vector2f(730, 420), sf::Vector2f(220, 60),
                                  "Doc Gia", font, 0, sf::Color(255, 150, 100));
    }

    ~LoginChoiceScreen() {
        delete librarianButton;
        delete readerButton;
    }

    void update(sf::Vector2f mousePos) {
        librarianButton->update(mousePos);
        readerButton->update(mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (librarianButton->handleClick(mousePos)) return 1;
        if (readerButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(titleText);
            window.draw(descText);
            librarianButton->draw(window);
            readerButton->draw(window);
        }
    }
};

#endif// ============================================
// screens/LoginLibrarianScreen.hpp - Đăng nhập thủ thư
// ============================================
#ifndef LOGIN_LIBRARIAN_SCREEN_HPP
#define LOGIN_LIBRARIAN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class LoginLibrarianScreen {
private:
    Modal* modal;
    sf::Text titleText;
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

public:
    LoginLibrarianScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        titleText.setFont(font);
        titleText.setString("Dang Nhap Thu Thu");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 280);

        usernameField = new InputField(sf::Vector2f(500, 350), sf::Vector2f(400, 50),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(500, 420), sf::Vector2f(400, 50),
                                       "Password", font, true);

        loginButton = new Button(sf::Vector2f(500, 500), sf::Vector2f(400, 50),
                                "Dang Nhap", font);
        
        backButton = new Button(sf::Vector2f(500, 570), sf::Vector2f(400, 45),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~LoginLibrarianScreen() {
        delete usernameField;
        delete passwordField;
        delete loginButton;
        delete backButton;
    }

    void update(sf::Vector2f mousePos) {
        usernameField->update();
        passwordField->update();
        loginButton->update(mousePos);
        backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (loginButton->handleClick(mousePos)) return 1;
        if (backButton->handleClick(mousePos)) return 2;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(titleText);
            usernameField->draw(window);
            passwordField->draw(window);
            loginButton->draw(window);
            backButton->draw(window);
        }
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() {
        usernameField->clear();
        passwordField->clear();
    }
};

#endif// ============================================
// screens/LoginReaderScreen.hpp - Đăng nhập độc giả
// ============================================
#ifndef LOGIN_READER_SCREEN_HPP
#define LOGIN_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class LoginReaderScreen {
private:
    Modal* modal;
    sf::RectangleShape posterPanel;
    sf::RectangleShape formPanel;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text switchText;
    InputField* usernameField;
    InputField* passwordField;
    Button* loginButton;
    Button* backButton;

public:
    LoginReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(380, 600));
        posterPanel.setPosition(250, 150);
        posterPanel.setFillColor(sf::Color(40, 50, 70, 200));

        formPanel.setSize(sf::Vector2f(520, 600));
        formPanel.setPosition(630, 150);
        formPanel.setFillColor(sf::Color(30, 35, 50));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(40);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(440, 400);

        titleText.setFont(font);
        titleText.setString("Dang Nhap Doc Gia");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 220);

        switchText.setFont(font);
        switchText.setString("Chua co tai khoan? Dang ky ngay");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 640);

        usernameField = new InputField(sf::Vector2f(680, 300), sf::Vector2f(420, 50),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(680, 370), sf::Vector2f(420, 50),
                                       "Password", font, true);

        loginButton = new Button(sf::Vector2f(680, 450), sf::Vector2f(420, 50),
                                "Dang Nhap", font);
        
        backButton = new Button(sf::Vector2f(680, 520), sf::Vector2f(420, 45),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~LoginReaderScreen() {
        delete usernameField;
        delete passwordField;
        delete loginButton;
        delete backButton;
    }

    void update(sf::Vector2f mousePos) {
        usernameField->update();
        passwordField->update();
        loginButton->update(mousePos);
        backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (loginButton->handleClick(mousePos)) return 1;
        if (switchText.getGlobalBounds().contains(mousePos)) return 2;
        if (backButton->handleClick(mousePos)) return 3;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(posterPanel);
            window.draw(formPanel);
            window.draw(logoText);
            window.draw(titleText);
            usernameField->draw(window);
            passwordField->draw(window);
            loginButton->draw(window);
            backButton->draw(window);
            window.draw(switchText);
        }
    }

    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    void clearFields() {
        usernameField->clear();
        passwordField->clear();
    }
};

#endif// ============================================
// screens/ManageBooksScreen.hpp - FIXED VERSION
// ============================================
#ifndef MANAGE_BOOKS_SCREEN_HPP
#define MANAGE_BOOKS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Sidebar.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Modal.hpp"
#include "LibrarySystem.h"

class ManageBooksScreen {
private:
    sf::RenderWindow* windowRef;
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    Button* addButton;
    Button* viewAllButton;
    Button* searchButton;
    InputField* searchField;
    Modal* addBookModal;
    
    // Form thêm sách
    InputField* nameField;
    InputField* authorField;
    InputField* categoryField;
    InputField* yearField;
    InputField* publisherField;
    InputField* quantityField;
    Button* submitButton;
    Button* cancelButton;
    sf::Text formTitle;
    
    LibrarySystem* libSystem;
    std::vector<Button*> categoryButtons;

public:
    ManageBooksScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        windowRef = nullptr;
        
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        // LIGHT THEME
        background.setFillColor(sf::Color(245, 245, 250));

        titleText.setFont(font);
        titleText.setString("Quan Ly Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color(30, 30, 30));
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        addButton = new Button(sf::Vector2f(280, 100), sf::Vector2f(200, 60),
                              "Them Sach Moi", font, 0, sf::Color(76, 175, 80));
        
        viewAllButton = new Button(sf::Vector2f(500, 100), sf::Vector2f(200, 60),
                                  "Xem Tat Ca", font, 0, sf::Color(33, 150, 243));
        
        searchField = new InputField(sf::Vector2f(280, 200), sf::Vector2f(400, 50),
                                     "Nhap ma sach de tim...", font);
        
        searchButton = new Button(sf::Vector2f(700, 200), sf::Vector2f(150, 50),
                                 "Tim Kiem", font);

        // Modal thêm sách
        addBookModal = new Modal(font);

        formTitle.setFont(font);
        formTitle.setString("Them Sach Moi");
        formTitle.setCharacterSize(24);
        formTitle.setFillColor(sf::Color(30, 30, 30));
        formTitle.setPosition(600, 200);

        nameField = new InputField(sf::Vector2f(450, 270), sf::Vector2f(500, 45),
                                  "Ten sach", font);
        authorField = new InputField(sf::Vector2f(450, 330), sf::Vector2f(500, 45),
                                    "Tac gia", font);
        categoryField = new InputField(sf::Vector2f(450, 390), sf::Vector2f(500, 45),
                                      "The loai", font);
        yearField = new InputField(sf::Vector2f(450, 450), sf::Vector2f(240, 45),
                                  "Nam xuat ban", font);
        publisherField = new InputField(sf::Vector2f(710, 450), sf::Vector2f(240, 45),
                                       "Nha xuat ban", font);
        quantityField = new InputField(sf::Vector2f(450, 510), sf::Vector2f(240, 45),
                                      "So luong", font);

        submitButton = new Button(sf::Vector2f(450, 590), sf::Vector2f(240, 50),
                                 "Them Vao Thu Vien", font, 0, sf::Color(76, 175, 80));
        cancelButton = new Button(sf::Vector2f(710, 590), sf::Vector2f(240, 50),
                                 "Huy", font, 0, sf::Color(120, 120, 120));

        loadCategoryButtons(font);
    }

    ~ManageBooksScreen() {
        delete sidebar;
        delete addButton;
        delete viewAllButton;
        delete searchButton;
        delete searchField;
        delete addBookModal;
        delete nameField;
        delete authorField;
        delete categoryField;
        delete yearField;
        delete publisherField;
        delete quantityField;
        delete submitButton;
        delete cancelButton;
        for (auto btn : categoryButtons) delete btn;
    }

    void setWindow(sf::RenderWindow* win) {
        windowRef = win;
    }

    void loadCategoryButtons(sf::Font& font) {
        std::vector<std::string> categories = {
            "Giao trinh", "Tham khao", "Tieu thuyet", 
            "Truyen ngan", "Tap chi", "Truyen tranh", "Sach ki nang"
        };

        float btnX = 280;
        float btnY = 300;
        int col = 0;

        for (const auto& cat : categories) {
            Button* btn = new Button(
                sf::Vector2f(btnX, btnY),
                sf::Vector2f(180, 50),
                cat,
                font,
                0,
                sf::Color(100, 181, 246)
            );
            categoryButtons.push_back(btn);

            col++;
            if (col == 5) {
                col = 0;
                btnX = 280;
                btnY += 70;
            } else {
                btnX += 200;
            }
        }
    }

    void update() {
        if (!windowRef) return;
        
        sf::Vector2f mousePos = windowRef->mapPixelToCoords(
            sf::Mouse::getPosition(*windowRef)
        );
        
        sidebar->update(mousePos);
        addButton->update(mousePos);
        viewAllButton->update(mousePos);
        searchButton->update(mousePos);
        searchField->update();

        if (addBookModal->isShown()) {
            nameField->update();
            authorField->update();
            categoryField->update();
            yearField->update();
            publisherField->update();
            quantityField->update();
            submitButton->update(mousePos);
            cancelButton->update(mousePos);
        } else {
            for (auto btn : categoryButtons) btn->update(mousePos);
        }
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent() {
        if (!windowRef) return;
        
        sf::Event event;
        sf::Vector2f mousePos = windowRef->mapPixelToCoords(
            sf::Mouse::getPosition(*windowRef)
        );
        
        while (windowRef->pollEvent(event)) {
            searchField->handleEvent(event, mousePos);
            
            if (addBookModal->isShown()) {
                nameField->handleEvent(event, mousePos);
                authorField->handleEvent(event, mousePos);
                categoryField->handleEvent(event, mousePos);
                yearField->handleEvent(event, mousePos);
                publisherField->handleEvent(event, mousePos);
                quantityField->handleEvent(event, mousePos);
            }
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        if (addBookModal->isShown()) {
            if (addBookModal->handleClose(mousePos) || cancelButton->handleClick(mousePos)) {
                addBookModal->hide();
                clearFields();
                return 0;
            }
            if (submitButton->handleClick(mousePos)) {
                return 1; // Thêm sách
            }
        } else {
            if (addButton->handleClick(mousePos)) {
                addBookModal->show();
                return 2;
            }
            if (viewAllButton->handleClick(mousePos)) {
                return 3; // Xem tất cả
            }
            if (searchButton->handleClick(mousePos)) {
                return 4; // Tìm kiếm
            }
            for (auto btn : categoryButtons) {
                if (btn->handleClick(mousePos)) {
                    return 5; // Xem theo thể loại
                }
            }
        }
        return 0;
    }

    void handleAddBook() {
        std::string name = nameField->getText();
        std::string author = authorField->getText();
        std::string category = categoryField->getText();
        std::string yearStr = yearField->getText();
        std::string publisher = publisherField->getText();
        std::string quantityStr = quantityField->getText();
        
        if (name.empty() || author.empty() || category.empty() || 
            yearStr.empty() || publisher.empty() || quantityStr.empty()) {
            std::cout << "Loi: Vui long dien day du thong tin!" << std::endl;
            return;
        }
        
        std::ofstream out("ThemSach.txt", std::ios::app);
        if (out.is_open()) {
            out << name << "|" << author << "|" << category << "|"
                << yearStr << "|" << publisher << "|" << quantityStr << "\n";
            out.close();
        }
        
        libSystem->DocFileSach("ThemSach.txt");
        libSystem->GhiFileSach("DanhSachSach.txt");
        libSystem->GhiFileHeThong("DanhSachSach.txt");
        
        std::cout << "Da them sach: " << name << std::endl;
        
        addBookModal->hide();
        clearFields();
    }

    void clearFields() {
        nameField->clear();
        authorField->clear();
        categoryField->clear();
        yearField->clear();
        publisherField->clear();
        quantityField->clear();
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render() {
        if (!windowRef) return;
        
        windowRef->draw(background);
        sidebar->draw(*windowRef);
        windowRef->draw(titleText);

        if (!addBookModal->isShown()) {
            addButton->draw(*windowRef);
            viewAllButton->draw(*windowRef);
            searchField->draw(*windowRef);
            searchButton->draw(*windowRef);

            for (auto btn : categoryButtons) {
                btn->draw(*windowRef);
            }
        } else {
            addBookModal->draw(*windowRef);
            windowRef->draw(formTitle);
            nameField->draw(*windowRef);
            authorField->draw(*windowRef);
            categoryField->draw(*windowRef);
            yearField->draw(*windowRef);
            publisherField->draw(*windowRef);
            quantityField->draw(*windowRef);
            submitButton->draw(*windowRef);
            cancelButton->draw(*windowRef);
        }
    }

    std::string getSearchText() const { return searchField->getText(); }
    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef MANAGE_READERS_SCREEN_HPP
#define MANAGE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "LibrarySystem.h"

struct ReaderDisplayItem {
    sf::RectangleShape box;
    sf::Text idText;
    sf::Text nameText;
    sf::Text infoText;
    sf::Text statusText;
};

class ManageReadersScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    InputField* searchField;
    Button* searchButton;
    Button* viewAllButton;
    std::vector<ReaderDisplayItem*> readerItems;
    LibrarySystem* libSystem;

public:
    ManageReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Quan Ly Doc Gia");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));

        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(400, 50),
                                     "Tim kiem doc gia...", font);
        searchButton = new Button(sf::Vector2f(700, 100), sf::Vector2f(150, 50),
                                 "Tim", font);
        viewAllButton = new Button(sf::Vector2f(870, 100), sf::Vector2f(180, 50),
                                  "Xem Tat Ca", font, 0, sf::Color(100, 150, 200));

        loadReaders(font);
    }

    ~ManageReadersScreen() {
        delete sidebar;
        delete scrollView;
        delete searchField;
        delete searchButton;
        delete viewAllButton;
        for (auto item : readerItems) delete item;
    }

    void loadReaders(sf::Font& font) {
    for (auto item : readerItems) delete item;
    readerItems.clear();

    if (!libSystem) return;

    NodeReader* current = libSystem->getDanhSachDocGia();
    float yPos = 180;

    while (current != nullptr) {
        Reader* docGia = current->data;
        ReaderDisplayItem* item = new ReaderDisplayItem();
        
        item->box.setSize(sf::Vector2f(1000, 90));
        item->box.setPosition(300, yPos);
        item->box.setFillColor(sf::Color(25, 28, 40));
        item->box.setOutlineThickness(1);
        item->box.setOutlineColor(sf::Color(40, 43, 55));

        item->idText.setFont(font);
        item->idText.setString(docGia->getMaID());
        item->idText.setCharacterSize(18);
        item->idText.setFillColor(sf::Color(255, 193, 94));
        item->idText.setPosition(320, yPos + 15);

        item->nameText.setFont(font);
        item->nameText.setString(docGia->getHoTen());
        item->nameText.setCharacterSize(16);
        item->nameText.setFillColor(sf::Color::White);
        item->nameText.setPosition(420, yPos + 15);

        item->infoText.setFont(font);
        std::string info = "Email: " + docGia->getEmail() + " | SDT: " + docGia->getSDT();
        item->infoText.setString(info);
        item->infoText.setCharacterSize(13);
        item->infoText.setFillColor(sf::Color(150, 150, 150));
        item->infoText.setPosition(420, yPos + 40);

        int daMuon = docGia->DemSachDaMuon();
        int quaHan = docGia->DemSachQuaHan();
        
        item->statusText.setFont(font);
        std::string status = "Dang muon: " + std::to_string(daMuon) + "/" + 
                            std::to_string(docGia->getGioiHanMuon()) + " cuon";
        if (quaHan > 0) {
            status += " | QUA HAN: " + std::to_string(quaHan);
            item->statusText.setFillColor(sf::Color(255, 100, 100));
        } else {
            item->statusText.setFillColor(sf::Color(100, 180, 100));
        }
        item->statusText.setString(status);
        item->statusText.setCharacterSize(14);
        item->statusText.setPosition(420, yPos + 62);

        readerItems.push_back(item);
        yPos += 100;
        current = current->next;
    }

    scrollView->setMaxScroll(std::max(0.0f, yPos - 600.0f));
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchField->update();
        searchButton->update(mousePos);
        viewAllButton->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
        scrollView->handleScroll(event, mousePos);
    }

    void handleClick(sf::Vector2f mousePos, sf::Font& font) {
        if (searchButton->handleClick(mousePos)) {
            // Tìm kiếm độc giả
        }
        if (viewAllButton->handleClick(mousePos)) {
            loadReaders(font); // TODO: Pass font properly
        }
    }
    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        searchField->draw(window);
        searchButton->draw(window);
        viewAllButton->draw(window);

        for (auto item : readerItems) {
            window.draw(item->box);
            window.draw(item->idText);
            window.draw(item->nameText);
            window.draw(item->infoText);
            window.draw(item->statusText);
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.hpp"

enum class MessageType {
    INFO,
    SUCCESS,
    WARNING,
    ERROR,
    CONFIRM
};

class MessageBox {
private:
    sf::RectangleShape overlay;
    sf::RectangleShape box;
    sf::Text titleText;
    sf::Text messageText;
    Button* okButton;
    Button* cancelButton;
    bool isVisible;
    bool hasCancel;
    MessageType type;
    std::function<void()> onConfirm;
    std::function<void()> onCancel;

public:
    MessageBox(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));

        box.setSize(sf::Vector2f(500, 250));
        box.setPosition(450, 325);
        box.setFillColor(sf::Color(30, 35, 50));
        box.setOutlineThickness(2);

        titleText.setFont(font);
        titleText.setCharacterSize(22);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(480, 350);

        messageText.setFont(font);
        messageText.setCharacterSize(16);
        messageText.setFillColor(sf::Color(200, 200, 200));
        messageText.setPosition(480, 400);

        okButton = new Button(sf::Vector2f(650, 500), sf::Vector2f(120, 45),
                             "OK", font, 0, sf::Color(100, 180, 100));
        
        cancelButton = new Button(sf::Vector2f(500, 500), sf::Vector2f(120, 45),
                                 "Huy", font, 0, sf::Color(120, 120, 120));

        isVisible = false;
        hasCancel = false;
        type = MessageType::INFO;
    }

    ~MessageBox() {
        delete okButton;
        delete cancelButton;
    }

    void show(const std::string& title, const std::string& message, 
              MessageType msgType = MessageType::INFO, bool showCancel = false) {
        titleText.setString(title);
        messageText.setString(message);
        type = msgType;
        hasCancel = showCancel;
        isVisible = true;

        // Đặt màu theo loại
        switch(type) {
            case MessageType::SUCCESS:
                box.setOutlineColor(sf::Color(100, 180, 100));
                titleText.setFillColor(sf::Color(100, 255, 100));
                break;
            case MessageType::ERROR:
                box.setOutlineColor(sf::Color(255, 100, 100));
                titleText.setFillColor(sf::Color(255, 100, 100));
                break;
            case MessageType::WARNING:
                box.setOutlineColor(sf::Color(255, 193, 94));
                titleText.setFillColor(sf::Color(255, 193, 94));
                break;
            case MessageType::CONFIRM:
                box.setOutlineColor(sf::Color(100, 150, 255));
                titleText.setFillColor(sf::Color(100, 150, 255));
                break;
            default:
                box.setOutlineColor(sf::Color(100, 100, 120));
                titleText.setFillColor(sf::Color::White);
        }

        // Điều chỉnh vị trí nút
        if (hasCancel) {
            okButton->setPosition(sf::Vector2f(650, 500));
            cancelButton->setPosition(sf::Vector2f(500, 500));
        } else {
            okButton->setPosition(sf::Vector2f(575, 500));
        }
    }

    void hide() {
        isVisible = false;
    }

    bool isShown() const {
        return isVisible;
    }

    void setOnConfirm(std::function<void()> callback) {
        onConfirm = callback;
    }

    void setOnCancel(std::function<void()> callback) {
        onCancel = callback;
    }

    void update(sf::Vector2f mousePos) {
        if (!isVisible) return;
        
        okButton->update(mousePos);
        if (hasCancel) {
            cancelButton->update(mousePos);
        }
    }

    bool handleClick(sf::Vector2f mousePos) {
        if (!isVisible) return false;

        if (okButton->handleClick(mousePos)) {
            if (onConfirm) onConfirm();
            hide();
            return true;
        }

        if (hasCancel && cancelButton->handleClick(mousePos)) {
            if (onCancel) onCancel();
            hide();
            return true;
        }

        return false;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        window.draw(overlay);
        window.draw(box);
        window.draw(titleText);
        window.draw(messageText);
        okButton->draw(window);
        
        if (hasCancel) {
            cancelButton->draw(window);
        }
    }
};

#endif// ============================================
// ui/Modal.hpp - Popup overlay với bo góc
// ============================================
#ifndef MODAL_HPP
#define MODAL_HPP

#include <SFML/Graphics.hpp>
#include "RoundedRectangle.hpp"

class Modal {
private:
    sf::RectangleShape overlay;
    RoundedRectangleShape modalBox;
    RoundedRectangleShape closeButton;
    sf::Text closeText;
    bool isVisible;

public:
    Modal(sf::Font& font) {
        overlay.setSize(sf::Vector2f(1400, 900));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        overlay.setPosition(0, 0);

        modalBox.setSize(sf::Vector2f(900, 600));
        modalBox.setCornerRadius(16.0f); // Bo góc 16px
        modalBox.setPosition(250, 150);
        modalBox.setFillColor(sf::Color(30, 35, 50));

        closeButton.setSize(sf::Vector2f(35, 35));
        closeButton.setCornerRadius(17.5f); // Bo góc tròn
        closeButton.setPosition(1115, 155);
        closeButton.setFillColor(sf::Color(60, 63, 75));

        closeText.setFont(font);
        closeText.setString("X");
        closeText.setCharacterSize(20);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition(1128, 160);

        isVisible = false;
    }

    void show() { isVisible = true; }
    void hide() { isVisible = false; }
    bool isShown() const { return isVisible; }

    bool handleClose(sf::Vector2f mousePos) {
        if (closeButton.getGlobalBounds().contains(mousePos)) {
            hide();
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        if (isVisible) {
            window.draw(overlay);
            window.draw(modalBox);
            window.draw(closeButton);
            window.draw(closeText);
        }
    }

    RoundedRectangleShape& getModalBox() { return modalBox; }
};

#endif#ifndef OVERDUE_READERS_SCREEN_HPP
#define OVERDUE_READERS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "ScrollView.hpp"
#include "LibrarySystem.h"
#include "Reader.h"

struct OverdueReaderItem {
    sf::RectangleShape box;
    sf::Text idText;
    sf::Text nameText;
    sf::Text contactText;
    sf::Text overdueText;
};

class OverdueReadersScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text emptyText;
    Sidebar* sidebar;
    ScrollView* scrollView;
    std::vector<OverdueReaderItem*> readerItems;
    LibrarySystem* libSystem;

public:
    OverdueReadersScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Doc Gia Co Sach Qua Han");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color(255, 80, 80));
        titleText.setPosition(280, 30);

        emptyText.setFont(font);
        emptyText.setString("Khong co doc gia nao qua han!");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(100, 180, 100));
        emptyText.setPosition(550, 400);

        sidebar = new Sidebar(font);
        scrollView = new ScrollView(sf::FloatRect(250, 0, 1150, 900));
        
        loadOverdueReaders(font);
    }

    ~OverdueReadersScreen() {
        delete sidebar;
        delete scrollView;
        for (auto item : readerItems) delete item;
    }

    void loadOverdueReaders(sf::Font& font) {
        // Xóa items cũ
        for (auto item : readerItems) delete item;
        readerItems.clear();

        if (!libSystem) return;

        // Lấy danh sách độc giả từ LibrarySystem
        NodeReader* current = libSystem->getDanhSachDocGia();
        
        float yPos = 120;
        int count = 0;

        // Header bảng
        while (current != nullptr) {
            Reader* docGia = current->data;
            int soQuaHan = docGia->DemSachQuaHan();
            
            if (soQuaHan > 0) {
                OverdueReaderItem* item = new OverdueReaderItem();
                
                item->box.setSize(sf::Vector2f(1000, 80));
                item->box.setPosition(300, yPos);
                item->box.setFillColor(sf::Color(40, 30, 30));
                item->box.setOutlineThickness(2);
                item->box.setOutlineColor(sf::Color(200, 80, 80));

                item->idText.setFont(font);
                item->idText.setString(docGia->getMaID());
                item->idText.setCharacterSize(16);
                item->idText.setFillColor(sf::Color(255, 193, 94));
                item->idText.setPosition(320, yPos + 15);

                item->nameText.setFont(font);
                item->nameText.setString(docGia->getHoTen());
                item->nameText.setCharacterSize(16);
                item->nameText.setFillColor(sf::Color::White);
                item->nameText.setPosition(450, yPos + 15);

                item->contactText.setFont(font);
                std::string contact = docGia->getSDT() + " | " + docGia->getEmail();
                if (contact.length() > 40) contact = contact.substr(0, 40) + "...";
                item->contactText.setString(contact);
                item->contactText.setCharacterSize(13);
                item->contactText.setFillColor(sf::Color(150, 150, 150));
                item->contactText.setPosition(450, yPos + 40);

                item->overdueText.setFont(font);
                item->overdueText.setString("SO SACH QUA HAN: " + std::to_string(soQuaHan));
                item->overdueText.setCharacterSize(16);
                item->overdueText.setFillColor(sf::Color(255, 100, 100));
                item->overdueText.setPosition(900, yPos + 28);

                readerItems.push_back(item);
                yPos += 90;
                count++;
            }
            
            current = current->next;
        }

        scrollView->setMaxScroll(std::max(0.0f, yPos - 700.0f));
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleScrollEvent(sf::Event& event, sf::Vector2f mousePos) {
        scrollView->handleScroll(event, mousePos);
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        if (readerItems.empty()) {
            window.draw(emptyText);
        } else {
            for (auto item : readerItems) {
                window.draw(item->box);
                window.draw(item->idText);
                window.draw(item->nameText);
                window.draw(item->contactText);
                window.draw(item->overdueText);
            }
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <SFML/Graphics.hpp>

class ProgressBar {
private:
    sf::RectangleShape background;
    sf::RectangleShape fill;
    sf::Text percentText;
    float progress;

public:
    ProgressBar(sf::Font& font, sf::Vector2f position, sf::Vector2f size) {
        background.setSize(size);
        background.setPosition(position);
        background.setFillColor(sf::Color(40, 40, 40));
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color(80, 80, 80));

        fill.setSize(sf::Vector2f(0, size.y - 4));
        fill.setPosition(position.x + 2, position.y + 2);
        fill.setFillColor(sf::Color(100, 180, 100));

        percentText.setFont(font);
        percentText.setCharacterSize(14);
        percentText.setFillColor(sf::Color::White);
        
        progress = 0.0f;
    }

    void setProgress(float value) {
        progress = std::max(0.0f, std::min(100.0f, value));
        
        float width = (background.getSize().x - 4) * (progress / 100.0f);
        fill.setSize(sf::Vector2f(width, fill.getSize().y));

        percentText.setString(std::to_string((int)progress) + "%");
        sf::FloatRect textBounds = percentText.getLocalBounds();
        percentText.setPosition(
            background.getPosition().x + background.getSize().x / 2 - textBounds.width / 2,
            background.getPosition().y + background.getSize().y / 2 - textBounds.height / 2 - 3
        );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(fill);
        window.draw(percentText);
    }

    float getProgress() const { return progress; }
};

#endif// ============================================
// screens/RegisterReaderScreen.hpp - Đăng ký độc giả
// ============================================
#ifndef REGISTER_READER_SCREEN_HPP
#define REGISTER_READER_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Modal.hpp"
#include "InputField.hpp"
#include "Button.hpp"

class RegisterReaderScreen {
private:
    Modal* modal;
    sf::RectangleShape posterPanel;
    sf::RectangleShape formPanel;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text switchText;
    InputField* nameField;
    InputField* phoneField;
    InputField* emailField;
    InputField* usernameField;
    InputField* passwordField;
    InputField* confirmPasswordField;
    Button* registerButton;
    Button* backButton;

public:
    RegisterReaderScreen(sf::Font& font, Modal* modalRef) : modal(modalRef) {
        posterPanel.setSize(sf::Vector2f(380, 600));
        posterPanel.setPosition(250, 150);
        posterPanel.setFillColor(sf::Color(40, 50, 70, 200));

        formPanel.setSize(sf::Vector2f(520, 600));
        formPanel.setPosition(630, 150);
        formPanel.setFillColor(sf::Color(30, 35, 50));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(40);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(440, 400);

        titleText.setFont(font);
        titleText.setString("Dang Ky Doc Gia");
        titleText.setCharacterSize(26);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(700, 180);

        switchText.setFont(font);
        switchText.setString("Da co tai khoan? Dang nhap");
        switchText.setCharacterSize(13);
        switchText.setFillColor(sf::Color(255, 193, 94));
        switchText.setPosition(700, 700);

        nameField = new InputField(sf::Vector2f(680, 230), sf::Vector2f(420, 45),
                                   "Ho ten", font);
        
        phoneField = new InputField(sf::Vector2f(680, 290), sf::Vector2f(420, 45),
                                    "So dien thoai", font);
        
        emailField = new InputField(sf::Vector2f(680, 350), sf::Vector2f(420, 45),
                                    "Email", font);
        
        usernameField = new InputField(sf::Vector2f(680, 410), sf::Vector2f(420, 45),
                                       "Username", font);
        
        passwordField = new InputField(sf::Vector2f(680, 470), sf::Vector2f(420, 45),
                                       "Password", font, true);
        
        confirmPasswordField = new InputField(sf::Vector2f(680, 530), sf::Vector2f(420, 45),
                                             "Xac nhan password", font, true);

        registerButton = new Button(sf::Vector2f(680, 595), sf::Vector2f(420, 45),
                                   "Dang Ky", font);
        
        backButton = new Button(sf::Vector2f(680, 655), sf::Vector2f(420, 40),
                               "Quay Lai", font, 0, sf::Color(100, 100, 120));
    }

    ~RegisterReaderScreen() {
        delete nameField;
        delete phoneField;
        delete emailField;
        delete usernameField;
        delete passwordField;
        delete confirmPasswordField;
        delete registerButton;
        delete backButton;
    }

    void update(sf::Vector2f mousePos) {
        nameField->update();
        phoneField->update();
        emailField->update();
        usernameField->update();
        passwordField->update();
        confirmPasswordField->update();
        registerButton->update(mousePos);
        backButton->update(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        nameField->handleEvent(event, mousePos);
        phoneField->handleEvent(event, mousePos);
        emailField->handleEvent(event, mousePos);
        usernameField->handleEvent(event, mousePos);
        passwordField->handleEvent(event, mousePos);
        confirmPasswordField->handleEvent(event, mousePos);
    }

    int handleClick(sf::Vector2f mousePos) {
        if (registerButton->handleClick(mousePos)) return 1;
        if (switchText.getGlobalBounds().contains(mousePos)) return 2;
        if (backButton->handleClick(mousePos)) return 3;
        return 0;
    }

    void render(sf::RenderWindow& window) {
        if (modal && modal->isShown()) {
            window.draw(posterPanel);
            window.draw(formPanel);
            window.draw(logoText);
            window.draw(titleText);
            nameField->draw(window);
            phoneField->draw(window);
            emailField->draw(window);
            usernameField->draw(window);
            passwordField->draw(window);
            confirmPasswordField->draw(window);
            registerButton->draw(window);
            backButton->draw(window);
            window.draw(switchText);
        }
    }

    std::string getName() const { return nameField->getText(); }
    std::string getPhone() const { return phoneField->getText(); }
    std::string getEmail() const { return emailField->getText(); }
    std::string getUsername() const { return usernameField->getText(); }
    std::string getPassword() const { return passwordField->getText(); }
    std::string getConfirmPassword() const { return confirmPasswordField->getText(); }
    
    void clearFields() {
        nameField->clear();
        phoneField->clear();
        emailField->clear();
        usernameField->clear();
        passwordField->clear();
        confirmPasswordField->clear();
    }
};

#endif#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
private:
    std::map<std::string, sf::Font> fonts;
    static ResourceManager* instance;
    
    ResourceManager() {}

public:
    static ResourceManager* getInstance();
    bool loadFont(const std::string& name, const std::string& path);
    sf::Font& getFont(const std::string& name);
};

#endif// ============================================
// ui/RoundedRectangle.hpp - Bo góc không dùng setRadius
// ============================================
#ifndef ROUNDED_RECTANGLE_HPP
#define ROUNDED_RECTANGLE_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape {
private:
    sf::Vector2f size;
    float radius;
    unsigned int cornerPointCount;

public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0), 
                                   float radius = 10, 
                                   unsigned int cornerPointCount = 10)
        : size(size), radius(radius), cornerPointCount(cornerPointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) {
        this->size = size;
        update();
    }

    const sf::Vector2f& getSize() const {
        return size;
    }

    void setCornerRadius(float radius) {
        this->radius = radius;
        update();
    }

    float getCornerRadius() const {
        return radius;
    }

    virtual std::size_t getPointCount() const override {
        return cornerPointCount * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= cornerPointCount * 4)
            return sf::Vector2f(0, 0);

        float deltaAngle = 90.0f / (cornerPointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / cornerPointCount;
        unsigned int pointIndex = index % cornerPointCount;

        switch (centerIndex) {
            case 0: // Top-left
                center.x = radius;
                center.y = radius;
                return center + sf::Vector2f(
                    radius * std::cos((180 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((180 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 1: // Top-right
                center.x = size.x - radius;
                center.y = radius;
                return center + sf::Vector2f(
                    radius * std::cos((270 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((270 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 2: // Bottom-right
                center.x = size.x - radius;
                center.y = size.y - radius;
                return center + sf::Vector2f(
                    radius * std::cos((0 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((0 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
            case 3: // Bottom-left
                center.x = radius;
                center.y = size.y - radius;
                return center + sf::Vector2f(
                    radius * std::cos((90 + deltaAngle * pointIndex) * 3.14159f / 180),
                    radius * std::sin((90 + deltaAngle * pointIndex) * 3.14159f / 180)
                );
        }

        return sf::Vector2f(0, 0);
    }
};

#endif// ============================================
// ui/ScrollView.hpp - Cuộn danh sách
// ============================================
#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <SFML/Graphics.hpp>

class ScrollView {
private:
    float scrollOffset;
    float maxScroll;
    sf::FloatRect bounds;
    float scrollSpeed;

public:
    ScrollView(sf::FloatRect viewBounds) : bounds(viewBounds) {
        scrollOffset = 0;
        maxScroll = 0;
        scrollSpeed = 30.0f;
    }

    void handleScroll(sf::Event& event, sf::Vector2f mousePos) {
        // Kiểm tra chuột có trong vùng scroll không
        if (!bounds.contains(mousePos)) return;
        
        if (event.type == sf::Event::MouseWheelScrolled) {
            scrollOffset -= event.mouseWheelScroll.delta * scrollSpeed;
            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > maxScroll) scrollOffset = maxScroll;
        }
    }

    void setMaxScroll(float max) { 
        maxScroll = std::max(0.0f, max);
    }
    
    float getScrollOffset() const { return scrollOffset; }
    void reset() { scrollOffset = 0; }
    
    // Áp dụng offset cho position của object
    sf::Vector2f applyScroll(sf::Vector2f pos) const {
        return sf::Vector2f(pos.x, pos.y - scrollOffset);
    }
};

#endif// ============================================
// screens/SearchScreen.hpp - Tìm kiếm sách
// ============================================
#ifndef SEARCH_SCREEN_HPP
#define SEARCH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sidebar.hpp"
#include "InputField.hpp"
#include "Button.hpp"
#include "Card.hpp"
#include "LibrarySystem.h"

class SearchScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text resultText;
    Sidebar* sidebar;
    InputField* searchField;
    Button* searchButton;
    std::vector<Card*> resultCards;
    LibrarySystem* libSystem;

public:
    SearchScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Tim Kiem Sach");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        resultText.setFont(font);
        resultText.setString("Nhap tu khoa de tim kiem...");
        resultText.setCharacterSize(16);
        resultText.setFillColor(sf::Color(150, 150, 150));
        resultText.setPosition(280, 200);

        sidebar = new Sidebar(font);

        searchField = new InputField(sf::Vector2f(280, 100), sf::Vector2f(600, 50),
                                     "Ten sach, tac gia, the loai...", font);
        searchButton = new Button(sf::Vector2f(900, 100), sf::Vector2f(150, 50),
                                 "Tim Kiem", font);
    }

    ~SearchScreen() {
        delete sidebar;
        delete searchField;
        delete searchButton;
        for (auto card : resultCards) delete card;
    }

    void performSearch(sf::Font& font) {
        for (auto card : resultCards) delete card;
        resultCards.clear();

        std::string keyword = searchField->getText();
        if (keyword.empty()) {
            resultText.setString("Vui long nhap tu khoa!");
            resultText.setFillColor(sf::Color(255, 100, 100));
            return;
        }

        if (!libSystem) return;

        std::string keywordLower = keyword;
        for (char& c : keywordLower) c = tolower(c);

        std::vector<sf::Color> colors = {
            sf::Color(200, 80, 60), sf::Color(60, 140, 200),
            sf::Color(100, 180, 100), sf::Color(200, 150, 60),
            sf::Color(140, 80, 180)
        };

        NodeBook* current = libSystem->getDanhSachSach();
        float cardX = 280;
        float cardY = 250;
        int col = 0;
        int count = 0;

        while (current != nullptr) {
            Sach* book = current->data;
            
            std::string tenSach = book->getTenSach();
            std::string tacGia = book->getTacGia();
            std::string theLoai = book->getTheLoai();
            std::string maSach = book->getMaSach();
            
            for (char& c : tenSach) c = tolower(c);
            for (char& c : tacGia) c = tolower(c);
            for (char& c : theLoai) c = tolower(c);
            for (char& c : maSach) c = tolower(c);
            
            if (tenSach.find(keywordLower) != std::string::npos ||
                tacGia.find(keywordLower) != std::string::npos ||
                theLoai.find(keywordLower) != std::string::npos ||
                maSach.find(keywordLower) != std::string::npos) {
                
                Card* card = new Card(
                    sf::Vector2f(cardX, cardY),
                    sf::Vector2f(180, 250),
                    book->getMaSach(),
                    book->getTenSach(),
                    book->getTacGia(),
                    std::to_string(book->getNamXuatBan()),
                    book->getDiemTrungBinh(),
                    colors[count % 5],
                    font
                );
                resultCards.push_back(card);

                col++;
                if (col == 5) {
                    col = 0;
                    cardX = 280;
                    cardY += 270;
                } else {
                    cardX += 200;
                }
                count++;
            }
            
            current = current->next;
        }

        if (count == 0) {
            resultText.setString("Khong tim thay ket qua nao cho: \"" + keyword + "\"");
            resultText.setFillColor(sf::Color(255, 193, 94));
        } else {
            resultText.setString("Tim thay " + std::to_string(count) + " ket qua");
            resultText.setFillColor(sf::Color(100, 180, 100));
        }
    }

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
        searchField->update();
        searchButton->update(mousePos);
        for (auto card : resultCards) card->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void handleEvent(sf::Event& event, sf::Vector2f mousePos) {
        searchField->handleEvent(event, mousePos);
    }

    bool handleSearchClick(sf::Vector2f mousePos, sf::Font& font) {
        if (searchButton->handleClick(mousePos)) {
            performSearch(font);
            return true;
        }
        return false;
    }

    std::string handleCardClick(sf::Vector2f mousePos) {
        for (auto card : resultCards) {
            if (card->isClicked(mousePos)) {
                return card->getBookId();
            }
        }
        return "";
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);
        searchField->draw(window);
        searchButton->draw(window);
        window.draw(resultText);

        for (auto card : resultCards) {
            card->draw(window);
        }
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif// ============================================
// ui/Sidebar.hpp - LIGHT THEME + FIXED MENU
// ============================================
#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "State.hpp"
#include "USER.h"

struct MenuItem {
    std::string label;
    sf::RectangleShape shape;
    sf::Text text;
    bool isActive;
    int id;
};

class Sidebar {
private:
    sf::RectangleShape background;
    sf::Text logoText;
    std::vector<MenuItem> menuItems;
    int activeIndex;
    UserRole currentRole;

public:
    Sidebar(sf::Font& font) {
        background.setSize(sf::Vector2f(250, 900));
        background.setPosition(0, 0);
        // LIGHT THEME: Sidebar trắng
        background.setFillColor(sf::Color(255, 255, 255));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(32);
        // Logo màu xanh dương đậm
        logoText.setFillColor(sf::Color(41, 98, 255));
        logoText.setPosition(30, 30);

        activeIndex = 0;
        currentRole = UserRole::NONE;

        buildMenu(font);
    }

    void buildMenu(sf::Font& font) {
        menuItems.clear();
        std::vector<std::string> labels;

        // FIX: Menu đầy đủ với các mục rõ ràng
        if (currentRole == UserRole::NONE) {
            labels = {
                "Home", 
                "Top 10 sach",      // Mục riêng cho Top 10
                "Tat ca sach",      // Mục riêng cho tất cả sách
                "The loai", 
                "Tim kiem", 
                "Thanh vien"
            };
        } else if (currentRole == UserRole::READER) {
            labels = {
                "Home", 
                "Top 10 sach", 
                "Tat ca sach",
                "The loai",
                "Tim kiem", 
                "Sach dang muon", 
                "Lich su", 
                "Dang xuat"
            };
        } else if (currentRole == UserRole::LIBRARIAN) {
            labels = {
                "Home", 
                "Top 10 sach", 
                "Tat ca sach",
                "The loai",
                "Tim kiem", 
                "Quan ly sach",      // Chức năng quản lý sách
                "Quan ly doc gia", 
                "Doc gia qua han", 
                "Thong ke", 
                "Dang xuat"
            };
        }

        float yPos = 120;
        for (size_t i = 0; i < labels.size(); i++) {
            MenuItem item;
            item.label = labels[i];
            item.id = i;
            item.isActive = (i == 0);
            
            item.shape.setSize(sf::Vector2f(220, 45));
            item.shape.setPosition(15, yPos);
            
            // LIGHT THEME: Menu item colors
            if (item.isActive) {
                item.shape.setFillColor(sf::Color(240, 242, 255)); // Xanh nhạt khi active
            } else {
                item.shape.setFillColor(sf::Color(255, 255, 255)); // Trắng
            }

            item.text.setFont(font);
            item.text.setString(labels[i]);
            item.text.setCharacterSize(14);
            // Text màu đen/xám đậm
            item.text.setFillColor(sf::Color(60, 60, 60));
            item.text.setPosition(30, yPos + 14);

            menuItems.push_back(item);
            yPos += 52;
        }
    }

    void setUserRole(UserRole role, sf::Font& font) {
        currentRole = role;
        buildMenu(font);
    }

    void update(sf::Vector2f mousePos) {
        for (auto& item : menuItems) {
            if (item.shape.getGlobalBounds().contains(mousePos)) {
                if (!item.isActive) {
                    // LIGHT THEME: Hover effect
                    item.shape.setFillColor(sf::Color(235, 238, 250));
                }
            } else {
                if (!item.isActive) {
                    item.shape.setFillColor(sf::Color(255, 255, 255));
                }
            }
        }
    }

    int handleClick(sf::Vector2f mousePos) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (menuItems[i].shape.getGlobalBounds().contains(mousePos)) {
                // Reset all items
                for (auto& item : menuItems) {
                    item.isActive = false;
                    item.shape.setFillColor(sf::Color(255, 255, 255));
                }
                
                // Set clicked item as active
                menuItems[i].isActive = true;
                menuItems[i].shape.setFillColor(sf::Color(240, 242, 255));
                activeIndex = i;
                
                return i;
            }
        }
        return -1;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(logoText);
        
        for (const auto& item : menuItems) {
            window.draw(item.shape);
            window.draw(item.text);
        }
    }

    UserRole getUserRole() const { return currentRole; }
    
    std::string getMenuLabel(int index) const {
        if (index >= 0 && index < (int)menuItems.size()) {
            return menuItems[index].label;
        }
        return "";
    }
};

#endif// ============================================
// screens/SplashScreen.hpp - Màn hình khởi động
// ============================================
#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"

class SplashScreen {
private:
    sf::RectangleShape background;
    sf::RectangleShape card;
    sf::Text logoText;
    sf::Text titleText;
    sf::Text descText;
    Button* startButton;

public:
    SplashScreen(sf::Font& font) {
        background.setSize(sf::Vector2f(1400, 900));
        background.setFillColor(sf::Color(13, 15, 23));

        card.setSize(sf::Vector2f(600, 400));
        card.setPosition(400, 250);
        card.setFillColor(sf::Color(20, 23, 35));

        logoText.setFont(font);
        logoText.setString("Library");
        logoText.setCharacterSize(60);
        logoText.setFillColor(sf::Color(255, 193, 94));
        sf::FloatRect logoBounds = logoText.getLocalBounds();
        logoText.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
        logoText.setPosition(700, 350);

        titleText.setFont(font);
        titleText.setString("Quan Ly Thu Vien");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleText.setPosition(700, 430);

        descText.setFont(font);
        descText.setString("He thong quan ly sach nhanh chong va hien dai");
        descText.setCharacterSize(14);
        descText.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect descBounds = descText.getLocalBounds();
        descText.setOrigin(descBounds.width / 2, descBounds.height / 2);
        descText.setPosition(700, 480);

        startButton = new Button(sf::Vector2f(600, 530), sf::Vector2f(200, 50), 
                                "Xem Ngay", font);
    }

    ~SplashScreen() { delete startButton; }

    void update(sf::Vector2f mousePos) {
        startButton->update(mousePos);
    }

    bool handleClick(sf::Vector2f mousePos) {
        return startButton->handleClick(mousePos);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(card);
        window.draw(logoText);
        window.draw(titleText);
        window.draw(descText);
        startButton->draw(window);
    }
};

#endif#ifndef STATE_HPP
#define STATE_HPP

enum class AppState {
    SPLASH,
    HOME,
    CATEGORY,
    SEARCH,
    BOOK_DETAIL,
    LOGIN_CHOICE,
    LOGIN_READER,
    LOGIN_LIBRARIAN,
    REGISTER_READER,
    BORROWED_BOOKS,
    HISTORY,
    MANAGE_BOOKS,
    MANAGE_READERS,
    STATISTICS,
    OVERDUE_READERS  // Màn hình độc giả quá hạn
};

#endif#ifndef STATISTICS_SCREEN_HPP
#define STATISTICS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Sidebar.hpp"
#include "LibrarySystem.h"

class StatisticsScreen {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    Sidebar* sidebar;
    
    // Các thống kê
    sf::RectangleShape statBox1;
    sf::RectangleShape statBox2;
    sf::RectangleShape statBox3;
    sf::RectangleShape statBox4;
    
    sf::Text stat1Title;
    sf::Text stat1Value;
    sf::Text stat2Title;
    sf::Text stat2Value;
    sf::Text stat3Title;
    sf::Text stat3Value;
    sf::Text stat4Title;
    sf::Text stat4Value;
    
    LibrarySystem* libSystem;

public:
    StatisticsScreen(sf::Font& font, LibrarySystem* lib) : libSystem(lib) {
        background.setSize(sf::Vector2f(1150, 900));
        background.setPosition(250, 0);
        background.setFillColor(sf::Color(13, 15, 23));

        titleText.setFont(font);
        titleText.setString("Thong Ke Thu Vien");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 30);

        sidebar = new Sidebar(font);

        // Box 1: Tổng số sách
        statBox1.setSize(sf::Vector2f(450, 150));
        statBox1.setPosition(300, 120);
        statBox1.setFillColor(sf::Color(60, 100, 180));

        stat1Title.setFont(font);
        stat1Title.setString("Tong So Sach");
        stat1Title.setCharacterSize(18);
        stat1Title.setFillColor(sf::Color(200, 200, 200));
        stat1Title.setPosition(320, 140);

        stat1Value.setFont(font);
        stat1Value.setString("1,250");
        stat1Value.setCharacterSize(48);
        stat1Value.setFillColor(sf::Color::White);
        stat1Value.setPosition(320, 180);

        // Box 2: Tổng độc giả
        statBox2.setSize(sf::Vector2f(450, 150));
        statBox2.setPosition(780, 120);
        statBox2.setFillColor(sf::Color(100, 180, 100));

        stat2Title.setFont(font);
        stat2Title.setString("Tong Doc Gia");
        stat2Title.setCharacterSize(18);
        stat2Title.setFillColor(sf::Color(200, 200, 200));
        stat2Title.setPosition(800, 140);

        stat2Value.setFont(font);
        stat2Value.setString("567");
        stat2Value.setCharacterSize(48);
        stat2Value.setFillColor(sf::Color::White);
        stat2Value.setPosition(800, 180);

        // Box 3: Sách đang mượn
        statBox3.setSize(sf::Vector2f(450, 150));
        statBox3.setPosition(300, 300);
        statBox3.setFillColor(sf::Color(200, 150, 60));

        stat3Title.setFont(font);
        stat3Title.setString("Sach Dang Muon");
        stat3Title.setCharacterSize(18);
        stat3Title.setFillColor(sf::Color(200, 200, 200));
        stat3Title.setPosition(320, 320);

        stat3Value.setFont(font);
        stat3Value.setString("342");
        stat3Value.setCharacterSize(48);
        stat3Value.setFillColor(sf::Color::White);
        stat3Value.setPosition(320, 360);

        // Box 4: Sách quá hạn
        statBox4.setSize(sf::Vector2f(450, 150));
        statBox4.setPosition(780, 300);
        statBox4.setFillColor(sf::Color(200, 80, 80));

        stat4Title.setFont(font);
        stat4Title.setString("Sach Qua Han");
        stat4Title.setCharacterSize(18);
        stat4Title.setFillColor(sf::Color(200, 200, 200));
        stat4Title.setPosition(800, 320);

        stat4Value.setFont(font);
        stat4Value.setString("23");
        stat4Value.setCharacterSize(48);
        stat4Value.setFillColor(sf::Color::White);
        stat4Value.setPosition(800, 360);

        loadStatistics();
    }

    ~StatisticsScreen() {
        delete sidebar;
    }

    void loadStatistics() {
    if (!libSystem) return;
    int tongSoSach = 0;
    int tongSoLuong = 0;
    int tongDocGia = 0;
    int soSachDangMuon = 0;
    int soSachQuaHan = 0;

    // Đếm sách
    NodeBook* currentBook = libSystem->getDanhSachSach();
    while (currentBook != nullptr) {
        tongSoSach++;
        tongSoLuong += currentBook->data->getSoLuong();
        currentBook = currentBook->next;
    }

    // Đếm độc giả và sách đang mượn
    NodeReader* currentReader = libSystem->getDanhSachDocGia();
    while (currentReader != nullptr) {
        tongDocGia++;
        soSachDangMuon += currentReader->data->DemSachDaMuon();
        soSachQuaHan += currentReader->data->DemSachQuaHan();
        currentReader = currentReader->next;
    }

    // Cập nhật UI
    stat1Value.setString(std::to_string(tongSoSach));
    stat2Value.setString(std::to_string(tongDocGia));
    stat3Value.setString(std::to_string(soSachDangMuon));
    stat4Value.setString(std::to_string(soSachQuaHan));
}

    void update(sf::Vector2f mousePos) {
        sidebar->update(mousePos);
    }

    int handleSidebarClick(sf::Vector2f mousePos) {
        return sidebar->handleClick(mousePos);
    }

    void setUserRole(UserRole role, sf::Font& font) {
        sidebar->setUserRole(role, font);
    }

    void render(sf::RenderWindow& window) {
        window.draw(background);
        sidebar->draw(window);
        window.draw(titleText);

        window.draw(statBox1);
        window.draw(stat1Title);
        window.draw(stat1Value);

        window.draw(statBox2);
        window.draw(stat2Title);
        window.draw(stat2Value);

        window.draw(statBox3);
        window.draw(stat3Title);
        window.draw(stat3Value);

        window.draw(statBox4);
        window.draw(stat4Title);
        window.draw(stat4Value);
    }

    Sidebar* getSidebar() { return sidebar; }
};

#endif#ifndef TOAST_HPP
#define TOAST_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "MessageBox.hpp"

class Toast {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Clock clock;
    bool isVisible;
    float duration;
    float fadeTime;

public:
    Toast(sf::Font& font) {
        box.setSize(sf::Vector2f(400, 60));
        box.setPosition(500, 800);
        box.setFillColor(sf::Color(40, 40, 40, 230));

        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        isVisible = false;
        duration = 3.0f;
        fadeTime = 0.5f;
    }

    void show(const std::string& message, MessageType type = MessageType::INFO) {
        text.setString(message);
        
        // Căn giữa text
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(700, 820);

        // Đặt màu theo loại
        switch(type) {
            case MessageType::SUCCESS:
                box.setFillColor(sf::Color(40, 140, 40, 230));
                break;
            case MessageType::ERROR:
                box.setFillColor(sf::Color(180, 40, 40, 230));
                break;
            case MessageType::WARNING:
                box.setFillColor(sf::Color(200, 150, 40, 230));
                break;
            default:
                box.setFillColor(sf::Color(40, 40, 40, 230));
        }

        isVisible = true;
        clock.restart();
    }

    void update() {
        if (!isVisible) return;

        float elapsed = clock.getElapsedTime().asSeconds();
        
        if (elapsed > duration) {
            // Fade out
            float fadeProgress = (elapsed - duration) / fadeTime;
            if (fadeProgress >= 1.0f) {
                isVisible = false;
            } else {
                sf::Color boxColor = box.getFillColor();
                boxColor.a = 230 * (1.0f - fadeProgress);
                box.setFillColor(boxColor);
                
                sf::Color textColor = text.getFillColor();
                textColor.a = 255 * (1.0f - fadeProgress);
                text.setFillColor(textColor);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (isVisible) {
            window.draw(box);
            window.draw(text);
        }
    }

    bool isShown() const {
        return isVisible;
    }
};

#endif#include "Book.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Node.h"

string Sach::toCSV() const {
    ostringstream oss;
    oss << maSach << "|" << tenSach << "|" << tacGia << "|"
        << theLoai << "|" << namXuatBan << "|" << nhaXuatBan << "|"
        << soLuong;
    return oss.str();
}

void Sach::hienThiThongTin() const {
    cout << left << setw(15) << maSach
         <<  setw(25) << tenSach
         <<  setw(20) << tacGia
         <<  setw(15) << theLoai
         <<  setw(8)  << namXuatBan
         <<  setw(25) << nhaXuatBan
         <<  setw(13) << soLuong
         << fixed << setprecision(1) << getDiemTrungBinh()
         << "\n";
    
}

//Tạo đúng lớp con dựa trên thể loại
Sach* Sach::createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb) {
    Sach* s = nullptr;
    if (tl == "Giao trinh") s = new GiaoTrinh(ten, tg, tl, nam, nxb);
    else if (tl == "Tham khao") s = new ThamKhao(ten, tg, tl, nam, nxb);
    else if (tl == "Tieu thuyet") s = new TieuThuyet(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen ngan") s = new TruyenNgan(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen tranh") s = new TruyenTranh(ten, tg, tl, nam, nxb);
    else if (tl == "Sach ki nang") s = new SachKiNang(ten, tg, tl, nam, nxb);
    else s = new GiaoTrinh(ten, tg, tl, nam, nxb); // mặc định

    // Tự tạo mã ID (dựa prefix)
    static int stt = 1;
    ostringstream oss;
    oss << setw(5) << setfill('0') << stt;
    string id = s->prefix() + oss.str();
    s->setMaSach(id);
    stt++;

    return s;
    
}

void Sach::docFileInput(const string& fileName, NodeBook*& head) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cerr << " Khong mo duoc file: " << fileName << endl;
        return;
    }

    string line;
    NodeBook* tail = nullptr;
    while (getline(in, line)) {
        if (line.empty()) continue;
        string ten, tg, tl, nxb, namStr, soLuongStr, imagePath;
        stringstream ss(line);
        getline(ss, ten, '|');
        getline(ss, tg, '|');
        getline(ss, tl, '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');
        getline(ss, soLuongStr, '|');
        getline(ss, imagePath, '|');

        int nam = stoi(namStr);
        int sl = stoi(soLuongStr);
        Sach* s = Sach::createFromData(ten, tg, tl, nam, nxb);
        s->setSoLuong(sl);
        s->setMaSach(s->getMaSach()); // đảm bảo mã sách đúng

        NodeBook* newNode = new NodeBook(s);
        newNode->next = head;
        head = newNode;
    }
    in.close();
    cout << "Doc file thanh cong: " << fileName << endl;
}

void Sach::ghiFile(const string& fileName, NodeBook* head) {
    ofstream out(fileName);
    if (!out.is_open()) {
        cerr << " Khong mo duoc file de ghi: " << fileName << endl;
        return;
    }

    NodeBook* p = head;
    while (p) {
        out << p->data->toCSV() << endl;
        p = p->next;
    }

    out.close();
    cout << " Da ghi danh sach sach vao: " << fileName << endl;
}#include "Librarian.h"
#include "LibrarySystem.h"
#include "Book.h"
#include <iostream>
#include <iomanip>
using namespace std;

Librarian::Librarian() : USER() {
    vaiTro = UserRole::LIBRARIAN;
    chucVu = "Thu Thu";
}

Librarian::Librarian(string maID, string hoTen, string SDT, string Email, string username, string password, string chucVu)
    : USER(maID, hoTen, SDT, Email, username, password) {
    this->vaiTro = UserRole::LIBRARIAN;
    this->chucVu = chucVu;
}

void Librarian::SetThongTin(string maID,string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    this->vaiTro = UserRole::LIBRARIAN;
}

void Librarian::HienThiThongTin() const {
    USER::HienThiThongTin();
    cout << setw(15) << chucVu << endl;
}
void Librarian::ThemSach(LibrarySystem &L) {
    L.DocFileSach("ThemSach.txt");
    L.GhiFileSach("DanhSachSach.txt");
}

void Librarian::XoaSach(LibrarySystem &L) {
    string maSach;
    cout << "Nhap ma sach can xoa: ";
    getline(cin,maSach);
    L.XoaSach(maSach);
}

void Librarian::CapNhatThongTinSach(LibrarySystem &L) {
    L.CapNhatThongTinSach();
}

void Librarian::XemThongKe(LibrarySystem &L) {
    L.XemThongKe();
}#include "LibrarySystem.h"
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
    ofstream file(fileName, ios::app);
    
    if (!file.is_open()) {
        cout << "Khong the ghi file muon sach!\n";
        return;
    }

    NodeMuonSach* current = docGia->getDanhSachPhieuMuon();
    while (current != nullptr) {
        PhieuMuonSach* phieu = current->phieu;
        
        // Format: maSach|ngayMuon|ngayHetHan
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


void LibrarySystem::MuonSach(Reader* docGia, const string& maSach) {
    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            // Kiểm tra tình trạng sách
            if (current->data->getSoLuong() == 0) {
                cout << "Sach da het, khong the muon!\n";
                return;
            }

            // Kiểm tra đã mượn chưa
            if (docGia->DaMuonSach(maSach)) {
                cout << "Ban da muon sach nay roi.\n";
                return;
            }

            // Kiểm tra giới hạn
            if (docGia->DemSachDaMuon() >= docGia->getGioiHanMuon()) {
                cout << "Ban da muon toi da " << docGia->getGioiHanMuon() << " cuon.\n";
                return;
            }

            // Kiểm tra có sách quá hạn không
            int soQuaHan = docGia->DemSachQuaHan();
            if (soQuaHan > 0) {
                cout << "\033[1;31m✗ Ban co " << soQuaHan << " sach qua han!\033[0m\n";
                cout << "Vui long tra sach qua han truoc khi muon sach moi.\n";
                return;
            }

            // Cho phép mượn
            current->data->muonSach(); 

            // Tạo phiếu mượn với ngày mượn và hạn trả
            docGia->themPhieuMuonSach(current->data);
            docGia->ghiLichSu("Muon", current->data);

            // Lưu vào file
            GhiDanhSachMuonCuaDocGia(docGia);
            GhiFileHeThong("DanhSachSach.txt");

            cout << "\nMuon sach thanh cong: " << current->data->getTenSach() << endl;
            
            // Hiển thị thông tin phiếu mượn
            NodeMuonSach* phieuMoi = docGia->getDanhSachPhieuMuon();
            if (phieuMoi != nullptr) {
                PhieuMuonSach* p = phieuMoi->phieu;
                cout << "  Ma sach: " << p->sach->getMaSach() << endl;
                cout << "  Ngay muon: " << timeToString(p->ngayMuon) << endl;
                cout << "  Ngay tra: " << timeToString(p->ngayHetHan) << endl;
                cout << "  Han tra: 90 ngay (3 thang)" << endl;
            }
            
            return;
        }
        current = current->next;
    }

    cout << " Khong tim thay sach voi ma: " << maSach << endl;
}

void LibrarySystem::TraSach(Reader* docGia, const string& maSach) {
    if (!docGia->DaMuonSach(maSach)) {
        cout << " Ban chua muon sach co ma: " << maSach << endl;
        return;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {
            // Kiểm tra xem sách có quá hạn không
            NodeMuonSach* phieuNode = docGia->getDanhSachPhieuMuon();
            bool quaHan = false;
            int soNgayQuaHan = 0;
            
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

            // Tăng số lượng sách trong kho
            current->data->traSach();

            // Xóa phiếu mượn
            docGia->xoaPhieuMuonSach(maSach);
            docGia->ghiLichSu("Tra", current->data);

            cout << "\n Tra sach thanh cong: " << current->data->getTenSach() << endl;
            
            // Cảnh báo nếu quá hạn
            if (quaHan) {
                cout << "\033[1;31m⚠ CANH BAO: Sach tra qua han " << soNgayQuaHan << " ngay!\033[0m\n";
                cout << "Luu y: Ban nen tra sach dung han de tranh anh huong den viec muon sach sau nay.\n";
            }
            
            // Hỏi đánh giá
            int luaChon;
            cout << "\nBan co muon danh gia cuon sach vua tra khong? (1: Co, 0: Khong): ";
            cin >> luaChon;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (luaChon == 1) {
                DanhGiaSach(docGia, current->data);
            }

            // Lưu file
            GhiDanhSachMuonCuaDocGia(docGia);
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

    Reader* dg = new Reader();
    dg->SignUp(hoTen, sdt, email, user, pass);

    NodeReader* newNode = new NodeReader(dg);

    newNode->next = HeadDsDocGia;
    HeadDsDocGia = newNode;
    
    ofstream out("DocGia.txt", ios::app); // mở file ở chế độ ghi thêm
    if (out.is_open()) {
        out << dg->toCSV() << endl;
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
            if (current->data->Login(username, password)) {
                currentUser = current->data;
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
}#include "LichSuMuonTra.h"

StackLichSu::~StackLichSu() {
    while (top != nullptr) {
        NodeLichSu* temp = top;
        top = top->next;
        delete temp;
    }
}

void StackLichSu::push(const LichSuMuonTra& ls) {
    NodeLichSu* newNode = new NodeLichSu{ls, top};
    top = newNode;
}

bool StackLichSu::isEmpty() const {
    return top == nullptr;
}

void StackLichSu::hienThiLichSu() const {
    if (isEmpty()) {
        cout << "Ban chua co lich su muon tra nao.\n";
        return;
    }

    cout << "\nLich su muon - tra:\n";
    NodeLichSu* current = top;
    while (current != nullptr) {
        cout << current->data.hanhDong << " - " << current->data.tenSach
             << " (" << current->data.maSach << ") luc " << current->data.thoiGian << endl;
        current = current->next;
    }
}

#include "App.hpp"

int main() {
    try {
        App app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Loi: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}#include "Reader.h"
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

// Hàm chuyển đổi time_t sang chuỗi ngày
string timeString(time_t t) {
    tm* ltm = localtime(&t);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
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
    fileLichSu = "LichSu_" + maID + ".txt";
}

Reader::Reader(string ma, string hoTen, string sdt, string email, string username, string password) 
    : USER(ma, hoTen, sdt, email, username, password) {
    vaiTro = UserRole::READER;
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
}#include "USER.h"
#include <iomanip> 
#include <iostream>
using namespace std;


USER::USER() {
    maID = "";
    hoTen = "";
    SDT = "";
    Email = "";
    username = "";
    password = "";
    vaiTro = UserRole::READER;
}

USER::USER(string maID, string hoTen, string SDT, string Email, string username, string password) {
    this->maID = maID;
    this->hoTen = hoTen;
    this->SDT = SDT;
    this->Email = Email;
    this->username = username;
    this->password = password;
    this->trangThaiHoatDong = true;
    this->vaiTro = UserRole::READER;
}

USER::~USER() {}

void USER::HienThiThongTin() const {

    cout << left << setw(10) << maID 
                 << setw(25) << hoTen  
                 <<setw(20)  << SDT      
                 << setw(25) << Email    
                 << setw(15) << username;
}

void USER::CapNhatThongTin() {
    cout << "Cap nhat thong tin ca nhan: " << endl;
    cout << "Nhap ho ten moi: ";
    getline(cin, hoTen);
    cout << "Nhap SDT moi: ";
    getline(cin, SDT);
    cout << "Nhap email moi: ";
    getline(cin, Email);
    cout << "Thong tin da duoc cap nhat." << endl;
}

bool USER::Login(string username, string password) {
    return (this->username == username && this->password == password);
}

void USER::Logout() {
    cout << username << " da dang xuat khoi he thong." << endl;
}*/