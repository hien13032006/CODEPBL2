#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "Book.h"
#include "LichSuMuonTra.h"

struct NodeMuonSach {
    Sach data; //thong tin sach da muon
    NodeMuonSach *next; //con tro den sach tiep theo
};

class Reader : public USER {
    private:
        static int readerCount;
        NodeMuonSach *HeadDsMuonSach; //con tro den dau danh sach sach da muon
        StackLichSu LichSu;
    public:
        Reader();
        ~Reader();
        void SignUp(string hoTen, string SDT, string Email, string username, string password);
        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        void themSachDaMuon(const Sach& s);
        void xoaSachDaMuon(const string& maSach);
        void HienThiSachDaMuon() const;
        void ghiLichSu(const string& hanhDong, const Sach& s);
        void HienThiThongTin() const override;
        string toCSV() const;
        void HienThiLichSuMuonTra() const
};
#endif