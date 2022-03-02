//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H

#include "Card.h"
#include "../../FileManager/FileManager.h"

class Account {
public:
    static const constexpr char NAME_SIZE = 15;
    typedef char Name[NAME_SIZE];

    unsigned int uid;//学号

    FileManager::Time lastTimeEnterPasswd; //上一次输入密码的时间

    float totalConsumptionFromLastTime;//现在到上一次输入密码的消费总额

    Name name{};//姓名

    float balance;//余额

    CardList cards;

    Account(unsigned int uid, const std::string &name1, unsigned int serialNumber) : uid(uid), balance(0),
                                                                                     lastTimeEnterPasswd(0),
                                                                                     totalConsumptionFromLastTime(0),
                                                                                     cards(Card(uid, serialNumber)) {
        if (name1.size() > NAME_SIZE)
            throw;
        strncpy(name, name1.c_str(), name1.size());
        FileManager::getInstance()
                << FileManager::toStandardLogString("开户", (std::to_string(uid) + name).c_str(), 2021'09'01'24'00'00);
    }

    // from strings
    explicit Account(const std::vector<std::string> &strings, unsigned int serialNumber) : Account(
            std::stoul(strings[0]), strings[1], serialNumber) {}

    ~Account() {
        cards.clear();
    };

    // to string
    [[nodiscard]] std::string to_string() const {
        std::string res;
        res.append(std::to_string(uid));
        res.push_back(' ');
        res.append(name);
        res.push_back(' ');
        res.append(std::to_string(balance));
        res.push_back(' ');
        res.append(cards.to_string());
        return res;
    }

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
