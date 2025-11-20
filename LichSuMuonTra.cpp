#include "LichSuMuonTra.h"

StackLichSu::~StackLichSu() {
    while (top != nullptr) {
        NodeLichSu* temp = top;
        top = top->next;
        delete temp;
    }
}

void StackLichSu::push(const LichSuMuonTra& ls) {
    NodeLichSu* newNode = new NodeLichSu{ls, top};
    top = newNode;
}

bool StackLichSu::isEmpty() const {
    return top == nullptr;
}

void StackLichSu::hienThiLichSu() const {
    if (isEmpty()) {
        cout << "Ban chua co lich su muon tra nao.\n";
        return;
    }

    cout << "\nLich su muon - tra:\n";
    NodeLichSu* current = top;
    while (current != nullptr) {
        cout << current->data.hanhDong << " - " << current->data.tenSach
             << " (" << current->data.maSach << ") luc " << current->data.thoiGian << endl;
        current = current->next;
    }
}

