#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
using namespace std;

TreeNode* LibrarySystem::insertByKey(TreeNode* root, Sach* s, const string& key) {
    if (root == nullptr) return new TreeNode(s);
    if (key < root->data->getTenSach())
        root->left = insertByKey(root->left, s, key);
    else
        root->right = insertByKey(root->right, s, key);
    return root;
}

TreeNode* LibrarySystem::insertByIntKey(TreeNode* root, Sach* s, int key) {
    if (root == nullptr) return new TreeNode(s);
    if (key < root->data->getNamXuatBan())
        root->left = insertByIntKey(root->left, s, key);
    else
        root->right = insertByIntKey(root->right, s, key);
    return root;
}

void LibrarySystem::XayDungTatCaCay() {
    rootMaSach = rootTenSach = rootTacGia = rootTheLoai = rootNamXB = nullptr;

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        Sach* s = current->data;
        rootMaSach   = insertByKey(rootMaSach, s, s->getMaSach());
        rootTenSach  = insertByKey(rootTenSach, s, s->getTenSach());
        rootTacGia   = insertByKey(rootTacGia, s, s->getTacGia());
        rootTheLoai  = insertByKey(rootTheLoai, s, s->getTheLoai());
        rootNamXB    = insertByIntKey(rootNamXB, s, s->getNamXuatBan());
        current = current->next;
    }
}


void LibrarySystem::inOrderSearchKey(TreeNode* root, const string& key, int tieuChi, bool &found) const {
    if (!root) return;
    inOrderSearchKey(root->left, key, tieuChi, found);

    string value;
    switch (tieuChi) {
        case 1: value = root->data->getMaSach(); break;
        case 2: value = root->data->getTenSach(); break;
        case 3: value = root->data->getTacGia(); break;
        case 5: value = root->data->getTheLoai(); break;
    }

    if (value == key) {
        root->data->hienThiThongTin();
        found = true;
    }

    inOrderSearchKey(root->right, key, tieuChi, found);
}

void LibrarySystem::inOrderSearchInt(TreeNode* root, int key, bool &found) const {
    if (!root) return;
    inOrderSearchInt(root->left, key, found);

    if (root->data->getNamXuatBan() == key) {
        root->data->hienThiThongTin();
        found = true;
    }

    inOrderSearchInt(root->right, key, found);
}

LibrarySystem::LibrarySystem() {
    HeadDsSach = nullptr;
    HeadDsDocGia = nullptr;
}

LibrarySystem::~LibrarySystem() {
    NodeBook *currentBook = HeadDsSach;
    while (currentBook != nullptr) {
        NodeBook *temp = currentBook;
        currentBook = currentBook->next;
        delete temp;
    }

    NodeReader *currentReader = HeadDsDocGia;
    while (currentReader != nullptr) {
        NodeReader *temp = currentReader;
        currentReader = currentReader->next;
        delete temp;
    }
}

void LibrarySystem::DocFileSach(const string& fileName) {
    Sach::docFileInput(fileName, HeadDsSach);
    cout << " Da nap danh sach sach tu file: " << fileName << endl;
}

void LibrarySystem::GhiFileSach(const string& fileName) const {
    Sach::ghiFile(fileName, HeadDsSach);
    cout << " Da luu danh sach sach vao file: " << fileName << endl;
}

void LibrarySystem::GhiFileHeThong(const string& fileName) const {
    ofstream out(fileName);
    if (!out.is_open()) {
        cerr << "Khong the mo file " << fileName << " de ghi.\n";
        return;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        out << current->data->getMaSach() << "|"
            << current->data->getTenSach() << "|"
            << current->data->getTacGia() << "|"
            << current->data->getTheLoai() << "|"
            << current->data->getNamXuatBan() << "|"
            << current->data->getNhaXuatBan() << "|"
            << (current->data->getTinhTrang()) << "|"
            << current->data->getTongDiem() << "|"
            << current->data->getSoDanhGia()
            << "\n";
        current = current->next;
    }

    out.close();
    cout << "Da cap nhat file he thong.\n";
}

