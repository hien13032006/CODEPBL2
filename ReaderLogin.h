#pragma once
#include <string>
#include "Node.h"
#include "Reader.h"

bool loginReader(NodeReader* head,
                 const std::string& username,
                 const std::string& password);

NodeReader* findReader(NodeReader* head,
                       const std::string& username);


