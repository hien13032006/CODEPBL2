#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

class USER {
    protected:
        string maID;
        string hoTen;
        string SDT;
        string Email;
        string username;
        string password;
    public:
        USER();
        USER(string maID, string hoTen, string SDT, string Email, string username, string password);
        ~USER();
        virtual void HienThiThongTin() const;
        virtual void CapNhatThongTin();
        bool Login(string username, string password);
        void Logout();
        string getMaID() const;
        string getHoTen() const;
        string getUsername() const;
        string getSDT() const;
        string getEmail() const;
        void setPassword(string newPassword);
};
#endif