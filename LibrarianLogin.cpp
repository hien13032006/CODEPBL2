#include "LibrarianLogin.h"
#include <fstream>
#include <sstream>

// tìm username
NodeLibrarian* findLibrarian(NodeLibrarian* head,
                             const std::string& username)
{
    for(NodeLibrarian* p = head; p != nullptr; p = p->next)
    {
        if (p->data.getUsername() == username)
            return p;
    }
    return nullptr;
}

// ĐĂNG NHẬP
bool loginLibrarian(NodeLibrarian* head,
                    const std::string& username,
                    const std::string& password)
{
    NodeLibrarian* p = findLibrarian(head, username);
    if (!p) return false;
    return p->data.Login(username, password);
}

// LOAD từ file
NodeLibrarian* loadLibrarians(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return nullptr;

    NodeLibrarian* head = nullptr;
    std::string line;

    while (std::getline(in, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);

        std::string id, name, phone, email, user, pass;

        std::getline(ss, id,   '|');
        std::getline(ss, name, '|');
        std::getline(ss, phone,'|');
        std::getline(ss, email,'|');
        std::getline(ss, user, '|');
        std::getline(ss, pass, '|');

        Librarian lib;
        lib.SetThongTin(id, name, phone, email, user, pass);

        NodeLibrarian* n = new NodeLibrarian(lib);
        n->next = head;
        head = n;
    }
    return head;
}
