//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"

#define BALANCE_CEILING 999.99 //账户余额上限

namespace CardManage {
    static unsigned int serialNumber = 12345;                                    //流水号

    void log(const char *, const std::string &, const std::string &); //日志记录

    std::string to_string(unsigned int uid, const std::string &name, const std::string &info);

    void openAccount(unsigned int uid, const std::string &name, const std::string &time = ""); //开户

    void deleteAccount(unsigned int uid, const std::string &time = "");             //销户

    void distribute(unsigned int uid, const std::string &time = "");                //发卡

    void setLost(unsigned int uid, const std::string &time = "");                   //挂失

    void unsetLost(unsigned int uid, const std::string &time = "");                 //解挂

    void reissue(unsigned int uid, const std::string &time = "");                   //补卡

    void recharge(unsigned int uid, float amount, const std::string &time = "");    //充值

    void recall();                                                                  //日志回溯

    void operateByFile();                                                           //批量操作

};

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H