#include <iostream>
#include "lib/FileManager.h"

void testTimeWrapper(const std::function<void(void)> &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "use time: " << ms.count() << std::endl;
}

int main() {

    testTimeWrapper([&](){
        std::vector<std::vector<std::string>> s{{}};
        if (FileManager::getInstance().getCSVDataSource(s, FileManager::CONSUME_CSV(56)))
            for (const auto &str: s) {
                for (const auto &i: str)
                    std::cout << i << std::endl;
            }
        else std::cout << "err" << std::endl;
    });

    return 0;
}
