#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"

typedef std::function<void(void)> VoidCallBack;

void testTimeWrapper(const VoidCallBack &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "use time: " << ms.count() << " ms." << std::endl;
}

using CSV = FileManager::CSV;

void description(const CSV &csv){
    auto columns = csv[0].size();
    for (const auto & i : csv) {
        std::string buf;
        for (int j = 0; j < columns; ++j) {
            buf.append(i[j]);
        }
        std::cout<<buf<<std::endl;
    }
}

int main() {
    VoidCallBack func{
            [&]() {
//                CSV csv;
//                if (FileManager::getInstance().getCSVDataSource(csv, Pair((unsigned int) 1937, (unsigned int) 4),
//                                                                FileManager::CONSUME_CSV(1)));
//                else std::cout << "err1" << std::endl;

//                DataStore::insertAccount(Account(0, ""));
                auto & consumes = DataStore::getConsumes();
                auto columns = 1;
                std::cout<<std::to_string(consumes.size())<<std::endl;

                for (const auto & i : consumes) {
                    if (!i[0]) continue;
                    for (int j = 0; j < columns; ++j) {
                        std::cout<<i[j]->to_string()<<std::endl;
                    }
                }
//        FileManager::getInstance() << FileManager::toStandardLogString("THIS IS TITLE", "AND content here")
//                                   << FileManager::endl;
//        if (FileManager::getInstance().writeCSVData(csv, "xhzq.csv", "../adjygvjsafvj/"));
            }
    };

    testTimeWrapper(func);

    //释放指针
    for (const auto &item : DataStore::getConsumes()){
        for (const auto &i : item)
            delete i;
    }
    return 0;
}
