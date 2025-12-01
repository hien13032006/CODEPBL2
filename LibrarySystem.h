#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "Book.h"
#include "USER.h"
#include "Librarian.h"
#include "Reader.h"
#include "Node.h"
#include <string>
#include <vector>

const int TABLE_SIZE = 1000;

struct HashNode {
    string key;
    NodeBook* list;
    HashNode* next;
};

struct NodeThongKe {
    std::string tenSach;
    std::string tacGia;
    int namXB;
    std::string nhaXB;
    double tongDiem;
    int soDanhGia;
    NodeThongKe* next;
    
    NodeThongKe(std::string ten, std::string tg, int nxb, std::string nxbx, double tong, int so)
        : tenSach(ten), tacGia(tg), namXB(nxb), nhaXB(nxbx), tongDiem(tong), soDanhGia(so), next(nullptr) {}
    
    double diemTB() const {
        return soDanhGia == 0 ? 0 : tongDiem / soDanhGia;
    }
};

class LibrarySystem {
    private:
        NodeBook *HeadDsSach; 
        NodeReader *HeadDsDocGia; 
        NodeLibrarian *HeadDsTThu; 
        HashNode* hashTable[TABLE_SIZE];
        std::string top10BookIDs[10]; 
        int top10Count = 0; 

    public:
        LibrarySystem();
        ~LibrarySystem();

        int hashFunction(const string& s);

        void DocFileSach(const string& fileName);
        void GhiFileSach(const string& fileName) const;
        void GhiFileHeThong(const string& fileName) const;
        void DocFileHeThong(const string& fileName);
        void DocFileDocGia();
        void DocDanhSachMuonCuaDocGia(Reader* docGia);
        void GhiDanhSachMuonCuaDocGia(Reader* docGia);

        void XoaSach(const string &maSach);
        void CapNhatThongTinSach();
        void TimSach(const string& keyword);
        void BuildHashTable();
        void MuonSach(Reader* docGia, const string& maSach);
        void TraSach(Reader* docGia, const string& maSach);
        void DanhGiaSach(Reader* docGia, Sach* sach, int diem);
        
        bool KiemTraDaDanhGia(const string& maDocGia, const string& maSach);
        int LayDiemDanhGia(const string& maDocGia, const string& maSach);

        double TinhDiemTrungBinhTuFile(const string& tenSach,const string& tacGia,int namXB,const string& nhaXB);
        void HienThiDanhSachSach() ;
        void HienThiDocGiaQuaHan();
        void ThongKeSachQuaHan();

        bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
        void DangKyDocGia();
        bool DangNhapDocGia(USER* &currentUser);
        bool DangNhapThuThu(const string &usernameInput, const string &passwordInput, USER* &currentUser);
        bool DangXuat(USER* &currentUser);
        void HienThiTatCaDocGia() const;
        void XepHangSach();

        NodeBook* getDanhSachSach() { return HeadDsSach; }
        NodeReader* getDanhSachDocGia() { return HeadDsDocGia; }

        void DocTatCaDanhSachMuon();
        void XemThongKe();
        int DemTongSachDangMuon(const string& maSach);

        NodeBorrowerInfo* TimNguoiMuonSach(const std::string& maSach) const;
        int getTop10Count() const { return top10Count; }
        std::string getTop10ID(int index) const { 
            if (index >= 0 && index < top10Count) return top10BookIDs[index];
            return "";
        }

        void ThemSachMoi(Sach* sachMoi);
};
        
#endif