void LibrarySystem::DocFileHeThong(const string& fileName) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cerr << "Khong the mo file " << fileName << endl;
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string ma, ten, tacGia, theLoai, nhaXB, tinhTrangStr, diemTBStr, soDanhGiaStr;
        int namXB;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');
        ss >> namXB;
        ss.ignore();
        getline(ss, nhaXB, '|');
        getline(ss, tinhTrangStr,'|');
        getline(ss, diemTBStr, '|');
        getline(ss, soDanhGiaStr);

        bool tinhTrang = (tinhTrangStr == "Dang con");

        double tong = 0;
        int soDG = 0;
        if (!diemTBStr.empty()) tong = stod(diemTBStr);
        if (!soDanhGiaStr.empty()) soDG = stoi(soDanhGiaStr);

        Sach* sachMoi = Sach::createFromData(ten, tacGia, theLoai, namXB, nhaXB);
        sachMoi->setMaSach(ma);
        sachMoi->setTinhTrang(tinhTrang ? "Dang con" : "Da muon");
        sachMoi->setDanhGia(tong, soDG);

        NodeBook* newNode = new NodeBook(sachMoi);
        newNode->next = nullptr;

        if (HeadDsSach == nullptr) {
            HeadDsSach = newNode;
        } else {
            NodeBook* temp = HeadDsSach;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode;
        }
    }

    in.close();
    cout << " Doc du lieu tu file he thong thanh cong!\n";
}


