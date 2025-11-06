#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include "Node.h"
#include "Book.h"

inline NodeBook* findByMa(NodeBook* head, const std::string& ma) {
    for (NodeBook* p = head; p; p = p->next)
        if (p->data->getMaSach() == ma) return p;
    return nullptr;
}

// Đếm prefix hiện có để sinh mã mới (GT001, TN002, ...)
inline int countPrefix(NodeBook* head, const std::string& prefix) {
    int mx = 0;
    for (NodeBook* p = head; p; p = p->next) {
        const std::string& m = p->data->getMaSach();
        if (m.rfind(prefix, 0) == 0) { // bắt đầu bằng prefix
            int num = 0;
            try { num = std::stoi(m.substr(prefix.size())); } catch (...) {}
            if (num > mx) mx = num;
        }
    }
    return mx;
}

// Thêm sách (tự sinh mã theo prefix)
inline void addBook(NodeBook*& head, Sach* s) {
    std::string pre = s->prefix();
    int next = countPrefix(head, pre) + 1;
    std::ostringstream oss;
    oss << pre << std::setw(3) << std::setfill('0') << next;
    s->setMaSach(oss.str());

    NodeBook* n = new NodeBook(s);
    n->next = head;
    head = n;
}

// Xóa theo mã
inline bool deleteBookByMa(NodeBook*& head, const std::string& ma) {
    NodeBook *p = head, *prev = nullptr;
    while (p) {
        if (p->data->getMaSach() == ma) {
            if (prev) prev->next = p->next; else head = p->next;
            delete p->data;
            delete p;
            return true;
        }
        prev = p; p = p->next;
    }
    return false;
}

// Sửa tất cả field (ghi đè)
inline bool editBookByMa(NodeBook* head, const std::string& ma,
                         const std::string& ten, const std::string& tg,
                         const std::string& tl, int nam, const std::string& nxb)
{
    NodeBook* p = findByMa(head, ma);
    if (!p) return false;
    p->data->setTenSach(ten);
    p->data->setTacGia(tg);
    p->data->setTheLoai(tl);
    p->data->setNamXuatBan(nam);
    p->data->setNhaXuatBan(nxb);
    return true;
}
