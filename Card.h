//
// Created by 夏侯臻 on 2022/1/14.
//
#include <string>
#ifndef CAMPUSCARDBACKEND_CARD_H
#define CAMPUSCARDBACKEND_CARD_H
#define defaultDate 20240715 //默认的卡号有效期
#define balanceCeiling 999.99 //账户余额上限
using namespace std;
class Card
{
private:
    int cid;     //卡号
    int uid;     //学号
    string name; //姓名
    int date;    //有效日期
    int balance; //账户余额
public:
    Card(int, string, int); // card的构造函数
    void recharge(int);     //充值
};


#endif //CAMPUSCARDBACKEND_CARD_H
