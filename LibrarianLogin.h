#pragma once
#include <string>
#include "Node.h"
#include "Librarian.h"

bool loginLibrarian(NodeLibrarian* head,
                    const std::string& username,
                    const std::string& password);

NodeLibrarian* findLibrarian(NodeLibrarian* head,
                             const std::string& username);

NodeLibrarian* loadLibrarians(const std::string& filename);
