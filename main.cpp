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
                    throw;// no recharge_account
            }
        } else {
            throw; // "Unknown Operation"
        }
    }
    // ---
    delete[] operations;
}

void execute() {
    int cmd;
    int res;
    char str[20];
    unsigned int uid;
    unsigned int cid;
    std::string name;
    int amount;
    CardManage::Time time;
    unsigned int window;
    float price;
    while (true) {
        try {
            printf("1: Register account 2: Account cancellation \n"
                   "3: SetLoss          4: UnsetLost \n"
                   "5: Reissue          6: Recharge \n"
                   "7: Consume          8: Account description \n"
                   "9. Init data        -1: Exit \nInput command: \n"
            );
            scanf("%s", str);
            cmd = std::stoi(str);
            switch (cmd) {
                //开户
                case 1: {
                    printf("Registering an account is in progress.\nplease input your uid: ");
                    scanf("%s", str);
                    uid = std::stol(str);
                    printf("Please input ur name: ");
                    scanf("%s", str);
                    name = str;
                    cid = CardManage::openAccount(uid, name);
                    if (cid) {
                        printf("Succeeded: ur new card id is %u\n ", cid);
                    } else {
                        printf("Failed: account already in system\n");
                    }
                    break;
                }
                    //销户
                case 2: {
                    printf("Account cancellation operation is in progress.\nInput ur uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::deleteAccount(uid);
                    if (res) {
                        printf("Succeeded \n");
                    } else {
                        printf("Failed: uid is not in system\n");
                    }
                    break;
                }
                    //挂失
                case 3: {
                    printf("Reporting loss is in progress.\nInput ur uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::setLost(uid);
                    if (res == 1) {
                        printf("succeeded\n");
                    } else if (res == -1) {
                        printf("Failed: card already lost\n");
                    } else {
                        printf("Failed: uid is not in system\n");
                    }
                    break;
                }
                    //解挂
                case 4: {
                    printf("Loss unregister is in progress.\nInput ur uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::unsetLost(uid);
                    if (res == 1) {
                        printf("succeeded\n");
                    } else if (res == -1) {
                        printf("Failed: card no lost yet\n");
                    } else {
                        printf("Failed: uid is not in system\n");
                    }
                    break;
                }
                    //补卡
                case 5: {
                    printf("Reissuing a card is in progress.\nInput your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::reissue(uid);
                    if (res == 1) {
                        printf("succeeded\n");
                    } else if (res == -1) {
                        printf("Failed: the number of reissuing card reached the upper limit\n");
                    } else {
                        printf("Failed: uid is not in system\n");
                    }
                    break;
                }
                    //充值
                case 6: {
                    printf("A recharge operation is in progress.\nInput your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    printf("please input the amount you want to recharge:");
                    scanf("%s", str);
                    amount = std::stoi(str);
                    res = CardManage::recharge(uid, amount);
                    if (res == 0) {
                        printf("Failed: the uid is not in system\n");
                    } else if (res == -1) {
                        printf("Failed: the account balance exceeds 1000.\n");
                    } else {
                        printf("Succeeded: the account balance:%d\n", res);
                    }
                    break;
                }
                    //消费
                case 7: {
                    printf("A consumption operation is in progress.\nPlease choose the window you want to consume:");
                    scanf("%s", str);
                    window = std::stoi(str);
                    printf("Input your cid:");
                    scanf("%s", str);
                    cid = std::stol(str);
                    printf("Please input the amount you want to consume:");
                    scanf("%s", str);
                    price = std::stof(str);
                    printf("Insert in specified time? y or n");
                    scanf("%s", str);
                    if (std::string(str) == "y") {
                        printf("Input specified time: (format be like 2021092411002598)");
                        scanf("%s", str);
                        time = std::stoll(str);
                        Consume::consume(window, cid, price, time);
                    } else if (std::string(str) == "n") {
                        Consume::consume(window, cid, price);
                    } else {
                        printf("Undefined cmd\n");
                        break;
                    }
                    break;
                }
                case 8: {
                    printf("Please input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    auto account = DataStore::queryAccountByUid(uid);
                    printf("name:%s, uid:%u, balance:%.2f cards:\n%s",
                           account->name, account->uid, account->balance,
                           account->cards.to_string().c_str());
                    break;
                }
                case 9: {
                    printf("Initializing...\n");
                    testTimeWrapper(init);
                }
                    //退出
                case -1: {
                    break;
                }
                    //出错
                default: {
                    printf("Undefined cmd\n");
                    break;
                }
            }
            if (cmd == -1) {
                break;
            }
        } catch (std::exception &exception) {
            printf("%s \n", exception.what());
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
