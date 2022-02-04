//
// Created by 夏侯臻 on 2022/1/14.
//
#include <string>

#ifndef CAMPUSCARDBACKEND_CARD_H
#define CAMPUSCARDBACKEND_CARD_H

#define DEFAULT_DATE 20240715 //默认的卡号有效期
#define BALANCE_CEILING 999.99 //账户余额上限
using namespace std;

class Card {
private:
    unsigned int cid; //卡号
    string name;      //姓名
    int date;         //有效日期

public:
    unsigned int uid; //学号
    int balance;      //账户余额
    bool condition;   //卡的状态
    unsigned int password      //密码
    Card(unsigned int, const string &, unsigned int, unsigned int); // card的构造函数
};


#endif //CAMPUSCARDBACKEND_CARD_H
