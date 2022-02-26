//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

using namespace std;

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const string &name, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        DataStore::insertAccount(Account(uid, name));
        //log inside account constructor
    } else {
        not_in_sys("开户", uid, time);
    }
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account != DataStore::getAccounts().end()) {
        account->cards.clear();
        success("销户", account->name.c_str(), uid, time);
        DataStore::getAccounts().erase(account);
    } else {
        not_in_sys("销户", uid, time);
    }
}

//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account != DataStore::getAccounts().end()) {
        Card card(uid, ++CardManage::serialNumber);
        account->cards.push_front(card);
        success("发卡", account->name.c_str(), uid, time);
    } else {
        not_in_sys("发卡", uid, time);
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("挂失", uid, time);
    } else {
        auto card = account->cards.begin();
        if (card->condition) {
            card->condition = false;
            success("挂失", account->name.c_str(), uid, time);
        } else {
            auto buffer = new char[40];
            sprintf(buffer, "%d %s failed: Already lost", uid, account->name.c_str());
            log("挂失", buffer, time);
        }
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("解挂", uid, time);
        return;
    }
    auto card = account->cards.begin();
    if (!card->condition) {
        card->condition = true;
        success("解挂", account->name.c_str(), uid, time);
    } else {
        auto buffer = new char[40];
        sprintf(buffer, "%d %s failed: not lost yet", uid, account->name.c_str());
        log("解挂", buffer, time);
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("补卡", uid, time);
    }
        //最多只能补卡100次
    else if (account->cards.size() >= CardManage::MAX_REISSUE_TIMES) {
        auto buffer = new char[40];
        sprintf(buffer, "%d %s failed: Reached upper limit", uid, account->name.c_str());
        log("补卡", buffer, time);
    } else {
        Card card(uid, ++CardManage::serialNumber);
        //如果之前有卡,将之前卡的余额给新补的卡
        //将之前卡的状态设置为禁用状态
        setLost(uid);
        account->cards.push_front(card);
        success("补卡", account->name.c_str(), uid, time);
    }
}

//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, int amount, const Time &time) noexcept {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("充值", uid, time);
    } else {
        auto card = account->cards.begin();
        if (account->balance + (float) amount > BALANCE_CEILING) {
            auto buffer = new char[35];
            sprintf(buffer, "%d failed: Reached upper limit", uid);
            log("充值",
                buffer,
                time);
        } else {
            auto buffer = new char[80];
            sprintf(buffer, "%d %s success (cid: %d elder %.2f new %.2f)", uid, account->name.c_str(), card->cid,
                    account->balance,
                    account->balance + (float) amount);
            log("充值", buffer, time);
            account->recharge((float) amount);
        }
    }
}

//日志回溯
void CardManage::recall() {
}

void CardManage::log(const char *title, const char *content, const Time &time) {
    if (!time) {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content);
    } else {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content, time);
    }
}