void LibrarySystem::DocFileDocGia() {
    ifstream in("DocGia.txt");
    if (!in.is_open()) {
        cerr << "Khong tim thay file DocGia.txt.\n";
        return;
    }

    string line;
    int maxID = 0;
    int count = 0;

    while (getline(in, line)) {
        if (line.empty() || line == "\r" || line == "\n") continue;

        stringstream ss(line);
        string ma, hoten, sdt, email, user, pass;
        getline(ss, ma, '|');
        getline(ss, hoten, '|');
        getline(ss, sdt, '|');
        getline(ss, email, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');

        Reader* newReader = new Reader(ma, hoten, sdt, email, user, pass);

        NodeReader* newNode = new NodeReader(newReader);
        newNode->next = nullptr;

        if (HeadDsDocGia == nullptr) {
            HeadDsDocGia = newNode;
        } else {
            NodeReader* temp = HeadDsDocGia;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode;
        }

        try {
            int idNum = stoi(ma.substr(1));
            if (idNum > maxID) maxID = idNum;
        } catch (...) {}

        count++;
    }

    in.close();
    Reader::setReaderCount(maxID + 1);

    cout << " Da nap danh sach doc gia tu file (" << count << " doc gia).\n";
}

bool kiemTraSDT(const string& sdt) {
    if (sdt.length() != 10 || sdt[0] != '0') return false;
    char dauSo = sdt[1];
    if (dauSo!='3' && dauSo!='5' && dauSo!='7' && dauSo!='8' && dauSo!='9')
        return false;
    for(char c : sdt)
        if(!isdigit(c)) return false;
    return true;
}

// kiem tra email @gmail.com
bool kiemTraEmail(const string& email) {
    const string duoi = "@gmail.com";
    if(email.length() <= duoi.length()) return false;
    return email.substr(email.length()-duoi.length()) == duoi;
}

// password = 8
bool kiemTraMatKhau(const string& pass){
    return pass.length() == 8;
}

// username ton tai?
bool LibrarySystem::KiemTraDocGiaDaDangKy(const string& tenDangNhap) const {
    NodeReader* current = HeadDsDocGia;
    while(current){
        if(current->data->getUsername() == tenDangNhap)
            return true;
        current = current->next;
    }
    return false;
}

void LibrarySystem::DangKyDocGia() {
    string hoTen, sdt, email, user, pass;

    cout << "Nhap ho ten: ";
    getline(cin, hoTen);

    while(true){
        cout << "Nhap SDT: ";
        getline(cin, sdt);
        if(kiemTraSDT(sdt)) break;
        cout << "So dien thoai khong hop le. Nhap lai.\n";
    }

    while(true){
        cout << "Nhap Email: ";
        getline(cin, email);
        if(kiemTraEmail(email)) break;
        cout << "Email khong hop le. Nhap lai.\n";
    }

    while(true){
        cout << "Nhap username: ";
        getline(cin, user);
        if(!KiemTraDocGiaDaDangKy(user)) break;
        cout << "Username ton tai. Nhap lai.\n";
    }

    while(true){
        cout << "Nhap password (8 ky tu): ";
        getline(cin, pass);
        if(kiemTraMatKhau(pass)) break;
        cout << "Password khong hop le.\n";
    }

    Reader* dg = new Reader();
    dg->SignUp(hoTen, sdt, email, user, pass);

    NodeReader* newNode = new NodeReader(dg);
    newNode->next = HeadDsDocGia;
    HeadDsDocGia = newNode;

    ofstream out("DocGia.txt", ios::app);
    if(out.is_open()){
        out << dg->toCSV() << endl;
        out.close();
        cout << "Da luu doc gia moi.\n";
    } else {
        cout << "Khong mo duoc file DocGia.txt.\n";
    }
}


bool LibrarySystem::DangNhapDocGia(USER* &currentUser) {
    string username, password;

    while(true){
        cout << "\n===== DANG NHAP DOC GIA =====\n";
        cout << "Nhap username: ";
        getline(cin, username);

        cout << "Nhap password: ";
        getline(cin, password);

        NodeReader* p = HeadDsDocGia;
        bool ok = false;
        while(p){
            if(p->data->Login(username, password)){
                currentUser = p->data;
                cout << "Dang nhap thanh cong!\n";
                return true;
            }
            p = p->next;
        }
        cout << "Sai username / password. Nhap lai!\n";
    }
}

bool LibrarySystem::DangNhapThuThu(const string &usernameInput,
                                   const string &passwordInput,
                                   USER* &currentUser){
    string username = usernameInput;
    string password = passwordInput;

    while(true){
        ifstream in("ThuThu.txt");
        if(!in.is_open()){
            cout << "Khong the mo file ThuThu.txt\n";
            return false;
        }

        bool found = false;
        string line;
        while(getline(in, line)){
            stringstream ss(line);
            string maID, hoTen, sdt, email, user, pass;
            getline(ss, maID, '|');
            getline(ss, hoTen, '|');
            getline(ss, sdt, '|');
            getline(ss, email, '|');
            getline(ss, user, '|');
            getline(ss, pass, '|');

            if(user == username && pass == password){
                Librarian* tt = new Librarian();
                tt->SetThongTin(maID, hoTen, sdt, email, user, pass);
                currentUser = tt;
                cout << "Dang nhap thanh cong!\n";
                found = true;
                break;
            }
        }
        in.close();

        if(found) return true;

        cout << "Sai tai khoan / mat khau! Nhap lai!\n";
        cout << "Ten dang nhap: ";
        getline(cin, username);
        cout << "Mat khau: ";
        getline(cin, password);
    }
}

bool LibrarySystem::DangXuat(USER* &currentUser){
    if(currentUser){
        currentUser->Logout();
        currentUser = nullptr;
        return true;
    }
    cout << "Khong co ai dang dang nhap.\n";
    return false;
}

void LibrarySystem::HienThiTatCaDocGia() const {
    NodeReader *p = HeadDsDocGia;
    if(!p){
        cout << "Khong co doc gia nao.\n";
        return;
    }
    cout << "===== TAT CA DOC GIA =====\n";
    cout << left << setw(13) << "Ma ID"
         << setw(23) << "Ho Ten"
         << setw(17) << "So DT"
         << setw(20) << "Email"
         << setw(15) << "Username"
         << "\n";

    while(p){
        p->data->HienThiThongTin();
        p = p->next;
    }
}

void LibrarySystem::MuonSach(Reader* docGia, const string& maSach) {
    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {

            if (current->data->getTinhTrang() == "Da muon") {
                cout << "Sach da het !\n";
                return;
            }

            if (docGia->DaMuonSach(maSach)) {
                cout << "Ban da muon sach nay roi.\n";
                return;
            }

            if (docGia->DemSachDaMuon() >= 5) {
                cout << "Ban da muon toi da so sach.\n";
                return;
            }

            current->data->muonSach();
            docGia->themSachDaMuon(current->data);
            docGia->ghiLichSu("Muon", current->data);

            GhiFileHeThong("DanhSachSach.txt");
            cout << "Muon sach thanh cong: " << current->data->getTenSach() << endl;
            return;
        }
        current = current->next;
    }
    cout << "Khong tim thay sach.\n";
}

