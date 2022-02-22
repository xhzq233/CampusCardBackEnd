//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H

#include "Card/Card.h"
#include "../../FileManager/FileManager.h"

class Account {
private:
    /* data */
public:
    unsigned int uid;//学号
    std::string name;//姓名
    double balance;//卡号
    std::list<Card> cards;//卡
    Account(unsigned int uid, std::string name);

    // from strings
    explicit Account(const std::vector<std::string>& strings): Account(std::stoi(strings[0]),strings[1]){}

    void consume(float price); //消费

    void recharge(float amount); //充值

    ~Account();

    /// comparable
    bool operator>(const Account &right) const {
        return uid > right.uid;
    }

    bool operator<(const Account &right) const {
        return uid < right.uid;
    }
};


#endif //CAMPUSCARDBACKEND_ACCOUNT_H
