#pragma once
#include "Node.h"

static void addBook(NodeBook* &head, Sach *s) {
    NodeBook *p = new NodeBook(s);
    p->next = head;
    head = p;
}
