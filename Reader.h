#ifndef Reader_h
#define Reader_h
#include "USER.h"
#include "LichSuMuonTra.h"
#include "Node.h"
#include <string>
#include <ctime>

class Sach;

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
        Reader(string ma, string hoTen, string sdt, string email, string username, string password, time_t ngaydk);
        ~Reader();

        void HienThiThongTin() const override;
        UserRole getVaiTro() const override { return UserRole::READER; }

        void SignUp(string hoTen, string SDT, string Email, string username, string password);

        bool coTheMuonSach() const;
        bool DaMuonSach(const string maSach) const;
        int DemSachDaMuon() const;
        int getSoSachConCoTheMuon() const;
        
        void themPhieuMuonSach(const Sach* s);  //Tạo phiếu mượn
        void themPhieuMuonSachTuFile(const Sach* s, time_t ngayMuon, time_t ngayHetHan);
        void xoaPhieuMuonSach(const string& maSach); //Xóa phiếu mượn khi trả sách

        void HienThiSachDangMuon() const;
        int DemSachQuaHan() const; 

        void docFileSachDangMuonTuFile();
        void ghiLichSu(const string& hanhDong, const Sach* s);
        void HienThiLichSuMuonTra() const;

        string toCSV() const;
        
        time_t getNgayDangKy() const { return ngayDangKy; }
        int getGioiHanMuon() const { return gioiHanSachMuon; }
        string getFileLichSu() const { return fileLichSu; }
        NodeMuonSach* getDanhSachPhieuMuon() const { return HeadDsMuonSach; } 
        void setDanhSachPhieuMuon(NodeMuonSach* head) {  HeadDsMuonSach = head;}
        void setGioiHanMuon(int limit) { gioiHanSachMuon = limit; }
        static void setReaderCount(int n) { readerCount = n; }
        static int getReaderCount() { return readerCount; }
};
#endif