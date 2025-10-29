#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "LichSuMuonTra.h"

class Sach;

struct NodeMuonSach {
    Sach* data; //thong tin sach da muon
    NodeMuonSach *next; //con tro den sach tiep theo
};

class Reader : public USER {
    private:
        static int readerCount;
        NodeMuonSach *HeadDsMuonSach; //con tro den dau danh sach sach da muon
        StackLichSu LichSu;
        string fileLichSu;
    public:
        Reader();
        ~Reader();
        void SignUp(string hoTen, string SDT, string Email, string username, string password);
        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        void themSachDaMuon(const Sach* s);
        void xoaSachDaMuon(const string& maSach);
        void HienThiSachDaMuon() const;
        void ghiLichSu(const string& hanhDong, const Sach* s);
        void HienThiThongTin() const override;
        string toCSV() const;
        void HienThiLichSuMuonTra() const;
        void setMaReader(const string& ma) { maID = ma; }
        void setHoTen(const string& hoten) { hoTen = hoten; }
        void setSDT(const string& sdt) { SDT = sdt; }
        void setEmail(const string& email) { Email = email; }
        void setUsername(const string& user) { username = user; }
        void setPassword(const string& pass) { password = pass; }
};
#endif