#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Node.h"
#include "Librarian.h"
using namespace std;

inline NodeLibrarian* loadLibrarians(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return nullptr;

    NodeLibrarian* head = nullptr;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, phone, mail, user, pass;

        getline(ss, id,   '|');
        getline(ss, name, '|');
        getline(ss, phone,'|');
        getline(ss, mail, '|');
        getline(ss, user, '|');
        getline(ss, pass, '|');

        Librarian lb;
        lb.SetThongTin(id, name, phone, mail, user, pass);

        NodeLibrarian* n = new NodeLibrarian(lb);
        n->next = head;
        head = n;
    }
    return head;
}
