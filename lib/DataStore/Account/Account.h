//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H

#include "Card/Card.h"
#include "../../FileManager/FileManager.h"

class list {
private:
    Card card;
    list *next;

public:

    list(unsigned int uid, unsigned int serialNumber) : card(uid, serialNumber), next(nullptr) {};

    ~list()=default;

    void push(Card &newCard);

    Card &begin();

    void clear();

    int size();
};


class Account {
private:
    /* data */
public:
    unsigned int uid;//学号
    static const constexpr char NAME_SIZE = 15;
    typedef char Name[NAME_SIZE];
    unsigned int lastTime; //上一次输入密码的时间
    float lastTotal;//现在到上一次输入密码的消费总额
    Name name{0};//姓名
    float balance;//余额
    list cards;//卡

    Account(unsigned int uid, const std::string &name1, unsigned int serialNumber) : uid(uid), balance(0), lastTime(0),
                                                                                     lastTotal(0),
                                                                                     cards(uid, serialNumber) {
        if (name1.size() > NAME_SIZE)
            throw;
        strncpy(name, name1.c_str(), name1.size());
        FileManager::getInstance() << FileManager::toStandardLogString("开户", this->to_string());
    }

    // from strings
    explicit Account(const std::vector<std::string> &strings, unsigned int serialNumber) : Account(
            std::stoul(strings[0]), strings[1], serialNumber) {}

    ~Account() = default;

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
