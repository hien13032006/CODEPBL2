#pragma once
#include "Node.h"
#include "Book.h"
using namespace std;

static bool deleteBook(NodeBook* &head, const string& ten) {
    NodeBook *p = head, *prev = nullptr;

    while(p) {
        if (p->data->getTenSach() == ten) {
            if (prev) prev->next = p->next;
            else head = p->next;

            delete p->data;
            delete p;
            return true;
        }
        prev = p;
        p = p->next;
    }
    return false;
}
