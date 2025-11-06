#pragma once
#include <fstream>
#include <sstream>
#include "Node.h"
#include "Book.h" 
using namespace std;

static NodeBook* loadBooks(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return nullptr;

    NodeBook *head = nullptr;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);

        string ten, tacgia, loai, nxb, yearStr;
        getline(ss, ten,    '|');
        getline(ss, tacgia, '|');
        getline(ss, loai,   '|');
        getline(ss, yearStr,'|');
        getline(ss, nxb,    '|');

        int nam = stoi(yearStr);

        Sach *s = Sach::createFromData(ten, tacgia, loai, nam, nxb);

        NodeBook *p = new NodeBook(s);
        p->next = head;
        head = p;
    }
    return head;
}
