#pragma once
#include "Node.h"
#include "Book.h"
#include <iostream>
using namespace std;

static void PrintBookList(NodeBook *head) {
    for (NodeBook *p = head; p != nullptr; p = p->next) {
        p->data->hienThiThongTin();
        cout << "-----------------------\n";
    }
}
