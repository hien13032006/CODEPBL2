#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
private:
    // Map lưu trữ texture: Key là đường dẫn file, Value là Texture
    std::map<std::string, sf::Texture> textures;
    
    // Texture mặc định dùng khi không tìm thấy ảnh
    sf::Texture defaultTexture;

    // Singleton instance
    static ResourceManager* instance;

    // Constructor private
    ResourceManager() {
        // Tải sẵn ảnh default khi khởi tạo
        if (!defaultTexture.loadFromFile("default_book.png")) {
            sf::Image img;
            img.create(50, 70, sf::Color(200, 200, 200));
            defaultTexture.loadFromImage(img);
        }
        defaultTexture.setSmooth(true);
    }

public:
    // Lấy instance duy nhất (Singleton)
    static ResourceManager* getInstance() {
        if (instance == nullptr) {
            instance = new ResourceManager();
        }
        return instance;
    }

    sf::Texture& getTexture(const std::string& path) {
        // 1. Nếu đường dẫn rỗng, trả về mặc định
        if (path.empty()) return defaultTexture;
        // 2. Kiểm tra xem đã load chưa
        if (textures.find(path) == textures.end()) {
            sf::Texture tex;
            if (tex.loadFromFile(path)) {
                tex.setSmooth(true);
                textures[path] = tex; // Lưu vào cache
            } else {
                textures[path] = defaultTexture;
            }
        }
        // 3. Trả về texture từ RAM
        return textures[path];
    }  
    static void cleanup() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
};

ResourceManager* ResourceManager::instance = nullptr;

#endif
