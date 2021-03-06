#include "main.h"

using namespace Main;

int main() {
    {
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
                       "9: Init data        10: Query consumption\n"
                       "11: Query uid       12: Analyze consumption\n"
                       "13: Query name      14: Most Frequent Window\n"
                       "-1: Exit \n"
                       "Input command: "
                );
                scanf("%s", str);
                cmd = std::stoi(str);
                switch (cmd) {
                    //开户
                    case 1: {
                        printf("Registering an account is in progress.\nplease input your uid: ");
                        scanf("%s", str);
                        uid = std::stol(str);
                        printf("Please input your name: ");
                        scanf("%s", str);
                        cid = CardManage::openAccount(uid, str);
                        if (cid == -1) {
                            printf("Failed: the uid format is wrong.\n");
                        } else if (cid == 0) {
                            printf("Failed: account already in system.\n");
                        } else {
                            printf("Succeeded: your new card id is %u.\n ", cid);
                        }
                        break;
                    }
                        //销户
                    case 2: {
                        printf("Account cancellation operation is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        res = CardManage::account_cancellation(uid);
                        if (res) {
                            printf("Succeeded.\n");
                        } else {
                            printf("Failed: uid is not in system.\n");
                        }
                        break;
                    }
                        //挂失
                    case 3: {
                        printf("Reporting loss is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        res = CardManage::setLost(uid);
                        if (res == 1) {
                            printf("Succeeded.\n");
                        } else if (res == -1) {
                            printf("Failed: card already lost.\n");
                        } else {
                            printf("Failed: uid is not in system.\n");
                        }
                        break;
                    }
                        //解挂
                    case 4: {
                        printf("Loss unregister is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        res = CardManage::unsetLost(uid);
                        if (res == 1) {
                            printf("Succeeded.\n");
                        } else if (res == -1) {
                            printf("Failed: card no lost yet.\n");
                        } else {
                            printf("Failed: uid is not in system.\n");
                        }
                        break;
                    }
                        //补卡
                    case 5: {
                        printf("Reissuing a card is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        res = CardManage::reissue(uid);
                        if (res == 0) {
                            printf("Failed: uid is not in system.\n");
                        } else if (res == -1) {
                            printf("Failed: the number of reissuing card reached the upper limit.\n");
                        } else {
                            printf("Succeeded: new card cid:%u.", res);
                        }
                        break;
                    }
                        //充值
                    case 6: {
                        printf("A recharge operation is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        printf("please input the amount you want to recharge:");
                        scanf("%s", str);
                        amount = std::stoi(str);
                        res = CardManage::recharge(uid, amount);
                        if (res == 0) {
                            printf("Failed: the uid is not in system.\n");
                        } else if (res == -1) {
                            printf("Failed: the account balance exceeds 1000.\n");
                        } else {
                            printf("Succeeded: the account balance:%d.\n", res);
                        }
                        break;
                    }
                        //消费
                    case 7: {
                        printf("A consumption operation is in progress.\nPlease choose the window you want to consume:");
                        scanf("%s", str);
                        window = std::stoi(str);
                        printf("Please input your cid:");
                        scanf("%s", str);
                        cid = std::stol(str);
                        printf("Please input the amount you want to consume:");
                        scanf("%s", str);
                        price = std::stof(str);
                        printf("Insert in specified time? y or n\n");
                        scanf("%s", str);
                        if (std::string(str) == "y") {
                            printf("Input specified time: (format be like 2021092411002598)");
                            scanf("%s", str);
                            time = std::stoll(str);
                            res = Consume::consume(window, cid, price, time);
                            if (res == 0) {
                                printf("No such a card.\n");
                            } else if (res == 1) {
                                printf("Invalid card.\n");
                            } else if (res == 2) {
                                printf("Insufficient account balance.\n");
                            } else if (res == 3) {
                                printf("Succeeded.\n");
                            } else if (res == 4) {
                                printf("Consumption is not allowed now.\n");
                            }
                        } else if (std::string(str) == "n") {
                            res = Consume::consume(window, cid, price);
                            if (res == 0) {
                                printf("No such a card.\n");
                            } else if (res == 1) {
                                printf("Invalid card.\n");
                            } else if (res == 2) {
                                printf("Insufficient account balance.\n");
                            } else if (res == 3) {
                                printf("Succeeded.\n");
                            } else if (res == 4) {
                                printf("Consumption is not allowed now.\n");
                            }
                        } else {
                            printf("Undefined cmd.\n");
                            break;
                        }
                        break;
                    }
                        //查询信息
                    case 8: {
                        printf("Querying an account is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        auto account = DataStore::subscript2Account(DataStore::queryAccountByUid(uid));
                        if (account)
                            printf("%s", account->to_string().c_str());
                        else
                            printf("No such an account.\n");
                        break;
                    }
                        //初始化数据
                    case 9: {
                        printf("Initializing...\n");
                        testTimeWrapper(init);
                        break;
                    }
                        //消费记录总额查询
                    case 10: {
                        printf("Querying consumption is in progress.\nPlease input your uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        DataAnalyze::Time begin, end;
                        printf("Please input the beginning time:");
                        scanf("%s", str);
                        begin = std::stoull(str);
                        printf("Please input the end time:");
                        scanf("%s", str);
                        end = std::stoull(str);
                        float total = DataAnalyze::accumulatedConsumption(uid, begin, end);
                        printf("The account with uid %u \nspent a total of %.2f yuan during this time frame.\n", uid,
                               total);
                        break;
                    }
                        //模糊查询
                    case 11: {
                        printf("Please input fuzzy uid:");
                        scanf("%s", str);
                        auto results = DataAnalyze::fuzzyQueryOnUid(std::regex(str));
                        if (results.empty()) {
                            printf("No results found.\n");
                        } else {
                            printf("A total of %lu results have been found.\n", results.size());
                            for (auto &&result: results) {
                                printf("%d\n", result);
                            }
                        }
                        break;
                    }
                    case 12: {
                        printf("Please input the uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        auto results = DataAnalyze::analyze(uid);
                        printf("Here are the probable friends:\n");
                        for (auto &&result: results) {
                            printf("count: %d, id: %d ", result.count, result.student_id);
                        }
                        printf("\n");
                        break;
                    }
                    case 13: {
                        printf("Please input the name:");
                        scanf("%s", str);
                        auto results = DataAnalyze::fuzzyQueryOnName(std::regex(str));
                        if (results.empty()) {
                            printf("No results found.\n");
                        } else {
                            printf("A total of %lu results have been found.\n", results.size());
                            for (auto &&result: results) {
                                printf("%d\n", result);
                            }
                        }
                        break;
                    }
                    case 14: {
                        printf("Input the uid:");
                        scanf("%s", str);
                        uid = std::stol(str);
                        auto r = DataAnalyze::mostFrequentWindowOfSomeone(uid);
                        printf("window: %u,count: %u\n", r.first, r.second);
                        break;
                    }
                    case -1: {
                        break;
                    }
                    default: {
                        printf("Undefined cmd.\n");
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

        // Reference:
        // https://stackoverflow.com/questions/8588541/c-should-i-bother-deleting-pointers-to-application-lifetime-variables
        // 释放指针
        for (auto item: DataStore::getConsumptions()) {
            delete item;
        }
    }
    return 0;
}