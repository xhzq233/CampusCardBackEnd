//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include <string>
#include <list>
#include <map>
#include "Card.h"
#include "DataStore.h"
#include "FileManager.h"

#define BALANCECEILING 999.99 //账户余额上限

class CardManage
{
private:

    CardManage();                                                  //构造函数
    ~CardManage() = default;                                       //析构函数
    static void log(const char *, const std::string &, const std::string &); //日志记录

public:
    static unsigned int serialNumber;                                    //流水号
    CardManage(const CardManage &) = delete;                             //拷贝构造函数
    CardManage &operator=(const CardManage &) = delete;                  //拷贝赋值函数
    CardManage(CardManage &&) = delete;                                  //移动构造函数

    static void openAccount(unsigned int uid, const std::string &name, const std::string &time = ""); //开户
    static void deleteAccount(unsigned int uid, const std::string &time = "");             //销户
    static void distribute(unsigned int uid, const std::string &time = "");                //发卡
    static void setLost(unsigned int uid, const std::string &time = "");                   //挂失
    static void unsetLost(unsigned int uid, const std::string &time = "");                 //解挂
    static void reissue(unsigned int uid, const std::string &time = "");                   //补卡
    static void recharge(unsigned int uid, float amount, const std::string &time = "");    //充值
    static std::vector<Account>::iterator queryById(unsigned  int uid);                               //用id查询
    static void queryByName(const std::string &);                             //用姓名查询
    static void recall();                                                //日志回溯
    static void openAccountByFile();                                     //批量开户
    static void operateByFile();                                         //批量操作
};

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H