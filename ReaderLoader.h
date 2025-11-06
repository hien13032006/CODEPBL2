#pragma once
#include <string>
#include "Node.h"
#include "Reader.h"

// load file txt
NodeReader* loadReaders(const std::string& filename);

void saveReaders(const std::string& filename,
                 NodeReader* head);
