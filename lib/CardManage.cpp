//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const string &name)
{
    fstream f();
}
//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid)
{
    info.erase(uid);
    auto range = v.equal_range(uid);
    for (auto iter = range.first; iter != range.second; iter++) //将销户的所有卡号状态设置为禁用
    {
        if (iter->first == uid)
        {
            iter->second->condition = false;
        }
    }
    log();
}
//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid)
{
    if (info.count(uid) != 0 && v.count(uid) == 0)
    {
        Card *card = new Card(uid, info[uid], ++serialNumber);
        v.insert(make_pair(uid, card));
    }
    log();
}
//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid)
{
    auto range = v.equal_range(uid);
    (--range.second)->second->condition = false;
}
//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid)
{
    auto range = v.equal_range(uid);
    (--range.second)->second->condition = true;
}
//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid)
{
    auto range = v.equal_range(uid);
    for (auto i = range.first; i != range.second; i++)
    {
        if (i->first == uid)
        {
            i->second->condition = false; //将该学号关联的其他卡片设置为挂失禁用状态；
        }
    }
    Card *card = new Card(uid, info[uid], ++serialNumber);
    v.insert(make_pair(uid, card));
}
//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, unsigned int amount)
{
    auto range = v.equal_range(uid);
    Card *card = (--range.second)->second;
    if (card->balance + amount > balanceCeiling)
    {
        ;
    }
    else
    {
        card->balance += amount;
    }
    log();
}
//查询和学号匹配的卡
bool CardManage::query(unsigned int uid)
{
}
//日志记录
void CardManage::log()
{
}
//日志回溯
void CardManage::recall()
{
}