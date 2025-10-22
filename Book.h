#ifndef Book_H
#define Book_H

#include <iostream>
#include <string>

using namespace std;

class Sach {
    protected:
        string maSach;
        string tenSach;
        string tacGia;
        string nhaXuatBan;
        int namXuatBan;
        int soLuong;
    public:
        Sach();
        virtual ~Sach();
        virtual void nhapThongTin();
        virtual void hienThiThongTin() const;
        const string& getMaSach() const;
        const string& getTenSach() const;
        const string& getTacGia() const;
        const string& getNhaXuatBan() const;
        int getNamXuatBan() const;
        int getSoLuong() const;

        void setMaSach(const string& m);
        void setTenSach(const string& t);
        void setTacGia(const string& tg);
        void setNhaXuatBan(const string& nxb);
        void setNamXuatBan(int n);
        void setSoLuong(int s);

        virtual string prefix() const = 0;

        void setautoMaSach(int id);
        string tocsv() const;
};

class GiaoTrinh : public Sach {
    public:
        GiaoTrinh();
        string prefix() const override;
};

class ThamKhao : public Sach {
    public:
        ThamKhao();
        string prefix() const override;
};

class TieuThuyet : public Sach {
    public:
        TieuThuyet();
        string prefix() const override;
};

class TruyenNgan : public Sach {
    public:
        TruyenNgan();
        string prefix() const override;
};

class TapChi : public Sach {
    public:
        TapChi();
        string prefix() const override;
};

class TruyenTranh : public Sach {
    public:
        TruyenTranh();
        string prefix() const override;
};

class SachKiNang : public Sach {
    public:
        SachKiNang();
        string prefix() const override;
};
#endif
