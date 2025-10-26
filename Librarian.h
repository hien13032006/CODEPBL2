#ifndef Librarian_h
#define Librarian_h
#include "USER.h"

class LibrarySystem;

class Librarian : public USER {
    public:
        void SetThongTin(string maID, string hoTen, string SDT, string Email, string username, string password);
        void HienThiThongTin() const override;
        void ThemSach(LibrarySystem &L);
        void XoaSach(LibrarySystem &L);
        void CapNhatSach(); 
};
#endif