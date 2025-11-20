#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

enum class UserRole {
    LIBRARIAN,
    READER,
    NONE
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
    public:
        USER();
        USER(string maID, string hoTen, string SDT, string Email, string username, string password);
        virtual ~USER();

        virtual void HienThiThongTin() const;
        virtual void CapNhatThongTin();
        virtual UserRole getVaiTro() const { return vaiTro; }

        bool isLibrarian() const { return vaiTro == UserRole::LIBRARIAN; }
        bool isReader() const { return vaiTro == UserRole::READER; }

        bool Login(string username, string password);
        void Logout();

        string getMaID() const { return maID; }
        string getHoTen() const { return hoTen; }
        string getUsername() const { return username; }
        string getEmail() const { return Email; }
        string getSDT() const { return SDT; }
        string getPassword() const { return password; }
        string getTenVaiTro() const {
            return (vaiTro == UserRole::LIBRARIAN) ? "Librarian" : "Reader";
        }

        virtual string toCSV() const;


        void setMaID(string newMaID) { maID = newMaID; }
        void setHoTen(string newHoTen) { hoTen = newHoTen; }
        void setSDT(string newSDT)  { SDT = newSDT; }
        void setEmail(string newEmail) { Email = newEmail; }
        void setUsername(string newUsername) { username = newUsername; }
        void setPassword(string newPassword) { password = newPassword; }
};
#endif