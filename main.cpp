#include <iostream>
#include "lib/FileManager.h"

int main() {
    std::vector<std::string> s;
    if (FileManager::getInstance().getStringDataSourceByLine(s)) for (auto &&str: s) std::cout << str << std::endl;
    else std::cout << "err" << std::endl;
    return 0;
}
