#include "main.h"

using namespace Main;

std::vector<unsigned int> fuzzyQueryOnName(const std::regex &re) {
    std::vector<unsigned int> res;
    for (auto &account: DataStore::getAccounts()) {
        if (std::regex_match(account->name, re))
            res.emplace_back(account->uid);
    }
    return res;
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
            printf("1: 开户             2: 销户 \n"
                   "3: 挂失             4: 解挂 \n"
                   "5: 补卡             6: 充值 \n"
                   "7: 消费             8: 查询账户信息 \n"
                   "9: 初始数据         10: 查询总消费 \n"
                   "11: 查询学号        12: 分析5个最经常一起吃饭的同学 \n"
                   "13: 查询名字        14: 分析最经常去的窗口\n"
                   "-1: 退出 \n"
                   "Input command: "
            );
            scanf("%s", str);
            cmd = std::stoi(str);
            switch (cmd) {
                case 1: {
                    printf("开户.\nplease input your uid: ");
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
                        printf("Success: your new card id is %u.\n ", cid);
                    }
                    break;
                }
                case 2: {
                    printf("销户\nPlease input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::account_cancellation(uid);
                    if (res) {
                        printf("Success.\n");
                    } else {
                        printf("Failed: uid is not in system.\n");
                    }
                    break;
                }
                case 3: {
                    printf("挂失\nPlease input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::setLost(uid);
                    if (res == 1) {
                        printf("Success.\n");
                    } else if (res == -1) {
                        printf("Failed: card already lost.\n");
                    } else {
                        printf("Failed: uid is not in system.\n");
                    }
                    break;
                }
                case 4: {
                    printf("解挂\nPlease input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::unsetLost(uid);
                    if (res == 1) {
                        printf("Success.\n");
                    } else if (res == -1) {
                        printf("Failed: card no lost yet.\n");
                    } else {
                        printf("Failed: uid is not in system.\n");
                    }
                    break;
                }
                case 5: {
                    printf("补卡.\nPlease input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    res = CardManage::reissue(uid);
                    if (res == 0) {
                        printf("Failed: uid is not in system.\n");
                    } else if (res == -1) {
                        printf("Failed: the number of reissuing card reached the upper limit.\n");
                    } else {
                        printf("Success: new card cid:%u.", res);
                    }
                    break;
                }
                case 6: {
                    printf("充值\nPlease input your uid:");
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
                        printf("Success: the account balance:%d.\n", res);
                    }
                    break;
                }
                case 7: {
                    printf("消费.\nPlease choose the window you want to consume:");
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
                            printf("Success.\n");
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
                            printf("Success.\n");
                        } else if (res == 4) {
                            printf("Consumption is not allowed now.\n");
                        }
                    } else {
                        printf("Undefined cmd.\n");
                        break;
                    }
                    break;
                }
                case 8: {
                    printf("查询账户信息\nPlease input your uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    auto account = DataStore::subscript2Account(DataStore::queryAccountByUid(uid));
                    if (account)
                        printf("%s", account->to_string().c_str());
                    else
                        printf("No such an account.\n");
                    break;
                }
                case 9: {
                    printf("初始化数据\n");
                    testTimeWrapper(init);
                    break;
                }
                case 10: {
                    printf("消费记录总额查询（时间段） \nPlease input your uid:");
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
                    printf("Query fuzzy uid is in progress.\nPlease input fuzzy uid:");
                    scanf("%s", str);
                    std::string s(str);
                    auto results = DataAnalyze::fuzzyQueryOnUid(s);
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
                    printf("Analyze consumption is in progress.\nPlease input the uid:");
                    scanf("%s", str);
                    uid = std::stol(str);
                    auto results = DataAnalyze::analyze(uid);
                    printf("Here are the probable friends:\n");
                    for (auto &&result: results) {
                        printf("%d ,count:%d\n", result.student_id, result.count);
                    }
                    printf("\n");
                    break;
                }
                case 13: {
                    printf("Query fuzzy name is in progress.\nPlease input fuzzy name:");
                    scanf("%s", str);
                    auto results = fuzzyQueryOnName(std::regex(str));
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
                    printf("输入uid:\n");
                    scanf("%s", str);
                    uid = std::stol(str);
                    auto r = DataAnalyze::mostFrequentWindowOfSomeone(uid);
                    printf("最经常去 Window:%d,count:%d\n", r.first, r.second);
                    break;
                }
                case -1: {
                    break;
                }
                    //出错
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

int main() {
    execute();
    return 0;
}
