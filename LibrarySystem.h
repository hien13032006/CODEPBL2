#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "Book.h"
#include "USER.h"
#include "Librarian.h"
#include "Reader.h"
#include "Node.h"

class LibrarySystem {
    private:
        NodeBook *HeadDsSach; //con tro den dau danh sach sach
        NodeReader *HeadDsDocGia; //con tro den dau danh sach doc gia
        NodeLibrarian *HeadDsTThu; //con tro den dau danh sach thu thu
    public:
        LibrarySystem();
        ~LibrarySystem();
        void ThemSach(Sach &book);
        void XoaSach(const string &maSach);
        void TimSach();
        void HienThiDanhSachSach() const;
        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        void DangKyDocGia();
        void HienThiTatCaDocGia() const;
        bool KiemTraThuThuDaDangKy(const string& tenDangNhap) const;
        void DangKyThuThu();
        bool DangNhapDocGia(const string &username, const string &password, USER* &currentUser);
        bool DangNhapThuThu(const string &username, const string &password, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void LuuDuLieu() const;
};
#endif