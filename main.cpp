#include <iostream>
#include "lib/FileManager.h"

int main() {
    std::vector<std::vector<std::string>> s{{}};
    if (FileManager::getInstance().getCSVDataSource(s, FileManager::CONSUME_CSV(56)))
        for (const auto& str: s){
            for (const auto& i: str)
                std::cout << i << std::endl;
        }
    else std::cout << "err" << std::endl;
    return 0;
}
