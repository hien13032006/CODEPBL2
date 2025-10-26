
#ifndef LICHSUMUONTRA_H
#define LICHSUMUONTRA_H

#include <ctime>
#include <string>
#include <iostream>
using namespace std;

struct LichSuMuonTra {
    string hanhDong; //muon hoac tra 
    string maSach;
    string tenSach;
    string thoiGian;
    string maDocGia;

    string toCSV() const {
        return hanhDong + "|" + maDocGia + "|" + maSach + "|" + tenSach + "|" + thoiGian;
}

};

struct NodeLichSu {
    LichSuMuonTra data;
    NodeLichSu* next;
};


class StackLichSu {
    private:
        NodeLichSu* top;
    public:
        StackLichSu() : top(nullptr) {}
        ~StackLichSu();
        void push(const LichSuMuonTra& ls);
        bool isEmpty() const;
        void hienThiLichSu() const;
};

#endif