void LibrarySystem::TraSach(Reader* docGia, const string& maSach) {
    if (!docGia->DaMuonSach(maSach)) {
        cout << "Ban chua muon sach co ma: " << maSach << endl;
        return;
    }

    NodeBook* current = HeadDsSach;
    while (current != nullptr) {
        if (current->data->getMaSach() == maSach) {

            current->data->traSach();
            docGia->xoaSachDaMuon(maSach);
            docGia->ghiLichSu("Tra", current->data);

            cout << "Tra sach thanh cong: " << current->data->getTenSach() << endl;

            int luaChon;
            cout << "Ban co muon danh gia sach nay? (1:Co / 0:Khong): ";
            cin >> luaChon;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (luaChon == 1) {
                DanhGiaSach(docGia, current->data);
            }

            GhiFileHeThong("DanhSachSach.txt");
            return;
        }
        current = current->next;
    }

    cout << "Khong tim thay sach trong he thong.\n";
}

void LibrarySystem::DanhGiaSach(Reader* docGia, Sach* sach) {
    cout << "\n===== DANH GIA SACH =====\n";
    cout << "Ban cam thay sach: \"" << sach->getTenSach() << "\" nhu the nao:\n";
    cout << "1. Rat hay!\n";
    cout << "2. Hay!\n";
    cout << "3. Binh thuong!\n";
    cout << "4. Khong hay!\n";
    cout << "5. Rat te!\n";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int diem = 0;
    string danhGiaText;

    switch (choice) {
        case 1: diem = 10; danhGiaText = "Rat hay"; break;
        case 2: diem = 8;  danhGiaText = "Hay"; break;
        case 3: diem = 5;  danhGiaText = "Binh thuong"; break;
        case 4: diem = 4;  danhGiaText = "Khong hay"; break;
        case 5: diem = 2;  danhGiaText = "Rat te"; break;
        default:
            cout << "Lua chon khong hop le!\n";
            return;
    }

    sach->themDanhGia(diem);
    GhiFileHeThong("DanhSachSach.txt");

    ofstream out("DanhGia.txt", ios::app);
    if (out.is_open()) {
        out << sach->getMaSach() << "|"
            << sach->getTenSach() << "|"
            << sach->getTacGia() << "|"
            << sach->getNamXuatBan() << "|"
            << sach->getNhaXuatBan() << "|"
            << docGia->getMaID() << "|"
            << docGia->getUsername() << "|"
            << danhGiaText << "|"
            << diem << endl;
        out.close();
    }
    cout << "Cam on ban da danh gia!\n";
}

