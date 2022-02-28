//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

void
Consume::baseConsume(Account &account, const Window &window, const Card &card, const float &price, const Time &time) {
    account.consume(price);
    DataStore::pushConsumption(window, new Consumption(card.cid, window, time, price));
    Consume::log(time, card.cid, window, price, "succeeded");
}

void Consume::consume(const Window &window, const Card &card, const float &price) {
    auto time = FileManager::nowTime();
    int hour = static_cast<int>(time / 100'000'000 % 100);
    auto account = DataStore::queryAccountByUid(card.cid);
    if (!card.condition) {
        printf("Invalid card.\n");
        Consume::log(time, card.cid, window, price, "failed");
    } else if (account->balance < price) {
        printf("Insufficient account balance.\n"); //账户余额不足
        Consume::log(time, card.cid, window, price, "failed");
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        //当前时间段内消费超过20,则需要输入密码
        if (time / 1000000 - account->lastTime < 2 && account->lastTotal + price > 20) {
            if (checkPasswd(card)) {
                account->lastTotal = 0;
                account->lastTime = time / 1000000;
                Consume::baseConsume(*account, window, card, price, time);
                show(window, time);
            }
        }
            //在分开时间段消费,则重置消费金额
        else if (time / 1000000 - account->lastTime > 2) {
            account->lastTotal = price;
            account->lastTime = time / 1000000;
            Consume::baseConsume(*account, window, card, price, time);
            show(window, time);
        }
            //在同一时间段内消费,则累加消费金额
        else {
            account->lastTotal += price;
            account->lastTime = time / 1000000;
            Consume::baseConsume(*account, window, card, price, time);
            show(window, time);
        }
    } else {
        printf("Consumption is not allowed now."); //当前时间不允许消费
        Consume::log(time, card.cid, window, price, "failed");
    }
}

//void Consume::consume(const Window &window, const Card &card, const float &price, const Time &time) {
//    int hour = (int) (time / 1000000 % 100);
//    auto account = DataStore::queryAccountByUid(card.cid);
//    //无效的卡
//    if (!card.condition) {
//        printf("Invalid card.");
//        Consume::log(time, card.cid, window, price, "failed");
//        //账户余额不足
//    } else if (account->balance < price) {
//        printf("Insufficient account balance.");
//        Consume::log(time, card.cid, window, price, "failed");
//        //指定时间内消费
//    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
//        //当前时间段内消费超过20,则需要输入密码
//        if (time / 1000000 - account->lastTime < 2 && account->lastTotal + price > 20) {
//            if (checkPasswd(card)) {
//                account->lastTotal = 0;
//                account->lastTime = time / 1000000;
//                Consume::baseConsume(*account, window, card, price, time);
//                show(window, time);
//            }
//        }
//            //在分开时间段消费,则重置消费金额
//        else if (time / 1000000 - account->lastTime > 2) {
//            account->lastTotal = price;
//            account->lastTime = time / 1000000;
//            Consume::baseConsume(*account, window, card, price, time);
//            show(window, time);
//        }
//            //在同一时间段内消费,则累加消费金额
//        else {
//            account->lastTotal += price;
//            account->lastTime = time / 1000000;
//            Consume::baseConsume(*account, window, card, price, time);
//            show(window, time);
//        }
//    } else {
//        printf("Consumption is not allowed now."); //当前时间不允许消费
//        Consume::log(time, card.cid, window, price, "failed");
//    }
//}

void Consume::consume(const Consumption &log) {
    auto account = DataStore::queryAccountByCid(log.cid);
    auto card = account->cards.begin();
    int hour = (int) (log.time / 1000000 % 100);
    //无效的卡
    if (!card.condition) {
        Consume::log(log.time, card.cid, log.window, log.price, "failed Invalid card");
        //账户余额不足
    } else if (account->balance < log.price) {
        Consume::log(log.time, card.cid, log.window, log.price, "failed Insufficient account balance");
        //指定时间内消费
    } else if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour <= 19) {
        //当前时间段内消费超过20,则需要输入密码
        if (log.time / 1000000 - account->lastTime < 2 && account->lastTotal + log.price > 20) {
            if (checkPasswd(card)) {
                account->lastTotal = 0;
                account->lastTime = log.time / 1000000;
                Consume::baseConsume(*account, log.window, card, log.price, log.time);
            }
        }
            //在分开时间段消费,则重置消费金额
        else if (log.time / 1000000 - account->lastTime > 2) {
            account->lastTotal = log.price;
            account->lastTime = log.time / 1000000;
            Consume::baseConsume(*account, log.window, card, log.price, log.time);
        }
            //在同一时间段内消费,则累加消费金额
        else {
            account->lastTotal += log.price;
            account->lastTime = log.time / 1000000;
            Consume::baseConsume(*account, log.window, card, log.price, log.time);
        }
    } else {
        //当前时间不允许消费
        Consume::log(log.time, card.cid, log.window, log.price, "failed time not allowed");
    }
}

void Consume::show(const Window &window) {
    const auto &consumptions = *(DataStore::getConsumptions()[window]);
    int count = 0;      //当日收费次数
    float revenue = 0; // 当日营收
    int date = static_cast<int>(FileManager::nowTime() / 10'000'000'000); //日期
    for (int i = 0; i < DataStore::MAXSIZE; ++i) {
        int _date = static_cast<int>(consumptions[i]->time / 10'000'000'000); //消费记录里的日期
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
        int _date = static_cast<int>(consumptions[i]->time / 10'000'000'000);
        if (_date == date) {
            count++;
            revenue += consumptions[i]->price;
        }
    }
    printf("日期:%d 收费次数:%d\n营收:%.2f", date, count, revenue);
}

void Consume::log(const Time &time, unsigned int cid, Window window, float price, const char *res) {
    static char content[70];
    sprintf(content, "%llu %d %d %.2f %s", time, cid, window, price, res);
    FileManager::getInstance() << FileManager::toStandardLogString("消费", content, time);
}

bool Consume::checkPasswd(const Card &card) {
    //限制5次输入密码
//    for (int i = 0; i < 5; ++i) {
//        char password[10];
//        printf("Please input your password:");
//        scanf("%s", password); //输入密码
//        if (card.checkPassword(strtol(password, nullptr, 10))) {
//            printf("Password entered successfully.");
//            return true;
//        }
//    }
//    printf("Incorrect password.");
//    return false;
    return true;
}