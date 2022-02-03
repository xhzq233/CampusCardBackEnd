#include <iostream>
#include "lib/FileManager.h"

void testTimeWrapper(const std::function<void(void)> &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "use time: " << ms.count() << " ms." << std::endl;
}

int main() {

    testTimeWrapper([&]() {
        std::vector<std::vector<std::string>> s;
        if (FileManager::getInstance().getCSVDataSource(s, FileManager::CONSUME_CSV(56)));
        else std::cout << "err1" << std::endl;

        std::cout << "the first:" << s[0][0]<<" ,length:" << std::to_string(s.size()) << std::endl;

        FileManager::getInstance()<<s[0][0]<<s[0][0]<<s[0][0]<< "xhzq.xyz";

//        if (FileManager::getInstance().writeCSVData(s, "xhzq.csv", "../adjygvjsafvj/"));
    });

    return 0;
}
