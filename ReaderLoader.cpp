#include "ReaderLoader.h"
#include <fstream>
#include <sstream>

NodeReader* loadReaders(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return nullptr;

    NodeReader* head = nullptr;
    std::string line;

    while (std::getline(in, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);

        std::string id, name, phone, mail, user, pass;

        std::getline(ss, id,   '|');
        std::getline(ss, name, '|');
        std::getline(ss, phone,'|');
        std::getline(ss, mail, '|');
        std::getline(ss, user, '|');
        std::getline(ss, pass, '|');

        Reader r(id, name, phone, mail, user, pass);

        NodeReader* n = new NodeReader(r);
        n->next = head;
        head = n;
    }

    return head;
}


void saveReaders(const std::string& filename,
                 NodeReader* head)
{
    std::ofstream out(filename);
    if (!out.is_open()) return;

    for(NodeReader* p = head; p != nullptr; p = p->next)
    {
        out << p->data.getMaID()   << "|"
            << p->data.getHoTen()  << "|"
            << p->data.getSDT()    << "|"
            << p->data.getEmail()  << "|"
            << p->data.getUsername()<< "|"
            << "xxxxxxxx"          << "\n";   // pass
    }
    out.close();
}
