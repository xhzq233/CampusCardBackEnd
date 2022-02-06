//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CARD_H
#define CARD_H

#include <string>

#define DEFAULT_DATE 20240715 //默认的卡号有效期
#define UID_LENGTH 10   //学号长度
using namespace std;

class Card
{
private:
    string name;       //姓名
    unsigned int date; //有效日期

public:
    unsigned int uid;     //学号
    unsigned int cid;     //卡号
    unsigned int balance; //账户余额
    bool condition;       //卡的状态

    Card(unsigned int, const string &, unsigned int); // card的构造函数
};

#endif // CARD_H