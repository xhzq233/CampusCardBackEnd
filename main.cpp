#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"
#include "lib/CardManage/CardManage.h"
#include "lib/Consume/Consume.h"
#include "lib/Container/CircularArray.h"

typedef std::function<void(void)> VoidCallBack;

void testTimeWrapper(const VoidCallBack &func) {

    auto t1 = std::chrono::high_resolution_clock::now();

    func();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;
    printf("use time: %f ms.\n", ms.count());
}

using CSV = FileManager::CSV;

constexpr static const unsigned int RESERVED_SIZE = 2'500'000;
typedef BaseOperation *SortedOperations[RESERVED_SIZE];

void init() {
    // MARK:--- init operations
    auto *operations = new SortedOperations{nullptr};
    auto &res = DataStore::getConsumptions();
    unsigned int num = 0;
    for (auto re: res)
        re->for_loop([&](auto value) {
            operations[num++] = value;
        });

    CSV temp;

    FileManager::getInstance().getCSVDataSource(temp, 3, FileManager::CARD_MANAGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new CardManageOperation(item);
    temp.clear();

    FileManager::getInstance().getCSVDataSource(temp, 4, FileManager::CARD_RECHARGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new RechargeOperation(item);
    temp.clear();

    std::sort(operations, operations + num, [](BaseOperation *l, BaseOperation *r) -> bool {
        return (*l) < (*r);
    });
    // sort complete


    RechargeOperation *rechargeOperation;
    Consumption *consumption;
    CardManageOperation *cardManageOperation;

    // start_index operation
    using namespace Consume;
    for (int i = 0; i < num; ++i) {
        if ((rechargeOperation = dynamic_cast<RechargeOperation *>(operations[i]))) {
            CardManage::recharge(rechargeOperation->uid, rechargeOperation->price, rechargeOperation->time);
        } else if ((consumption = dynamic_cast<Consumption *>(operations[i]))) {
            consume(*consumption);
        } else if ((cardManageOperation = dynamic_cast<CardManageOperation *>(operations[i]))) {
            switch (cardManageOperation->operationName) {
                case CardManageOperation::Reissue:
                    CardManage::reissue(cardManageOperation->uid, cardManageOperation->time);
                    break;
                case CardManageOperation::Uncouple:
                    CardManage::unsetLost(cardManageOperation->uid, cardManageOperation->time);
                    break;
                case CardManageOperation::SetLoss:
                    CardManage::setLost(cardManageOperation->uid, cardManageOperation->time);
                    break;
                case CardManageOperation::Cancellation:
                    CardManage::deleteAccount(cardManageOperation->uid, cardManageOperation->time);
                    break;
                case CardManageOperation::Recharge:
                    throw;// no recharge
            }
        } else {
            throw; // "Unknown Operation"
        }
    }
    // ---
    delete[] operations;
}

void test() {
    CircularArray<int *> a(8);
    for (int i = 0; i < 6; ++i) {
        a.push_back(new int(i));
    }
    auto index = a.halfSearch([](const int *value) -> bool {
        return *value < 2;
    });
    printf("%d \n", index);
    a.insert(new int(2));
    a.insert(new int(2));
    a.insert(new int(2));
    auto index2 = a.halfSearch([](const int *value) -> bool {
        return *value < 2;
    });
    printf("%d \n", index2);
    a.for_loop([](auto value) {
        printf("%d", *value);
    });
}

void execute() {
    while (true) {
        unsigned int uid = 2020171992;
        std::string name = "夏侯臻";
        CardManage::openAccount(uid, name);
        CardManage::deleteAccount(uid);
        CardManage::openAccount(uid, name);
        CardManage::distribute(uid);
        CardManage::recharge(uid, 20);
        auto &card = DataStore::queryAccountByUid(uid)->cards.begin();
        Consume::consume(2, card, 30.0);
        int cmd;
        scanf("%d", &cmd);

        if (cmd == -1)
            break;
    }
}

int main() {
    testTimeWrapper(init);

    // Reference:
    // https://stackoverflow.com/questions/8588541/c-should-i-bother-deleting-pointers-to-application-lifetime-variables
    // 释放指针
    for (auto item: DataStore::getConsumptions()) {
        delete item;
    }
    return 0;
}
