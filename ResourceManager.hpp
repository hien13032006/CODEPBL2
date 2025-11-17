#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
private:
    std::map<std::string, sf::Font> fonts;
    static ResourceManager* instance;
    
    ResourceManager() {}

public:
    static ResourceManager* getInstance();
    bool loadFont(const std::string& name, const std::string& path);
    sf::Font& getFont(const std::string& name);
};

#endif