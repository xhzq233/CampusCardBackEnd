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

// print -head num
void description(const CSV &csv, int num = 5) {
    auto columns = csv[0].size();
    for (int i = 0; i < num; ++i) {
        std::string buf;
        for (int j = 0; j < columns; ++j) {
            buf.append(csv[i][j]);
        }
        std::cout << buf << std::endl;
    }
}

// print -head num
void description(const DataStore::Consumes &consumes, int row = 4, int col = 5) {
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) {
            if (!consumes[i][j]) continue;
            std::cout << consumes[i][j]->to_string() << std::endl;
        }
}

// print -head num
void description(const DataStore::Accounts &accounts, int num = 5) {
    for (int i = 0; i < num; ++i)
        std::cout << accounts[i].to_string() << std::endl;
}

int main() {
    VoidCallBack func{
            [&]() {
//                CSV csv;
//                if (FileManager::getInstance().getCSVDataSource(csv, Pair((unsigned int) 1937, (unsigned int) 4),
//                                                                FileManager::CONSUME_CSV(1)));
//                else std::cout << "err1" << std::endl;

//                DataStore::insertAccount(Account(0, ""));
                description(DataStore::getConsumes());
                description(DataStore::getAccounts());
//        FileManager::getInstance() << FileManager::toStandardLogString("THIS IS TITLE", "AND content here")
//                                   << FileManager::endl;
//        if (FileManager::getInstance().writeCSVData(csv, "xhzq.csv", "../adjygvjsafvj/"));
            }
    };

    testTimeWrapper(func);

    //释放指针
    for (const auto &item: DataStore::getConsumes()) {
        for (const auto &i: item)
            if (i)
                delete i;
    }
    return 0;
}
