#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "Book.h"
#include "USER.h"
#include "Librarian.h"
#include "Reader.h"

class LibrarySystem {
    private:
        NodeBook *HeadDsSach; //con tro den dau danh sach sach
        NodeReader *HeadDsDocGia; //con tro den dau danh sach doc gia
        NodeLibrarian *HeadDsTThu; //con tro den dau danh sach thu thu
    public:
        LibrarySystem();
        ~LibrarySystem();
        void DocFileSach(const string& fileName);
        void GhiFileSach(const string& fileName) const;
        void GhiFileHeThong(const string& fileName) const;
        void XoaSach(const string &maSach);
        void TimSach();
        void MuonSach(Reader* docGia, const string& maSach);
        void TraSach(Reader* docGia, const string& maSach);
        void HienThiDanhSachSach() const;
        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        void DangKyDocGia();
        void HienThiTatCaDocGia() const;
        bool DangNhapDocGia(const string &username, const string &password, USER* &currentUser);
        bool DangNhapThuThu(const string &username, const string &password, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void DocFileHeThong(const string& fileName);
        void DocFileDocGia();
};
#endif