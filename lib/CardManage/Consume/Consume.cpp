//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

int Consume::consume(const Window &window, unsigned int cid, const float &price, const Consume::Time &time) {
    if (DataStore::getAccountsMapByCid().count(cid) == 0) {
        printf("No such a card.\n");
        Consume::log(time, cid, window, price, "failed");
        return 0;
    }
    auto account = DataStore::getAccountsMapByCid().at(cid);
    char hour = (char) (time / 1'00'00'00 % 100);
    auto card = account->cards.begin();
    if (!card.condition) {//already set lost
        Consume::log(time, cid, window, price, "failed");
        return 1;

    } else if (account->balance < price) {//Insufficient balance
        Consume::log(time, cid, window, price, "failed");
        return 2;
        //指定时间内消费
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        account->consume(price);
        DataStore::insertConsumptionOnSpecifiedTime(window, new Consumption(cid, window, time, price));
        Consume::log(time, cid, window, price, "succeeded");
        return 3;
    } else {
        //当前时间不允许消费
        Consume::log(time, cid, window, price, "failed");
        return 4;
    }
}

int Consume::consume(const Window &window, unsigned int cid, const float &price) {
    auto time = FileManager::nowTime();
    if (DataStore::getAccountsMapByCid().count(cid) == 0) {
        Consume::log(time, cid, window, price, "failed");
        return 0;
    }
    auto account = DataStore::getAccountsMapByCid().at(cid);
    char hour = (char) (time / 100'000'000 % 100);
    auto &card = account->cards.begin();
    if (!card.condition) {
        Consume::log(time, cid, window, price, "failed");
        return 1;
    } else if (account->balance < price) {
        Consume::log(time, cid, window, price, "failed");
        return 2;
    } else if (hour >= 7 && hour <= 23) {
        //当前时间段内消费超过20,则需要输入密码
        if (time - account->lastTimeEnterPasswd < GAP_TIME && account->totalConsumptionFromLastTime + price > 20) {
            if (checkPasswd(card)) {
                account->totalConsumptionFromLastTime = 0;
                account->lastTimeEnterPasswd = time;
                account->consume(price);
                DataStore::pushConsumption(window, new Consumption(cid, window, time, price));
                Consume::log(time, cid, window, price, "succeeded");
                show(window, time);
                return 3;
            } else {
                return 5;
            }
        } else if (time - account->lastTimeEnterPasswd > GAP_TIME) {//在分开时间段消费,则重置消费金额
            account->totalConsumptionFromLastTime = price;
            account->lastTimeEnterPasswd = time;
        } else {//在同一时间段内消费,则累加消费金额
            account->totalConsumptionFromLastTime += price;
        }
        account->consume(price);
        DataStore::pushConsumption(window, new Consumption(cid, window, time, price));
        Consume::log(time, cid, window, price, "succeeded");
        show(window, time);
        return 3;
    } else { //当前时间不允许消费
        Consume::log(time, cid, window, price, "failed");
        return 4;
    }
}

void
Consume::consume(CircularArray<Consumption *> *container, const Consumption *consumption) {

    if (DataStore::getAccountsMapByCid().count(consumption->cid) == 0) {
        Consume::log(consumption->time, consumption->cid, consumption->window, consumption->price,
                     "failed: No such a card");
        return;
    }
    auto account = DataStore::getAccountsMapByCid().at(consumption->cid);
    auto &card = account->cards.begin();
    char hour = (char) (consumption->time / 1000000 % 100);
    if (!card.condition) {//invalid
        Consume::log(consumption->time, card.cid, consumption->window, consumption->price, "failed: Invalid card");
    } else if (account->balance < consumption->price) {
        Consume::log(consumption->time, card.cid, consumption->window, consumption->price,
                     "failed Insufficient account balance");
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        account->consume(consumption->price);
        container->push_back(new Consumption(*consumption));
        Consume::log(consumption->time, card.cid, consumption->window, consumption->price, "success");
    } else {
        Consume::log(consumption->time, card.cid, consumption->window, consumption->price, "failed time not allowed");
    }
}

void Consume::show(const Window &window) {
    const auto &consumptions = *(DataStore::getConsumptions()[window - 1]);
    int count = 0;      //当日收费次数
    float revenue = 0; // 当日营收
    int date = (int) (FileManager::nowTime() / 100'000'000); //日期
    consumptions.for_loop([&](auto _, auto value) {
        int _date = (int) (value->time / 100'000'000);
        if (_date == date) {
            count++;
            revenue += value->price;
        }
    });
    printf("日期:%d 收费次数:%d\n营收:%.2f\n", date, count, revenue);
}

void Consume::show(const Window &window, const Time &time) {
    const auto &consumptions = *(DataStore::getConsumptions()[window - 1]);
    int count = 0;//指定时间的收费次数
    float revenue = 0; // 指定时间营收
    int date = (int) (time / 100'000'000); //日期
    consumptions.for_loop([&](auto _, auto value) {
        int _date = (int) (value->time / 100'000'000);
        if (_date == date) {
            count++;
            revenue += value->price;
        }
    });
    printf("日期:%d 收费次数:%d\n营收:%.2f\n", date, count, revenue);
}

bool Consume::checkPasswd(const Card &card) {
    // upper limit:5 counts
    try {
        for (int i = 0; i < 5; ++i) {
            char password[11];
            printf("Please input your password:");
            scanf("%s", password);
            if (card.checkPassword(stoi(password))) {
                printf("Password entered successfully.\n");
                return true;
            }
        }
        printf("Incorrect password.\n");
        return false;
    } catch (exception &exception) {
        printf("%s", exception.what()); // commonly is stoi: no conversion
        return false;
    }
}
