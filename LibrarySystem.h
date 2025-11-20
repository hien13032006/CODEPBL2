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

enum class BorrowStatus {
    SUCCESS,
    OUT_OF_STOCK,
    ALREADY_BORROWED,
    LIMIT_REACHED,
    OVERDUE,
    NOT_FOUND
};

struct BorrowResult {
    BorrowStatus status;
    Sach* book;
    std::string borrowDate;
    std::string returnDate;
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
        BorrowResult MuonSach(Reader* docGia, const string& maSach);
        Sach* TraSach(Reader* docGia, const string& maSach, bool &quaHan, int &soNgayQuaHan);

        void DanhGiaSach(Reader* docGia, Sach* sach, int diem, const string& text);
        double TinhDiemTrungBinhTuFile(const string& tenSach,const string& tacGia,int namXB,const string& nhaXB);
        void HienThiDanhSachSach() ;
        void HienThiDocGiaQuaHan();
        void ThongKeSachQuaHan();

        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        bool DangKyDocGia(const string &hoTen, const string &sdt, const string &email, const string &user,const string &pass);
        bool DangNhapDocGia(const string &username, const string &password, USER* &currentUser);
        bool DangNhapThuThu(const string &username, const string &password, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void HienThiTatCaDocGia() const;
        void XepHangSach();

        NodeBook* getDanhSachSach() { return HeadDsSach; }
        NodeReader* getDanhSachDocGia() { return HeadDsDocGia; }

        void DocTatCaDanhSachMuon();
        void XemThongKe();

        Sach* TimSachTheoMa(const std::string& maSach);

};
        
#endif