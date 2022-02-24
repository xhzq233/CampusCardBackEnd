//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

unsigned long long to_time() {
    time_t now = time(nullptr);
    tm *t = localtime(&now);
    unsigned long long time = 0;
    time += t->tm_sec * 10'000;
    time += t->tm_min * 1'000'000;
    time += t->tm_hour * 100'000'000;
    time += t->tm_mday * 10'000'000'000;
    time += (t->tm_mon + 1) * 1'000'000'000'000;
    time += (t->tm_year + 1900) * 100'000'000'000'000;
    return time;
}

void Consume::consume(const Window &window, const Card &card, const float &price) {
    auto time = to_time();
    int hour = static_cast<int>(time / 100'000'000 % 100);
    auto account = DataStore::queryAccountByUid(card.cid);
    if (card.condition && account->balance > price) {
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour >= 19) {
            if (price <= 20 || passwd_is_correct(card)) {
                account->consume(price);
                DataStore::insertConsumption(window, new Consumption(card.cid, window, time, price));
                show(window);
            } else {

            }
        } else {

        }
    } else {

    }
}


void Consume::consume(const Window &window, const Card &card, const float &price, const string &time) {
    int hour = stoi(time.substr(8, 2));
    auto account = DataStore::queryAccountByUid(card.cid);
    if (card.condition && account->balance > price) {
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 && hour >= 19) {
            if (price <= 20 || passwd_is_correct(card)) {
                account->consume(price);
                DataStore::insertConsumption(window, new Consumption(card.cid, window, time, price));
                show(window, time);
            } else {

            }
        } else {

        }
    } else {

    }
}


void Consume::consume(const Consumption &log) {
    auto account = DataStore::queryAccountByUid(log.cid);
    account->consume(log.price);
}

void Consume::show(const Window &window) {

}

void Consume::show(const Window &window, const std::string &time) {

}


bool Consume::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}