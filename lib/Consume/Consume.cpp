//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

void Consume::consume(const Window &window, unsigned int cid, const float &price, const Consume::Time &time) {


    if (DataStore::getAccountsMapByCid().count(cid) == 0) {
        printf("No such a card");
        Consume::log(time, cid, window, price, "failed");
        return;
    }
    auto account = DataStore::getAccountsMapByCid().at(cid);
    char hour = (char) (time / 1'00'00'00 % 100);
    auto card = account->cards.begin();

    if (!card.condition) {//already set lost
        printf("Invalid card.");
        Consume::log(time, cid, window, price, "failed");

    } else if (account->balance < price) {//Insufficient balance
        printf("Insufficient account balance.");
        Consume::log(time, cid, window, price, "failed");
        //指定时间内消费
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        account->consume(price);
        DataStore::insertConsumptionOnSpecifiedTime(window, new Consumption(cid, window, time, price));
        Consume::log(time, cid, window, price, "succeeded");
    } else {
        printf("Consumption is not allowed now."); //当前时间不允许消费
        Consume::log(time, cid, window, price, "failed");
    }
}

void Consume::consume(const Window &window, unsigned int cid, const float &price) {
    auto time = FileManager::nowTime();

    if (DataStore::getAccountsMapByCid().count(cid) == 0) {
        printf("No such a card");
        Consume::log(time, cid, window, price, "failed");
        return;
    }

    auto account = DataStore::getAccountsMapByCid().at(cid);
    char hour = (char) (time / 100'000'000 % 100);
    auto card = account->cards.begin();
    if (!card.condition) {
        printf("Invalid card.\n");
        Consume::log(time, cid, window, price, "failed");
    } else if (account->balance < price) {
        printf("Insufficient account balance.\n"); //账户余额不足
        Consume::log(time, cid, window, price, "failed");
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        //当前时间段内消费超过20,则需要输入密码
        if (time - account->lastTimeEnterPasswd < GAP_TIME && account->totalConsumptionFromLastTime + price > 20) {
            if (checkPasswd(card)) {
                account->totalConsumptionFromLastTime = 0;
                account->lastTimeEnterPasswd = time;

                account->consume(price);
                DataStore::pushConsumption(window, new Consumption(cid, window, time, price));
                Consume::log(time, cid, window, price, "succeeded");
                show(window, time);
            }
            return;
        }
            //在分开时间段消费,则重置消费金额
        else if (time - account->lastTimeEnterPasswd > GAP_TIME) {
            account->totalConsumptionFromLastTime = price;
            account->lastTimeEnterPasswd = time;
            show(window, time);
        }
            //在同一时间段内消费,则累加消费金额
        else {
            account->totalConsumptionFromLastTime += price;
            show(window, time);
        }

        account->consume(price);
        DataStore::pushConsumption(window, new Consumption(cid, window, time, price));
        Consume::log(time, cid, window, price, "succeeded");

    } else {
        printf("Consumption is not allowed now."); //当前时间不允许消费
        Consume::log(time, cid, window, price, "failed");
    }
}

void Consume::consume(const Consumption &consumption) {


    if (DataStore::getAccountsMapByCid().count(consumption.cid) == 0) {
        Consume::log(consumption.time, consumption.cid, consumption.window, consumption.price,
                     "failed: No such a card");
        return;
    }

    auto account = DataStore::getAccountsMapByCid().at(consumption.cid);
    auto &card = account->cards.begin();
    char hour = (char) (consumption.time / 1000000 % 100);
    //无效的卡
    if (!card.condition) {
        Consume::log(consumption.time, card.cid, consumption.window, consumption.price, "failed: Invalid card");
        //账户余额不足
    } else if (account->balance < consumption.price) {
        Consume::log(consumption.time, card.cid, consumption.window, consumption.price,
                     "failed Insufficient account balance");
        //指定时间内消费
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        account->consume(consumption.price);
        Consume::log(consumption.time, card.cid, consumption.window, consumption.price, "succeeded");
    } else {
        //当前时间不允许消费
        Consume::log(consumption.time, card.cid, consumption.window, consumption.price, "failed time not allowed");
    }
}

void Consume::show(const Window &window) {
    const auto &consumptions = *(DataStore::getConsumptions()[window]);
    int count = 0;      //当日收费次数
    float revenue = 0; // 当日营收
    int date = (int) (FileManager::nowTime() / 10'000'000'000); //日期
    for (int i = 0; i < DataStore::MAXSIZE; ++i) {
        int _date = (int) (consumptions[i]->time / 10'000'000'000); //消费记录里的日期
        if (_date == date) {
            count++;
            revenue += consumptions[i]->price;
        }
    }
    printf("日期:%d 收费次数:%d\n营收:%f", date, count, revenue);
}

void Consume::show(const Window &window, const Time &time) {
    const auto &consumptions = *(DataStore::getConsumptions()[window]);
    int count = 0;//指定时间的收费次数
    float revenue = 0; // 指定时间营收
    int date = (int) (time / 100000000); //日期
    for (int i = 0; i < DataStore::MAXSIZE; ++i) {
        int _date = (int) (consumptions[i]->time / 10'000'000'000);
        if (_date == date) {
            count++;
            revenue += consumptions[i]->price;
        }
    }
    printf("日期:%d 收费次数:%d\n营收:%.2f", date, count, revenue);
}

bool Consume::checkPasswd(const Card &card) {
    //限制5次输入密码
    for (int i = 0; i < 5; ++i) {
        char password[10];
        printf("Please input your password:");
        scanf("%s", password); //输入密码
        if (card.checkPassword(strtol(password, nullptr, 10))) {
            printf("Password entered successfully.");
            return true;
        }
    }
    printf("Incorrect password.");
    return false;
}
