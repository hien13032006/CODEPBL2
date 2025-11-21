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
    string key;        // từ khóa
    NodeBook* list;    // danh sách các sách có chứa từ khóa
    HashNode* next;
    
};


class LibrarySystem {
    private:
        NodeBook *HeadDsSach; //con tro den dau danh sach sach
        NodeReader *HeadDsDocGia; //con tro den dau danh sach doc gia
        NodeLibrarian *HeadDsTThu; //con tro den dau danh sach thu thu
        HashNode* hashTable[TABLE_SIZE];
        std::vector<std::string> top10BookIDs;
    public:
        LibrarySystem();
        ~LibrarySystem();

        int hashFunction(const string& s);

        void DocFileSach(const string& fileName);//them sach tu file
        void GhiFileSach(const string& fileName) const;//luu thong tin sach vua them vao file sau khi cap maSach
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

        void DanhGiaSach(Reader* docGia, Sach* sach);
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
        std::vector<BorrowerInfo> TimNguoiMuonSach(const std::string& maSach) const;
        const std::vector<std::string>& getTop10IDs() const { return top10BookIDs; }
};
struct NodeThongKe {
    std::string tenSach;
    std::string tacGia;
    int namXB;
    std::string nhaXB;
    double tongDiem;
    int soDanhGia;
    NodeThongKe* next;
    
    // Thêm constructor để dễ dàng khởi tạo
    NodeThongKe(std::string ten, std::string tg, int nxb, std::string nxbx, double tong, int so)
        : tenSach(ten), tacGia(tg), namXB(nxb), nhaXB(nxbx), tongDiem(tong), soDanhGia(so), next(nullptr) {}
    
    double diemTB() const {
        return soDanhGia == 0 ? 0 : tongDiem / soDanhGia;
    }
};
        
#endif