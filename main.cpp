#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"
#include "lib/CardManage/CardManage.h"
#include "lib/Consume/Consume.h"

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

constexpr static const unsigned int RESERVED_SIZE = 2'500'000;
typedef BaseOperation *SortedOperations[RESERVED_SIZE];

void initialize_data() {
    // MARK:--- init operations
    SortedOperations operations{nullptr};
    auto &res = DataStore::getConsumptions();
    unsigned int num = 0;
    for (auto &re: res)
        for (const auto &item: re) {
            if (!item) break;
            operations[num++] = item;
        }
    CSV temp;

    FileManager::getInstance().getCSVDataSource(temp, 3, FileManager::CARD_MANAGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new CardManageOperation(item);
    temp.clear();

    FileManager::getInstance().getCSVDataSource(temp, 4, FileManager::CARD_RECHARGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new RechargeOperation(item);

    std::sort(operations, operations + num, [](BaseOperation *l, BaseOperation *r) -> bool {
//        if (l && r)
        return (*l) < (*r);
//        else
//            return l == nullptr;
    });
    // sort complete
    RechargeOperation *rechargeOperation;
    Consumption *consumption;
    CardManageOperation *cardManageOperation;
    for (int i = 0; i < num; ++i) {
        if (rechargeOperation = dynamic_cast<RechargeOperation *>(operations[i])) {

        } else if (consumption = dynamic_cast<Consumption *>(operations[i])) {

        } else if (cardManageOperation = dynamic_cast<CardManageOperation *>(operations[i])) {

        }
    }
    // ---
}

int main() {
//    VoidCallBack func{
//            [&]() {
//                CSV csv;
//                if (FileManager::getInstance().getCSVDataSource(csv, 4,
//                                                                FileManager::CARD_RECHARGE_CSV_NAME));
//                else printf("err");
//                description(csv);
//                DataStore::insertAccount(Account(0, ""));
//                description(DataStore::getConsumptions());
 //               description(DataStore::getConsumptions());
//                initialize_data();
//                DataStore::queryConsumption(1,43532);
//                description(DataStore::getAccounts());
//                FileManager::getInstance() << FileManager::toStandardLogString("THIS IS TITLE", "AND content here")
//                                           << FileManager::endl;
//                if (FileManager::getInstance().writeCSVData(csv, "xhzq.csv", "../adjygvjsafvj/"));
//            }
//    };
//
//    testTimeWrapper(func);

//    释放指针
//    for (const auto &item: DataStore::getConsumptions()) {
//        for (const auto &i: item)
//            if (i)
//                delete i;
//    }
    unsigned int uid = 202017184;
    std::string name = "夏侯臻";
    CardManage::openAccount(uid, name);
    CardManage::distribute(uid);
    CardManage::recharge(uid, 20.0);
    Consume::consume(2,*DataStore::queryAccountByUid((uid))->cards.begin(),10.0);
    return 0;
}
