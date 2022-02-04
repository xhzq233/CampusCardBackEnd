//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include <string>
#include <list>
#include <map>
#include "Card.h"
#include "FileManager.h"

#define BALANCE_CEILING 999.99 //账户余额上限

using namespace std;

/*
 * 可以满足10000人的校园卡管理，每位同学可以补卡100次；提供模糊匹配的姓名信息查询；形成操作日志；提供批量操作能力
 * */
class CardManage
{
private:
    static map<unsigned int, string> info;      //系统内的用户
    static map<unsigned int, list<Card *> *> v; //系统内的卡

public:
    int serialNumber;                                   //流水号
    CardManage();                                       //构造函数
    ~CardManage() = default;                            //析构函数
    CardManage(const CardManage &) = delete;            //拷贝构造函数
    CardManage &operator=(const CardManage &) = delete; //拷贝赋值函数
    CardManage(CardManage &&) = delete;                 //移动构造函数
    void openAccount(unsigned int, const string &);     //开户
    void deleteAccount(unsigned int);                   //销户
    void distribute(unsigned int);                      //发卡
    void setLost(unsigned int);                         //挂失
    void unsetLost(unsigned int);                       //解挂
    void reissue(unsigned int);                         //补卡
    void recharge(unsigned int, unsigned int);          //充值
    bool query(unsigned int);                           //账户查询
    void recall();                                      //日志回溯
};

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H