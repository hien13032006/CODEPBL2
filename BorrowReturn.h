#pragma once
#include <fstream>
#include <ctime>
#include "Node.h"
#include "Book.h"

inline bool muonSach(NodeBook* head, const std::string& ma, const std::string& user, const std::string& histFile){
    for (NodeBook* p = head; p; p = p->next){
        if (p->data->getMaSach()==ma){
            if (p->data->getTinhTrang()=="Dang con"){
                p->data->muonSach();
                std::ofstream out(histFile, std::ios::app);
                std::time_t t=std::time(nullptr);
                out<< (long long)t <<"|MUON|"<< ma <<"|"<< user <<"\n";
                return true;
            }
            return false;
        }
    }
    return false;
}
inline bool traSach(NodeBook* head, const std::string& ma, const std::string& user, const std::string& histFile){
    for (NodeBook* p = head; p; p = p->next){
        if (p->data->getMaSach()==ma){
            if (p->data->getTinhTrang()=="Da muon"){
                p->data->traSach();
                std::ofstream out(histFile, std::ios::app);
                std::time_t t=std::time(nullptr);
                out<< (long long)t <<"|TRA|"<< ma <<"|"<< user <<"\n";
                return true;
            }
            return false;
        }
    }
    return false;
}
