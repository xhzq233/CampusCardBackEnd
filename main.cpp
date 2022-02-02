#include <iostream>
#include "FileManager.h"
int main() {
    std::vector<std::string> s;
    FileManager::getInstance().getStringDataSourceByLine(s);
    for(auto &&str:s){
        std::cout << str << std::endl;
    }
    return 0;
}
