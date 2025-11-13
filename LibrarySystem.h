#pragma once
#include <string>   
#include <fstream>
#include "Book.h" 
using namespace std;

// ========== cây tìm kiếm ==========
struct TreeNode {
    Sach* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Sach* s) : data(s), left(nullptr), right(nullptr) {}
};


class LibrarySystem {
private:
    NodeBook*      HeadDsSach;
    NodeReader*    HeadDsDocGia;
    NodeLibrarian* HeadDsTThu;

    // tree
    TreeNode *rootMaSach;
    TreeNode *rootTenSach;
    TreeNode *rootTacGia;
    TreeNode *rootTheLoai;
    TreeNode *rootNamXB;

    // Helper tree
    TreeNode* insertByKey(TreeNode* root, Sach* s, const string& key);
    TreeNode* insertByIntKey(TreeNode* root, Sach* s, int key);

    void inOrderSearchKey(TreeNode* root, const string& key, int tieuChi, bool &found) const;
    void inOrderSearchInt(TreeNode* root, int key, bool &found) const;

public:

    LibrarySystem();
    ~LibrarySystem();
    NodeReader* getReaderHead() const { return HeadDsDocGia; }

    void XayDungTatCaCay();
    bool deleteBook(const string& id);
    bool updateBook(const string& id, string tenMoi, string tacGiaMoi, int namMoi, int soLuongMoi);
    Sach* findBookByName(const string& name);
    Sach* findBookByID(const string& id);

    // ==============================
    void DocFileSach(const string& fileName);
    void GhiFileSach(const string& fileName) const;
    void GhiFileHeThong(const string& fileName) const;
    void DocFileHeThong(const string& fileName);
    void DocFileDocGia();
    void addBook(Sach* s);
    bool XoaSach(const string &maSach);
    void CapNhatThongTinSach();
    void TimSach();
    void HienThiDanhSachSach();

    void MuonSach(Reader* docGia, const string& maSach);
    void TraSach(Reader* docGia, const string& maSach);

    void DanhGiaSach(Reader* docGia, Sach* sach);
    double TinhDiemTrungBinhTuFile(const string& tenSach,const string& tacGia,int namXB,const string& nhaXB);
    void XepHangSach();

    NodeBook* getHeadBook() const { return HeadDsSach; }
    NodeBook* getHeadBook()       { return HeadDsSach; }

    bool usernameExist(const std::string& user) {
        for (NodeReader* p = HeadDsDocGia; p; p = p->next)
            if (p->data->getUsername() == user)
                return true;
        return false;
    }

    bool phoneExist(const std::string& sdt) {
        for (NodeReader* p = HeadDsDocGia; p; p = p->next)
            if (p->data->getSDT() == sdt)
                return true;
        return false;
    }

    bool emailExist(const std::string& email) {
        for (NodeReader* p = HeadDsDocGia; p; p = p->next)
            if (p->data->getEmail() == email)
                return true;
        return false;
    }

    // --- ADD READER ---
    bool addReader(Reader* r) {
        NodeReader* node = new NodeReader(r);
        node->next = HeadDsDocGia;
        HeadDsDocGia = node;

        ofstream out("Reader.txt", ios::app);
        if (out.is_open()) {
            out << r->toCSV() << "\n";
            out.close();
            return true;
        }
        return false;
    }

    // --- SAVE ALL READER ---
    void updateReaderFile() {
        ofstream out("Reader.txt", ios::trunc);
        NodeReader* p = HeadDsDocGia;
        while (p) {
            out << p->data->toCSV() << "\n";
            p = p->next;
        }
        out.close();
    }

    // --- LOGIN ---
    Reader* loginReader(string user, string pass) {
        for (NodeReader* p = HeadDsDocGia; p; p = p->next)
            if (p->data->getUsername() == user &&
                p->data->getPassword() == pass)
                return p->data;

        return nullptr;
    }

    // --- DELETE ---
    bool deleteReader(string user);

    Reader* getReaderByIndex(int idx);

    int exportReaderList(string arr[]); 
    bool KiemTraDocGiaDaDangKy(const string& tenDangNhap) const;
    bool DangKyDocGia(const string &hoTen, const string &sdt, const string &email, const string &user, const string &pass);
    Reader* DangNhapDocGia(const string& username, const string& password);
    Librarian* DangNhapThuThu(const string &usernameInput,const string &passwordInput);
    bool DangXuat(USER* &currentUser);
    void HienThiTatCaDocGia() const;


};
