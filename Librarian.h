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

        string getChucVu() const {return chucVu;}
};
#endif