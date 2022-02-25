//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

void Consume::consume(const Window &window, const Card &card, const float &price) {
    auto time = FileManager::to_time();
    int hour = static_cast<int>(time / 100'000'000 % 100);
    auto account = DataStore::queryAccountByUid(card.cid);
    if (!card.condition) {
        printf("Invalid card.");
    } else if (account->balance > price) {
        printf("Insufficient account balance."); //账户余额不足
    } else {
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
            //免密支付时间
            if (price <= 20 || checkPasswd(card)) {
                //价格20内或者20以上输入密码正确
                account->consume(price);
                DataStore::insertConsumption(window, new Consumption(card.cid, window, time, price));
                show(window);
            }
        } else {
            printf("Consumption is not allowed now."); //当前时间不允许消费
        }
    }
}

void Consume::consume(const Window &window, const Card &card, const float &price, const string &time) {
    int hour = stoi(time.substr(8, 2));
    auto account = DataStore::queryAccountByUid(card.cid);
    if (!card.condition) {
        printf("Invalid card.");
    } else if (account->balance < price) {
        printf("Insufficient account balance."); //账户余额不足
    } else {
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
            //免密支付时间
            if (price <= 20 || checkPasswd(card)) {
                //价格20内或者20以上输入密码正确
                account->consume(price);
                DataStore::insertConsumption(window, new Consumption(card.cid, window, std::stoull(time), price));
                show(window, time);
            }
        } else {
            printf("Consumption is not allowed now."); //当前时间不允许消费
        }
    }
}

void Consume::consume(const Consumption &log) {
    auto account = DataStore::queryAccountByUid(log.cid);
    account->consume(log.price);
}

void Consume::show(const Window &window) {
    auto consumptions = DataStore::getConsumptions()[window];
    int count = 0;      //当日收费次数
    float revenue = 0; // 当日营收
    int date = static_cast<int>(FileManager::to_time() / 10'000'000'000); //日期
    for (int i = 0; i < DataStore::MAXSIZE; ++i) {
        int _date = static_cast<int>(consumptions[i]->time / 10'000'000'000); //消费记录里的日期
        if (_date == date) {
            count++;
            revenue += consumptions[i]->price;
        }
    }
    printf("日期:%d 收费次数:%d\n营收:%f", date, count, revenue);
}

void Consume::show(const Window &window, const std::string &time) {
    auto consumptions = DataStore::getConsumptions()[window];
    int count = 0;//指定时间的收费次数
    float revenue = 0; // 指定时间营收
    int date = stoi(time.substr(0, 8)); //日期
    for (int i = 0; i < DataStore::MAXSIZE; ++i) {
        int _date = static_cast<int>(consumptions[i]->time / 10'000'000'000);
        if (_date == date) {
            count++;
            revenue += consumptions[i]->price;
        }
    }
    printf("日期:%d 收费次数:%d\n营收:%f", date, count, revenue);
}

bool Consume::checkPasswd(const Card &card) {
    //限制5次输入密码
    for (int i = 0; i < 5; ++i) {
        char password[10];
        printf("Please input your password:");
        scanf("%s", password); //输入密码
        if (card.checkPassword(strtol(password, nullptr,10))) {
            printf("Password entered successfully.");
            return true;
        }
    }
    printf("Incorrect password.");
    return false;
}