#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "Book.h"

struct NodeMuonSach {
    Sach data; //thong tin sach da muon
    NodeMuonSach *next; //con tro den sach tiep theo
};

class Reader : public USER {
    private:
        static int readerCount;
        NodeMuonSach *HeadDsMuonSach; //con tro den dau danh sach sach da muon
    public:
        Reader();
        ~Reader();
        void SignUp(string hoTen, string SDT, string Email, string username, string password);
        bool SachDaMuon(const string maSach);
        void MuonSach(const Sach &book);
        void TraSach(const string &maSach);
        void HienThiSachDaMuon() const;
        void HienThiThongTin() const override;
};
#endif