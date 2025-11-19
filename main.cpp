#include "App.hpp"

int main() {
    try {
        App app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Loi: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}