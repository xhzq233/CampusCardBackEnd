#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"
#include "lib/CardManage/CardManage.h"
#include "lib/Consume/Consume.h"
#include "lib/Utils/MergeSort.h"

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
constexpr static const unsigned int MERGE_NUM = 64;
typedef BaseOperation *SortedOperations[RESERVED_SIZE];


void init() {
    // MARK:--- init operations
    auto *operations = new SortedOperations{nullptr};
    auto &res = DataStore::getConsumptions();
    unsigned int num = 0;

    int per_indexes[MERGE_NUM + 1]; // 记录每部分排好序的数组的下标
    int per_indexes_index = 0;
    for (auto re: res) {
        if (re->count() == 0)
            continue;
        per_indexes[per_indexes_index++] = (int) num;
        re->for_loop([&](auto value) {
            operations[num++] = value;
        });
    }
    per_indexes[per_indexes_index++] = (int) num;
    CSV temp;

    FileManager::getInstance().getCSVDataSource(temp, 3, FileManager::CARD_MANAGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new CardManageOperation(item);
    per_indexes[per_indexes_index++] = (int) num;
    temp.clear();

    FileManager::getInstance().getCSVDataSource(temp, 4, FileManager::CARD_RECHARGE_CSV_NAME);
    for (const auto &item: temp)
        operations[num++] = new RechargeOperation(item);
    per_indexes[per_indexes_index++] = (int) num;
    for (int i = per_indexes_index; i < MERGE_NUM + 1; ++i) {
        per_indexes[i] = (int) num;
    }
    temp.clear();

    MergeSort<BaseOperation *>::sort(operations, per_indexes, MERGE_NUM);

    // separate complete
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
    int cmd;
    int res;
    char str[20];
    char *buf;
    unsigned int uid;
    unsigned int cid;
    std::string name;
    int amount;
    unsigned int window;
    float price;
    while (true) {
        printf("1:register an account 2:cancel account 3:report loss 4:lose register 5:reissue 6:recharge 7:consume 9.init data -1:exit\nplease input the cmd:");
        scanf("%s", str);
        cmd = std::stoi(str);
        switch (cmd) {
            //开户
            case 1: {
                printf("Registering an account is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                printf("please input your name:");
                scanf("%s", str);
                name = str;
                cid = CardManage::openAccount(uid, name);
                if (cid) {
                    printf("succeeded\nyour new cid is %u\n", cid);
                } else {
                    printf("failed account already in system\n");
                }
                break;
            }
                //销户
            case 2: {
                printf("Account cancellation operation is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                res = CardManage::deleteAccount(uid);
                if (res) {
                    printf("succeeded\n");
                } else {
                    printf("failed the uid is not in system\n");
                }
                break;
            }
                //挂失
            case 3: {
                printf("Reporting loss is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                res = CardManage::setLost(uid);
                if (res == 1) {
                    printf("succeeded\n");
                } else if (res == -1) {
                    printf("failed card already has been lost\n");
                } else {
                    printf("failed the uid is not in system\n");
                }
                break;
            }
                //解挂
            case 4: {
                printf("Loss unregister is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                res = CardManage::unsetLost(uid);
                if (res == 1) {
                    printf("succeeded\n");
                } else if (res == -1) {
                    printf("failed card already has been found\n");
                } else {
                    printf("failed the uid is not in system\n");
                }
                break;
            }
                //补卡
            case 5: {
                printf("Reissuing a card is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                res = CardManage::reissue(uid);
                if (res == 1) {
                    printf("succeeded\n");
                } else if (res == -1) {
                    printf("failed the number of reissuing card has reached the maximum.\n");
                } else {
                    printf("failed the uid is not in system\n");
                }
                break;
            }
                //充值
            case 6: {
                printf("A recharge operation is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                printf("please input the amount you want to recharge:");
                scanf("%s", str);
                amount = strtol(str, &buf, 10);
                res = CardManage::recharge(uid, amount);
                if (res == 0) {
                    printf("failed the uid is not in system\n");
                } else if (res == -1) {
                    printf("failed the account balance exceeds 1000.\n");
                } else {
                    printf("succeeded the account balance:%d\n", res);
                }
                break;
            }
                //消费
            case 7: {
                printf("A consumption operation is in progress.\nplease choose the window you want to consume:");
                scanf("%s", str);
                window = strtol(str, &buf, 10);
                printf("please input your cid:");
                scanf("%s", str);
                cid = strtol(str, &buf, 10);
                printf("please input the amount you want to consume:");
                scanf("%s", str);
                price = strtof(str, &buf);
                auto card = DataStore::queryAccountByCid(cid)->cards.begin();
                Consume::consume(window, card, price);
                break;
            }
            case 8: {
                printf("Loss unregister is in progress.\nplease input your uid:");
                scanf("%s", str);
                uid = strtol(str, &buf, 10);
                auto account = DataStore::queryAccountByUid(uid);
                printf("name:%s,uid:%u,balance:%.2f\n", account->name, account->uid, account->balance);
                break;
            }
            case 9: {
                printf("initializing...\n");
                testTimeWrapper(init);
            }
                //退出
            case -1: {
                break;
            }
                //出错
            default: {
                printf("undefined cmd\n");
                break;
            }
        }
        if (cmd == -1) {
            break;
        }
    }
}

int main() {

    execute();

    // Reference:
    // https://stackoverflow.com/questions/8588541/c-should-i-bother-deleting-pointers-to-application-lifetime-variables
    // 释放指针
    for (auto item: DataStore::getConsumptions()) {
        delete item;
    }
    return 0;
}