double LibrarySystem::TinhDiemTrungBinhTuFile(const string& tenSach,
                                              const string& tacGia,
                                              int namXB,
                                              const string& nhaXB) {
    ifstream in("DanhGia.txt");
    if (!in.is_open()) return 0.0;

    string line;
    double tong = 0;
    int dem = 0;

    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string ma, ten, tg, namStr, nxb, maDG, tenDG, dgText, diemStr;

        getline(ss, ma, '|');
        getline(ss, ten, '|');
        getline(ss, tg,  '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');
        getline(ss, maDG, '|');
        getline(ss, tenDG, '|');
        getline(ss, dgText, '|');
        getline(ss, diemStr, '|');

        int diem = stoi(diemStr);
        int nam  = stoi(namStr);

        if (ten == tenSach && tg == tacGia && nam == namXB && nxb == nhaXB) {
            tong += diem;
            dem++;
        }
    }
    in.close();

    return dem == 0 ? 0.0 : tong / dem;
}


void LibrarySystem::HienThiDanhSachSach() {
    NodeBook *current = HeadDsSach;
    if (!current) {
        cout << "Khong co sach trong thu vien.\n";
        return;
    }

    cout << "+--------------------------------------------------------------------------------------------------------------+\n";
    cout << left << setw(15) << "Ma sach"
         << setw(25) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(15) << "The loai"
         << setw(10) << "NamXB"
         << setw(21) << "Nha XB"
         << setw(15) << "Tinh trang"
         << setw(10) << "Diem DG"
         << "|\n";
    cout << "+--------------------------------------------------------------------------------------------------------------+\n";

    while (current) {
        current->data->hienThiThongTin();
        current = current->next;
    }

    cout << "+--------------------------------------------------------------------------------------------------------------+\n";
}

void LibrarySystem::XepHangSach() {
    if (HeadDsSach == nullptr) {
        cout << "Thu vien chua co sach!\n";
        return;
    }

    struct NodeThongKe {
        string tenSach;
        string tacGia;
        int namXB;
        string nhaXB;
        double tongDiem;
        int soDanhGia;
        NodeThongKe* next;

        NodeThongKe(string ten, string tg, int nxb, string nxbx,
                     double tong, int so)
            : tenSach(ten), tacGia(tg), namXB(nxb),
              nhaXB(nxbx), tongDiem(tong), soDanhGia(so),
              next(nullptr) {}

        double diemTB() const {
            return soDanhGia == 0 ? 0 : tongDiem / soDanhGia;
        }
    };

    NodeThongKe* headTK = nullptr;

    for (NodeBook* cur = HeadDsSach; cur != nullptr; cur = cur->next) {
        Sach* s = cur->data;

        NodeThongKe* p = headTK;
        bool found = false;
        while (p) {
            if (p->tenSach == s->getTenSach() &&
                p->tacGia == s->getTacGia() &&
                p->namXB  == s->getNamXuatBan() &&
                p->nhaXB  == s->getNhaXuatBan())
            {
                p->tongDiem   += s->getTongDiem();
                p->soDanhGia  += s->getSoDanhGia();
                found = true;
                break;
            }
            p = p->next;
        }

        if (!found) {
            NodeThongKe* newNode = new NodeThongKe(
                s->getTenSach(),
                s->getTacGia(),
                s->getNamXuatBan(),
                s->getNhaXuatBan(),
                s->getTongDiem(),
                s->getSoDanhGia()
            );
            newNode->next = headTK;
            headTK = newNode;
        }
    }

    if (!headTK) {
        cout << "Chua co sach nao duoc danh gia.\n";
        return;
    }

    for (NodeThongKe* i = headTK; i; i = i->next) {
        for (NodeThongKe* j = i->next; j; j = j->next) {
            if (i->diemTB() < j->diemTB()) {
                swap(i->tenSach,  j->tenSach);
                swap(i->tacGia,   j->tacGia);
                swap(i->namXB,    j->namXB);
                swap(i->nhaXB,    j->nhaXB);
                swap(i->tongDiem, j->tongDiem);
                swap(i->soDanhGia,j->soDanhGia);
            }
        }
    }

    cout << "\n===== TOP 10 =====\n";
    cout << left
         << setw(5)  << "STT"
         << setw(30) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(8)  << "NamXB"
         << setw(20) << "NhaXB"
         << setw(10) << "DiemTB"
         << endl;
    cout << string(93,'-') << endl;

    int stt = 1;
    for (NodeThongKe* p = headTK; p && stt <= 10; p = p->next) {
        cout << setw(5)  << stt++
             << setw(30) << p->tenSach
             << setw(20) << p->tacGia
             << setw(8)  << p->namXB
             << setw(20) << p->nhaXB
             << setw(10) << fixed << setprecision(1) << p->diemTB()
             << endl;
    }
    cout << string(93,'-') << endl;

    while (headTK) {
        NodeThongKe* temp = headTK;
        headTK = headTK->next;
        delete temp;
    }
}


