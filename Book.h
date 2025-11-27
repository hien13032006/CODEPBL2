#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <iostream>
#include <sstream>
#include "Node.h" // Đảm bảo đã có file này định nghĩa NodeBook
using namespace std;

// [MỚI] Struct lưu thông tin đếm cho mảng tĩnh
struct TypeCounter {
    string prefix; 
    int count;     
};

class Sach {
protected:
    string maSach;
    string tenSach;
    string tacGia;
    string theLoai;
    int namXuatBan;
    string nhaXuatBan;
    int soLuong;
    string imagePath; 
    double tongDiemDanhGia = 0;
    int soLuotDanhGia = 0;

    // [MỚI] Dùng mảng tĩnh để đếm số lượng từng loại thay vì vector/map
    static const int MAX_TYPES = 50; // Giới hạn 50 loại sách
    static TypeCounter typeCounts[MAX_TYPES]; 
    static int currentTypeSize; // Biến đếm số lượng loại hiện có

public:
    Sach() : namXuatBan(0), soLuong(0) {}
    Sach(string ten, string tg, string tl, int nam, string nxb, int sl = 0)
        : tenSach(ten), tacGia(tg), theLoai(tl), namXuatBan(nam), nhaXuatBan(nxb), soLuong(sl) {}

    virtual ~Sach() = default;

    // Các phương thức ảo
    virtual string prefix() const = 0;     // Mã loại, ví dụ: "GT", "TT"
    virtual Sach* clone() const = 0;       // Tạo bản sao 
    virtual string toCSV() const;          // Xuất ra file
    virtual void hienThiThongTin() const;

    bool muonSach() {
        if (soLuong > 0) {
            soLuong--;
            return true;
        }
        return false; 
    }
    void traSach() {
        soLuong++;
    }

    void themDanhGia(int diem) {
        tongDiemDanhGia += diem;
        soLuotDanhGia++;
    }

    // Getter / Setter
    string getMaSach() const { return maSach; }
    string getTenSach() const { return tenSach; }
    string getTacGia() const { return tacGia; }
    string getTheLoai() const { return theLoai; }
    int getNamXuatBan() const { return namXuatBan; }
    string getNhaXuatBan() const { return nhaXuatBan; }
    int getSoLuong() const { return soLuong; }
    double getTongDiem() const { return tongDiemDanhGia; }
    int getSoDanhGia() const { return soLuotDanhGia; }
    double getDiemTrungBinh() const {
        return soLuotDanhGia == 0 ? 0 : tongDiemDanhGia / soLuotDanhGia;
    }
    string getImagePath() const { return imagePath; }

    void setMaSach(string ma) { maSach = ma; }
    void setTenSach(string ten) { tenSach = ten; }
    void setTacGia(string tg) { tacGia = tg; }
    void setTheLoai(string tl) { theLoai = tl; }
    void setNamXuatBan(int nxb) { namXuatBan = nxb; }
    void setNhaXuatBan(string nhxb) { nhaXuatBan = nhxb; }
    void setSoLuong(int sl) { soLuong = sl; }
    void setImagePath(string path) { imagePath = path; }

    void setDanhGia(double tong, int so) {
        tongDiemDanhGia = tong;
        soLuotDanhGia = so;
    }
    void themDanhGia(int diem) {
        tongDiemDanhGia += diem;
        soLuotDanhGia++;
    }
    void suaDanhGia(int diemCu, int diemMoi) {
        tongDiemDanhGia = tongDiemDanhGia - diemCu + diemMoi;
        // Số lượt đánh giá không đổi
    }
    // [MỚI] Hàm đồng bộ bộ đếm sau khi đọc file
    static void resyncCounters(NodeBook* head);

    static Sach* createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb);
    static void docFileInput(const string& fileName, NodeBook*& head);
    static void ghiFile(const string& fileName, NodeBook* head);
};

// --- Các lớp con ---
class GiaoTrinh : public Sach {
public:
    GiaoTrinh() {}
    GiaoTrinh(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "GT"; }
    Sach* clone() const override { return new GiaoTrinh(*this); }
};

class ThamKhao : public Sach {
public:
    ThamKhao() {}
    ThamKhao(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "TK"; }
    Sach* clone() const override { return new ThamKhao(*this); }
};

class TieuThuyet : public Sach {
public:
    TieuThuyet() {}
    TieuThuyet(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "TT"; }
    Sach* clone() const override { return new TieuThuyet(*this); }
};

class TruyenNgan : public Sach {
public:
    TruyenNgan(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "TN";}
    Sach* clone() const override { return new TruyenNgan(*this); }
};

class TapChi : public Sach {
public:
    TapChi(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "TC";}
    Sach* clone() const override { return new TapChi(*this); }
};

class TruyenTranh : public Sach {
public:
    TruyenTranh(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "TTR";}
    Sach* clone() const override { return new TruyenTranh(*this); }
};

class SachKiNang : public Sach {
public:
    SachKiNang(string ten, string tg, string tl, int nam, string nxb)
        : Sach(ten, tg, tl, nam, nxb) {}
    string prefix() const override { return "KN";}
    Sach* clone() const override { return new SachKiNang(*this); }
};

#endif