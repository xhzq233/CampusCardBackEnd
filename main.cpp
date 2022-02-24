#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"

typedef std::function<void(void)> VoidCallBack;

void testTimeWrapper(const VoidCallBack &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;
    printf("use time: %f ms.\n", ms.count());
}

using CSV = FileManager::CSV;

// print -head num
void description(const CSV &csv, int num = 5) {
    auto columns = csv[0].size();
    auto rows = csv.size();
    printf("rows: %lu, columns: %lu \n", rows, columns);
    std::string buf;
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < columns; ++j) {
            buf.append(csv[i][j]);
        }
        printf("%s \n", buf.c_str());
        buf.clear();
    }
}

// print -head num
void description(const DataStore::Consumptions &consumes, int row = 4, int col = 5) {
    for (int i = 0; i < row; ++i) {
        auto position = DataStore::getWindowPositions()[i];
        for (unsigned int j = position - col; j < position; ++j) {
            if (!consumes[i][j]) continue;
            printf("%s \n", consumes[i][j]->to_string().c_str());
        }
    }
}

// print -head num
void description(const DataStore::Accounts &accounts, int num = 5) {
    for (int i = 0; i < num; ++i)
        printf("%s \n", accounts[i].to_string().c_str());
}

int main() {
    VoidCallBack func{
            [&]() {
//                CSV csv;
//                if (FileManager::getInstance().getCSVDataSource(csv, 4,
//                                                                FileManager::CARD_RECHARGE_CSV_NAME));
//                else printf("err");
//                description(csv);
//                DataStore::insertAccount(Account(0, ""));
                description(DataStore::getConsumptions());
//                DataStore::queryConsumption(1,43532);
//                description(DataStore::getAccounts());
//                FileManager::getInstance() << FileManager::toStandardLogString("THIS IS TITLE", "AND content here")
//                                           << FileManager::endl;
//                if (FileManager::getInstance().writeCSVData(csv, "xhzq.csv", "../adjygvjsafvj/"));
            }
    };

    testTimeWrapper(func);

//    释放指针
//    for (const auto &item: DataStore::getConsumptions()) {
//        for (const auto &i: item)
//            if (i)
//                delete i;
//    }
    return 0;
}
