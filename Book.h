#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <iostream>
#include <sstream>
#include "Node.h"
using namespace std;

class Sach {
protected:
    string maSach;
    string tenSach;
    string tacGia;
    string theLoai;
    int namXuatBan;
    string nhaXuatBan;
    int soLuong;
    string imagePath; // Đường dẫn đến ảnh bìa sách
    double tongDiemDanhGia = 0;
    int soLuotDanhGia = 0;

public:
    Sach() : namXuatBan(0), soLuong(0) {}
    Sach(string ten, string tg, string tl, int nam, string nxb, int sl = 0)
        : tenSach(ten), tacGia(tg), theLoai(tl), namXuatBan(nam), nhaXuatBan(nxb), soLuong(sl) {}

    virtual ~Sach() = default;

    // Các phương thức ảo
    virtual string prefix() const = 0;     // Mã loại, ví dụ: "GT", "TT"
    virtual Sach* clone() const = 0;       // Tạo bản sao (dùng khi thêm vào danh sách)
    virtual string toCSV() const;          // Xuất ra file
    virtual void hienThiThongTin() const;

    bool muonSach() {
        if (soLuong > 0) {
            soLuong--;
            return true;
        }
        return false; // hết sách
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

    void setMaSach(string ma) { maSach = ma; }
    void setTenSach(string ten) { tenSach = ten; }
    void setTacGia(string tg) { tacGia = tg; }
    void setTheLoai(string tl) { theLoai = tl; }
    void setNamXuatBan(int nxb) { namXuatBan = nxb; }
    void setNhaXuatBan(string nhxb) { nhaXuatBan = nhxb; }
    void setSoLuong(int sl) { soLuong = sl; }


    void setDanhGia(double tong, int so) {
        tongDiemDanhGia = tong;
        soLuotDanhGia = so;
    }

    // Factory method: tạo lớp con phù hợp từ dữ liệu file
    static Sach* createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb);
    static void docFileInput(const string& fileName, NodeBook*& head);
    static void ghiFile(const string& fileName, NodeBook* head);
};

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
