//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H

#include "Card/Card.h"
#include "../../FileManager/FileManager.h"

class list {
private:
    Card *card;
    list *next;

public:
    list() : card(nullptr), next(nullptr) {};

    ~list();

    void push(Card *newCard);

    Card *begin();

    void clear();

    int size();
};


class Account {
private:
    /* data */
public:
    unsigned int uid;//学号
    std::string name;//姓名
    float balance;//余额
    list cards;//卡
    Account(unsigned int uid, std::string name) : uid(uid), name(move(name)), balance(0), cards() {
        FileManager::getInstance() << FileManager::toStandardLogString("开户", this->to_string());
    }

    // from strings
    explicit Account(const std::vector<std::string> &strings) : Account(std::stoul(strings[0]), strings[1]) {}

    ~Account()=default;

    // to string
    [[nodiscard]] std::string to_string() const;

    void consume(float price) {
        this->balance -= price;
    } //消费

    void recharge(float amount) {
        this->balance += amount;
    } //充值

    /// comparable
    bool operator>(const Account &right) const {
        return uid > right.uid;
    }

    bool operator<(const Account &right) const {
        return uid < right.uid;
    }
};


#endif //CAMPUSCARDBACKEND_ACCOUNT_H
