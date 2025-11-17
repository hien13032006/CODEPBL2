#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

enum class UserRole {
    Librarian,
    Reader
};

class USER {
    protected:
        string maID;
        string hoTen;
        string SDT;
        string Email;
        string username;
        string password;
        UserRole vaiTro;
        bool trangThaiHoatDong;
    public:
        USER();
        USER(string maID, string hoTen, string SDT, string Email, string username, string password);
        virtual ~USER();

        virtual void HienThiThongTin() const;
        virtual void CapNhatThongTin();
        virtual UserRole getVaiTro() const { return vaiTro; }

        bool isLibrarian() const { return vaiTro == UserRole::Librarian; }
        bool isReader() const { return vaiTro == UserRole::Reader; }
        bool isActive() const { return trangThaiHoatDong; }

        bool Login(string username, string password);
        void Logout();

        string getMaID() const { return maID; }
        string getHoTen() const { return hoTen; }
        string getUsername() const { return username; }
        string getEmail() const { return Email; }
        string getSDT() const { return SDT; }
        string getPassword() const { return password; }
        string getTenVaiTro() const {
            return (vaiTro == UserRole::Librarian) ? "Librarian" : "Reader";
    }

        void setMaID(string newMaID) { maID = newMaID; }
        void setHoTen(string newHoTen) { hoTen = newHoTen; }
        void setSDT(string newSDT)  { SDT = newSDT; }
        void setEmail(string newEmail) { Email = newEmail; }
        void setUsername(string newUsername) { username = newUsername; }
        void setPassword(string newPassword) { password = newPassword; }
        void setTrangThai(bool active) { trangThaiHoatDong = active; }
};
#endif