int LibrarySystem::exportReaderList(string arr[]) {
    int i = 0;
    for(NodeReader* p = HeadDsDocGia; p; p = p->next){
        arr[i] = p->data->getUsername() + " - "
               + p->data->getHoTen() + " - "
               + p->data->getSDT();
        i++;
        if(i >= 200) break;
    }
    return i;
}

bool LibrarySystem::deleteReader(string user) {
    NodeReader *p = HeadDsDocGia, *prev = nullptr;
    while(p){
        if(p->data->getUsername() == user){
            if(prev) prev->next = p->next;
            else HeadDsDocGia = p->next;

            delete p->data;
            delete p;

            updateReaderFile();
            return true;
        }
        prev = p;
        p = p->next;
    }
    return false;
}

Reader* LibrarySystem::getReaderByIndex(int idx) {
    int i = 0;
    for (NodeReader* p = HeadDsDocGia; p; p = p->next, i++)
        if (i == idx)
            return p->data;
    return nullptr;
}

void LibrarySystem::addBook(Sach* s) {
    NodeBook* newNode = new NodeBook(s);
    newNode->next = HeadDsSach;
    HeadDsSach = newNode;
    GhiFileHeThong("DanhSachSach.txt");
}
bool LibrarySystem::XoaSach(const string &maSach) {
    NodeBook* p = HeadDsSach;
    NodeBook* prev = nullptr;

    while (p) {
        if (p->data->getMaSach() == maSach) {

            if (prev) prev->next = p->next;
            else HeadDsSach = p->next;

            delete p->data;
            delete p;

            GhiFileHeThong("DanhSachSach.txt");
            return true;
        }
        prev = p;
        p = p->next;
    }

    return false;
}

bool LibrarySystem::deleteBook(const string& id){
    NodeBook *p = HeadDsSach, *prev = nullptr;
    while(p){
        if(p->data->getMaSach() == id){
            if(prev) prev->next = p->next;
            else HeadDsSach = p->next;

            delete p;
            GhiFileHeThong("DanhSachSach.txt");
            return true;
        }
        prev = p;
        p = p->next;
    }
    return false;
}



bool LibrarySystem::updateBook(const string& id,
                               string tenMoi,
                               string tacGiaMoi,
                               int namMoi,
                               int soLuongMoi)
{
    NodeBook *p = HeadDsSach;
    while(p){
        if(p->data->getMaSach() == id){
            if(!tenMoi.empty())     p->data->setTenSach(tenMoi);
            if(!tacGiaMoi.empty())  p->data->setTacGia(tacGiaMoi);
            if(namMoi > 0)          p->data->setNamXuatBan(namMoi);
            GhiFileHeThong("DanhSachSach.txt");
            return true;
        }
        p = p->next;
    }
    return false;
}



Sach* LibrarySystem::findBookByID(const string& id){
    NodeBook *p = HeadDsSach;
    while(p){
        if(p->data->getMaSach() == id)
            return p->data;
        p = p->next;
    }
    return nullptr;
}



Sach* LibrarySystem::findBookByName(const string& name){
    NodeBook *p = HeadDsSach;
    while(p){
        if(p->data->getTenSach() == name)
            return p->data;
        p = p->next;
    }
    return nullptr;
}
