//
// Created by 夏侯臻 on 2022/1/14.
//


#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Card.h"
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H
#define balanceCeiling 999.99 //账户余额上限
using namespace std;

class CardManage
{
private:
    static map<unsigned int, string> info;   //系统内的用户
    static multimap<unsigned int, Card *> v; //系统内的卡

public:
    int serialNumber;                               //流水号
    void openAccount(unsigned int, const string &); //开户
    void deleteAccount(unsigned int);               //销户
    void distribute(unsigned int);                  //发卡
    void setLost(unsigned int);                     //挂失
    void unsetLost(unsigned int);                   //解挂
    void reissue(unsigned int);                     //补卡
    void recharge(unsigned int, unsigned int);      //充值
    bool query(unsigned int);                       //账户查询
    void recall();                                  //日志回溯
    void log();                                     //日志记录
};
#endif // CAMPUSCARDBACKEND_CARDMANAGE_H

