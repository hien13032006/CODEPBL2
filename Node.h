#ifndef Node_h
#define Node_h
#include "Book.h"
#include "Reader.h"

struct NodeBook {
    Sach data;
    NodeBook *next;

    
    NodeBook(const Sach& s) {
        data = s;
        next = nullptr;
    }

};

struct NodeReader {
    Reader data;
    NodeReader *next;

    NodeReader(const Reader& r) {
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