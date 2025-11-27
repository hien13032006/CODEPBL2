#include "Book.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Node.h"

// [MỚI] Khởi tạo biến tĩnh
TypeCounter Sach::typeCounts[Sach::MAX_TYPES];
int Sach::currentTypeSize = 0;

string Sach::toCSV() const {
    ostringstream oss;
    oss << maSach << "|" 
        << tenSach << "|" 
        << tacGia << "|"
        << theLoai << "|" 
        << namXuatBan << "|" 
        << nhaXuatBan << "|"
        << soLuong << "|" 
        << tongDiemDanhGia << "|"  // Thêm dòng này
        << soLuotDanhGia << "|"    // Thêm dòng này
        << imagePath; 
    return oss.str();
}
void Sach::hienThiThongTin() const {
    cout << left << setw(15) << maSach
         <<  setw(25) << tenSach
         <<  setw(20) << tacGia
         <<  setw(15) << theLoai
         <<  setw(8)  << namXuatBan
         <<  setw(25) << nhaXuatBan
         <<  setw(13) << soLuong
         << fixed << setprecision(1) << getDiemTrungBinh()
         << "\n";
}

// [MỚI] Hàm đồng bộ bộ đếm dùng mảng tĩnh
void Sach::resyncCounters(NodeBook* head) {
    currentTypeSize = 0; // Reset lại khi bắt đầu đồng bộ

    NodeBook* p = head;
    while (p != nullptr) {
        string ma = p->data->getMaSach();
        string prefix = "";
        string numStr = "";
        
        // Tách Prefix (chữ) và Số từ mã sách (VD: GT00001)
        for (char c : ma) {
            if (isdigit(c)) numStr += c;
            else prefix += c;
        }
        
        if (!prefix.empty() && !numStr.empty()) {
            try {
                int num = stoi(numStr);
                
                // Tìm trong mảng xem prefix này đã có chưa
                bool found = false;
                for (int i = 0; i < currentTypeSize; i++) {
                    if (typeCounts[i].prefix == prefix) {
                        // Nếu tìm thấy, cập nhật số lớn nhất
                        if (num > typeCounts[i].count) {
                            typeCounts[i].count = num;
                        }
                        found = true;
                        break;
                    }
                }
                
                // Nếu chưa có và mảng chưa đầy, thêm mới
                if (!found && currentTypeSize < MAX_TYPES) {
                    typeCounts[currentTypeSize].prefix = prefix;
                    typeCounts[currentTypeSize].count = num;
                    currentTypeSize++;
                }
                
            } catch (...) {}
        }
        p = p->next;
    }
}

// [SỬA] Tạo sách và sinh ID dựa trên mảng tĩnh
Sach* Sach::createFromData(const string& ten, const string& tg, const string& tl, int nam, const string& nxb) {
    Sach* s = nullptr;
    if (tl == "Giao trinh") s = new GiaoTrinh(ten, tg, tl, nam, nxb);
    else if (tl == "Tham khao") s = new ThamKhao(ten, tg, tl, nam, nxb);
    else if (tl == "Tieu thuyet") s = new TieuThuyet(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen ngan") s = new TruyenNgan(ten, tg, tl, nam, nxb);
    else if (tl == "Truyen tranh") s = new TruyenTranh(ten, tg, tl, nam, nxb);
    else if (tl == "Sach ki nang") s = new SachKiNang(ten, tg, tl, nam, nxb);
    else s = new GiaoTrinh(ten, tg, tl, nam, nxb); 

    // Logic sinh ID mới
    string pre = s->prefix();
    int currentCount = 0;
    bool found = false;

    // Tìm trong mảng
    for (int i = 0; i < currentTypeSize; i++) {
        if (typeCounts[i].prefix == pre) {
            typeCounts[i].count++; // Tăng số đếm
            currentCount = typeCounts[i].count;
            found = true;
            break;
        }
    }

    // Nếu chưa có trong mảng (loại mới), thêm vào
    if (!found) {
        if (currentTypeSize < MAX_TYPES) {
            typeCounts[currentTypeSize].prefix = pre;
            typeCounts[currentTypeSize].count = 1;
            currentCount = 1;
            currentTypeSize++;
        } else {
            // Trường hợp mảng đầy (hiếm khi xảy ra với 50 loại)
            currentCount = 1; 
        }
    }

    // Format ID: Prefix + 5 chữ số (VD: GT00001)
    ostringstream oss;
    oss << setw(5) << setfill('0') << currentCount;
    string id = pre + oss.str();
    s->setMaSach(id);

    return s;
}

void Sach::docFileInput(const string& fileName, NodeBook*& head) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cerr << " Khong mo duoc file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        string ten, tg, tl, nxb, namStr, soLuongStr, imagePath;
        stringstream ss(line);
        getline(ss, ten, '|');
        getline(ss, tg, '|');
        getline(ss, tl, '|');
        getline(ss, namStr, '|');
        getline(ss, nxb, '|');
        getline(ss, soLuongStr, '|');
        getline(ss, imagePath, '|'); 

        try {
            int nam = stoi(namStr);
            int sl = stoi(soLuongStr);
            Sach* s = Sach::createFromData(ten, tg, tl, nam, nxb);
            s->setSoLuong(sl);
            // Lưu ý: ID đã được tạo trong createFromData, nhưng nếu đọc từ file
            // thì thường ta sẽ giữ nguyên ID cũ (nếu file có cột ID).
            // Tuy nhiên hàm này là đọc file INPUT (chưa có ID) nên để createFromData tự tạo là đúng.
            s->setImagePath(imagePath); 

            NodeBook* newNode = new NodeBook(s);
            newNode->next = head;
            head = newNode;
        } catch (...) {
            // Bỏ qua dòng lỗi
        }
    }
    in.close();
    cout << "Doc file input thanh cong: " << fileName << endl;
}

void Sach::ghiFile(const string& fileName, NodeBook* head) {
    ofstream out(fileName);
    if (!out.is_open()) {
        cerr << " Khong mo duoc file de ghi: " << fileName << endl;
        return;
    }

    NodeBook* p = head;
    while (p) {
        out << p->data->toCSV() << endl;
        p = p->next;
    }

    out.close();
    cout << " Da ghi danh sach sach vao: " << fileName << endl;
}