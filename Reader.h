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
        Reader(string ma, string hoTen, string sdt, string email, string username, string password);
        ~Reader();

        void SignUp(string hoTen, string SDT, string Email, string username, string password);

        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        void themSachDaMuon(const Sach* s);// them sach da muon vao NodeMuonSach
        void xoaSachDaMuon(const string& maSach);// xoa sach vua tra ra khoi NodeMuonSach
        void HienThiSachDaMuon() const;

        void ghiLichSu(const string& hanhDong, const Sach* s);
        void HienThiThongTin() const override;
        void HienThiLichSuMuonTra() const;

        string toCSV() const;
        
        void setMaReader(const string& ma) { maID = ma; }
        void setHoTen(const string& hoten) { hoTen = hoten; }
        void setSDT(const string& sdt) { SDT = sdt; }
        void setEmail(const string& email) { Email = email; }
        void setUsername(const string& user) { username = user; }
        void setPassword(const string& pass) { password = pass; }
        static void setReaderCount(int n) { readerCount = n; }
        static int getReaderCount() { return readerCount; }
};
#endif