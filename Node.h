#ifndef Node_h
#define Node_h
#include "Librarian.h"
#include <ctime>
#include <string>

class Sach;
class Reader;
inline std::string timeString(time_t t) {
    tm* ltm = localtime(&t);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return std::string(buffer);
}

struct NodeBook {
    Sach* data;
    NodeBook *next;

    NodeBook(Sach *s) {
        data = s;
        next = nullptr;
    }

};

struct NodeReader {
    Reader* data;
    NodeReader* next;

    NodeReader(Reader* r) {
        data = r;
        next = nullptr;
    }
};
struct NodeLibrarian {
    Librarian data;
    NodeLibrarian *next;

    NodeLibrarian(const Librarian& l) {
        data = l;
        next = nullptr;
    }
};

struct BorrowerInfo {
    std::string maDocGia;
    std::string tenDocGia;
    time_t ngayMuon;
    time_t ngayHetHan;
    std::string trangThai; // Ví dụ: "QUA HAN 5 NGAY"
    bool daQuaHan;
};

struct PhieuMuonSach {
    Sach* sach;
    time_t ngayMuon;      // Thời gian mượn
    time_t ngayHetHan;    // Thời gian hết hạn (3 tháng sau ngày mượn)
    
    PhieuMuonSach(Sach* s) : sach(s) {
        ngayMuon = time(nullptr);  // Lấy thời gian hiện tại
        
        // Tính ngày hết hạn: 3 tháng = 90 ngày
        ngayHetHan = ngayMuon + (90 * 24 * 60 * 60);
    }

    // Constructor khi đọc từ file (có ngày mượn sẵn)
    PhieuMuonSach(Sach* s, time_t ngayM, time_t ngayHH) 
        : sach(s), ngayMuon(ngayM), ngayHetHan(ngayHH) {}
    
    // Tính số ngày còn lại
    int soNgayConLai() const {
        time_t now = time(nullptr);
        double seconds = difftime(ngayHetHan, now);
        return (int)(seconds / (24 * 60 * 60));
    }
    
    // Kiểm tra đã quá hạn chưa
    bool daQuaHan() const {
        return soNgayConLai() < 0;
    }
    
    // Lấy chuỗi trạng thái
    string trangThaiHan() const {
        if (daQuaHan()) {
            return "QUA HAN";
        } else {
            int ngay = soNgayConLai();
            return to_string(ngay) + " ngay";
        }
    }
};

struct NodeMuonSach {
    PhieuMuonSach* phieu;
    NodeMuonSach* next;
    
    NodeMuonSach(PhieuMuonSach* p) : phieu(p), next(nullptr) {}
};


#endif