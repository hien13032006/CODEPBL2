#pragma once
#include <string>
#include "Node.h"
#include "Book.h"
using namespace std;

// Trả về danh sách KẾT QUẢ (linked list mới)
static NodeBook* searchBook(NodeBook *head, const string& key) {
    NodeBook *res = nullptr;

    for (NodeBook *p = head; p != nullptr; p = p->next) {
        string k = key;
        string ten = p->data->getTenSach();
        string tg  = p->data->getTacGia();
        string tl  = p->data->getTheLoai();

        // lower-case so sánh
        auto low = [](string s){
            for (char &c : s) c = tolower(c);
            return s;
        };

        if ( low(ten).find(low(k)) != string::npos ||
             low(tg ).find(low(k)) != string::npos ||
             low(tl ).find(low(k)) != string::npos )
        {
            NodeBook *n = new NodeBook(p->data->clone());
            n->next = res;
            res = n;
        }
    }
    return res;
}
