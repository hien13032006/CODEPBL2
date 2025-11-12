#pragma once
#include <string> 
#include "USER.h"   
using namespace std;
class LibrarySystem;

class Librarian : public USER {
    public:
        void SetThongTin(string maID, string hoTen, string SDT, string Email, string username, string password);
        void XoaSach(LibrarySystem &L);
        void CapNhatSach(); 
};
