#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "LichSuMuonTra.h"
#include <string>
#include <ctime>

class Sach;

struct NodeMuonSach {
    Sach* data; //thong tin sach da muon
    string ngayMuon;
    NodeMuonSach *next; //con tro den sach tiep theo
};

class Reader : public USER {
    private:
        time_t ngayDangKy;
        int gioiHanSachMuon;
        static int readerCount;
        NodeMuonSach *HeadDsMuonSach; //con tro den dau danh sach sach da muon
        StackLichSu LichSu;
        string fileLichSu;
    public:
        Reader();
        Reader(string ma, string hoTen, string sdt, string email, string username, string password);
        ~Reader();

        void HienThiThongTin() const override;
        UserRole getVaiTro() const override { return UserRole::READER; }

        void SignUp(string hoTen, string SDT, string Email, string username, string password);

        bool coTheMuonSach() const;
        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        void themSachDaMuon(const Sach* s);// them sach da muon vao NodeMuonSach
        void xoaSachDaMuon(const string& maSach);// xoa sach vua tra ra khoi NodeMuonSach
        void HienThiSachDangMuon();

        void docFileSachDangMuonTuFile();
        void ghiLichSu(const string& hanhDong, const Sach* s);
        void HienThiLichSuMuonTra() const;

        string toCSV() const;
        
        time_t getNgayDangKy() const { return ngayDangKy; }
        int getGioiHanSachMuon() const { return gioiHanSachMuon; }
        string getFileLichSu() const { return fileLichSu; }

        static void setReaderCount(int n) { readerCount = n; }
        static int getReaderCount() { return readerCount; }
};
#endif