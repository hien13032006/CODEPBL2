#ifndef Node_h
#define Node_h
#include "Reader.h"
#include "Librarian.h"

class Sach;

struct NodeBook {
    Sach* data;
    NodeBook *next;

    NodeBook(Sach *s) {
        data = s;
        next = nullptr;
    }

};

struct NodeReader {
    Reader* data;
    NodeReader* next;

    NodeReader(Reader* r) {
        data = r;
        next = nullptr;
    }
};


struct NodeLibrarian {
    Librarian data;
    NodeLibrarian *next;

    NodeLibrarian(const Librarian& l) {
        data = l;
        next = nullptr;
    }
};

#endif