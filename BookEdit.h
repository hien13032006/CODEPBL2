#pragma once
#include "Node.h"
#include "Book.h"
using namespace std;
static bool editBook(NodeBook* head, const string& ten, Sach *newData) {
    for (NodeBook *p = head; p != nullptr; p = p->next) {
        if (p->data->getTenSach() == ten) {
            *(p->data) = *newData;
            return true;
        }
    }
    return false;
}
