#include "ReaderLogin.h"
#include "LibrarySystem.h"
#include "Reader.h"

NodeReader* findReader(NodeReader* head,
                       const std::string& username)
{
    for(NodeReader* p = head; p != nullptr; p = p->next)
    {
        if (p->data->getUsername() == username)
            return p;
    }
    return nullptr;
}

bool loginReader(NodeReader* head,
                 const std::string& username,
                 const std::string& password)
{
    NodeReader* p = findReader(head, username);
    if (!p) return false;
    return p->data->Login(username, password);
}
