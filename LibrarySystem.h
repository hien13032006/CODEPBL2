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

        void DocFileSach(const string& fileName);//them sach tu file
        void GhiFileSach(const string& fileName) const;//luu thong tin sach vua them vao file sau khi cap maSach
        void GhiFileHeThong(const string& fileName) const;
        void DocFileHeThong(const string& fileName);
        void DocFileDocGia();

        void XoaSach(const string &maSach);
        void CapNhatThongTinSach();
        void TimSach();
        void MuonSach(Reader* docGia, const string& maSach);
        void TraSach(Reader* docGia, const string& maSach);

        void DanhGiaSach(Reader* docGia, Sach* sach);
        double TinhDiemTrungBinhTuFile(const string& tenSach,const string& tacGia,int namXB,const string& nhaXB);
        void HienThiDanhSachSach() ;

        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        void DangKyDocGia();
        bool DangNhapDocGia(USER* &currentUser);
        bool DangNhapThuThu(const string &usernameInput, const string &passwordInput, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void HienThiTatCaDocGia() const;
        void XepHangSach();

};
        
#endif