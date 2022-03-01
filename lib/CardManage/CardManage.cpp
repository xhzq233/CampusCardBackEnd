//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

using namespace std;

static const constexpr char not_in_sys[] = "%d failed: NOT IN SYS";
static const constexpr char success[] = "%d %s succeeded";
static const constexpr char open_account[] = "开户";
static const constexpr char delete_account[] = "销户";
static const constexpr char set_lost[] = "挂失";
static const constexpr char un_set_lost[] = "解挂";
static const constexpr char recharge_account[] = "充值";
static const constexpr char reissue_account[] = "补卡";

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
unsigned int CardManage::openAccount(unsigned int uid, const string &name, const Time &time) {
    auto iter = DataStore::queryAccountByUid(uid);
    if (iter == DataStore::getAccounts().end()) {
        auto newAccount = new Account(uid, name, DataStore::getSerialNumber());
        DataStore::insertAccount(newAccount);
        return newAccount->cards.begin().cid;
        //log inside account constructor
    } else {
        log(time, open_account, not_in_sys, uid);
        return 0;
    }
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
bool CardManage::deleteAccount(unsigned int uid, const Time &time) {
    auto iter = DataStore::queryAccountByUid(uid);
    auto account = *iter;
    if (iter != DataStore::getAccounts().end()) {
        account->cards.clear();
        log(time, delete_account, success, uid, account->name);
        DataStore::getAccounts().erase(iter);
        DataStore::getAccountsMapByCid().erase(account->cards.begin().cid); //already nullptr
        return true;
    } else {
        log(time, delete_account, not_in_sys, uid);
        return false;
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
int CardManage::setLost(unsigned int uid, const Time &time) {
    auto iter = DataStore::queryAccountByUid(uid);
    auto account = *iter;
    if (iter == DataStore::getAccounts().end()) {
        log(time, set_lost, not_in_sys, uid);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (card.condition) {
            card.condition = false;
            log(time, set_lost, success, uid, account->name);
            return 1;
        } else {
            log(time, set_lost, "%d %s failed: Already lost", uid, account->name);
        }
        return -1;
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
int CardManage::unsetLost(unsigned int uid, const Time &time) {
    auto iter = DataStore::queryAccountByUid(uid);
    auto account = *iter;
    if (iter == DataStore::getAccounts().end()) {
        log(time, un_set_lost, not_in_sys, uid);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (!card.condition) {
            card.condition = true;
            log(time, un_set_lost, success, uid, account->name);
            return 1;
        } else {
            log(time, un_set_lost, "%d %s failed: not lost yet", uid, account->name);
            return -1;
        }
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
int CardManage::reissue(unsigned int uid, const Time &time) {
    auto iter = DataStore::queryAccountByUid(uid);
    auto account = *iter;
    if (iter == DataStore::getAccounts().end()) {
        log(time, reissue_account, not_in_sys, uid);
        return 0;
    }
        //最多只能补卡100次
    else if (account->cards.size() >= CardManage::MAX_REISSUE_TIMES) {
        // reference:
        // https://sites.google.com/site/wyylview/dong-tai-fen-pei-nei-cun-zai-ke-nengheap-corruption-detected-de-yuan-yin-zhi-yi-2
        log(time, reissue_account, "%d %s failed: Reached upper limit", uid, account->name);
        return -1;
    } else {
        Card card(uid, DataStore::getSerialNumber());
//        DataStore::getAccountsMapByCid().erase(account->cards.begin().cid); // erase old value from map
        //将之前卡的状态设置为禁用状态
        account->cards.push(card);
        DataStore::getAccountsMapByCid()[card.cid] = &(*account); // push into map
        log(time, reissue_account, success, uid, account->name);
        return 1;
    }
}

//为该学号账户充值；账户余额上限999.99元；
int CardManage::recharge(unsigned int uid, int amount, const Time &time) noexcept {
    auto iter = DataStore::queryAccountByUid(uid);
    auto account = *iter;
    if (iter == DataStore::getAccounts().end()) {
        log(time, recharge_account, not_in_sys, uid);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (account->balance + (float) amount > BALANCE_CEILING) {
            log(time, recharge_account, "%d %s failed: Reached upper limit", uid, account->name);
            return -1;
        } else {
            log(time, recharge_account, "%d %s success (cid: %d elder %.2f new %.2f)", uid,
                account->name, card.cid,
                account->balance,
                account->balance + (float) amount);
            account->recharge((float) amount);
            return (int) (account->balance);
        }
    }
}

//日志回溯
void CardManage::recall() {
}