//
// Created by 夏侯臻 on 2022/2/21.
//

#ifndef CAMPUSCARDBACKEND_ACCOUNT_H
#define CAMPUSCARDBACKEND_ACCOUNT_H
#include <list>
#include "Card.h"

class Account
{
private:
    /* data */
public:
    unsigned int uid;
    std::string name;
    double balance;
    std::list<Card*>  v;
    Account(std::string name);
    ~Account();
};


#endif //CAMPUSCARDBACKEND_ACCOUNT_H
