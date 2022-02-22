#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"

void testTimeWrapper(const std::function<void(void)> &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "use time: " << ms.count() << " ms." << std::endl;
}

using CSV = FileManager::CSV;

int main() {
    testTimeWrapper([&]() {

//        CSV csv;
//        if (FileManager::getInstance().getCSVDataSource(csv, Pair((unsigned int) 1937, (unsigned int) 4),
//                                                        FileManager::CONSUME_CSV(1)));
//        else std::cout << "err1" << std::endl;
        DataStore::getConsumes();
//        DataStore::insertAccount(Account(0, ""));
//        DataStore::getConsumes();
//        FileManager::getInstance() << FileManager::toStandardLogString("THIS IS TITLE", "AND content here")
//                                   << FileManager::endl;
//        if (FileManager::getInstance().writeCSVData(csv, "xhzq.csv", "../adjygvjsafvj/"));
    });

    return 0;
}
