#ifndef Reader_H
#define Reader_H
#include "USER.h"
#include <iostream>
using namespace std;

class Reader : public USER {
    private:
        static int readerCount;
    public:
        READER();
        void SignUp(string hoTen, string SDT, string Email, string username, string password);
        void HienThiThongTin() const override;
        void CapNhatThongTin() override;
        void muonSach();
        void traSach();
        void HienThiSachDaMuon() const;
};
#endif