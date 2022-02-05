//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include <string>
#include <List>
#include <map>
#include "Card.h"
#include "FileManager.h"

#define BALANCECEILING 999.99 //账户余额上限

using namespace std;

class CardManage
{
private:
    static map<unsigned int, string> info;              //系统内的用户
    static map<unsigned int, list<Card *> *> v;         //系统内的卡
    static CardManage *instance;                        //唯一实例
    CardManage();                                       //构造函数
    ~CardManage() = default;                            //析构函数
    CardManage(const CardManage &) = delete;            //拷贝构造函数
    CardManage &operator=(const CardManage &) = delete; //拷贝赋值函数
    CardManage(CardManage &&) = delete;                 //移动构造函数

public:
    static unsigned int serialNumber;                    //流水号
    static CardManage *getInstance();                    //获取实例
    static void openAccount(unsigned int, const char *); //开户
    static void deleteAccount(unsigned int);             //销户
    static void distribute(unsigned int);                //发卡
    static void setLost(unsigned int);                   //挂失
    static void unsetLost(unsigned int);                 //解挂
    static void reissue(unsigned int);                   //补卡
    static void recharge(unsigned int, unsigned int);    //充值
    bool query(unsigned int);                            //账户查询
    static void recall();                                //日志回溯
    static void log(const char *, const string &);       //日志记录
};

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H