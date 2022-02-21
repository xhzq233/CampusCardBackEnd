//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H

#include <list>
#include "Card.h"

class Account {
private:
    /* data */
public:
    unsigned int uid;//学号
    std::string name;//姓名
    double balance;//卡号
    std::list<Card *> cards;//卡
    Account(unsigned int uid, std::string &name);

    inline void consume(float price); //消费

    inline void recharge(float amount); //充值

    ~Account();

    //用于 priority_queue
    bool operator<(const Account &right) const {
        return uid < right.uid; //大顶堆
    }
};


#endif //CAMPUSCARDBACKEND_ACCOUNT_H
