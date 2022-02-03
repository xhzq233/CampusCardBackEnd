//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

CardManage::CardManage()
{
    serialNumber = 12345;
    info = {};
    v = {};
}

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const string &name)
{
    info.insert(make_pair(uid, std::move(name)));
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid)
{
    info.erase(uid);
    //将销户的所有卡号状态设置为禁用
    setLost(uid);
}

//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid)
{
    if (info.count(uid) != 0 && v.count(uid) == 0)
    {
        list<Card *> *l = new list<Card *>({new Card(uid, info[uid], ++serialNumber)});
        v.insert(make_pair(uid, l));
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid)
{
    Card *current = (*v[uid]->begin()); //最新的卡
    current->condition = false;
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid)
{
    Card *current = (*v[uid]->begin()); //最新的卡
    current->condition = true;
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid)
{
    //最多只能补卡100次
    if (v[uid]->size() >= 100)
    {
        return;
    }
    Card *card = new Card(uid, info[uid], ++serialNumber);
    //如果之前有卡,将之前卡的余额给新补的卡
    if (!v[uid]->empty())
    {
        card->balance = (*v[uid]->begin())->balance;
    }
    //将之前卡的状态设置为禁用状态
    setLost(uid);
    v[uid]->push_front(card);
}

//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, unsigned int amount)
{
    Card *current = (*v[uid]->begin()); //最新的卡
    if (!current->condition)            //当前卡为禁用状态
    {
        ;
    }
    else if (current->balance + amount > BALANCECEILING)
    {
        ;
    }
    else
    {
        current->balance += amount;
    }
}

//查询和学号匹配的卡
bool CardManage::query(unsigned int uid)
{
}

//日志回溯
void CardManage::recall()
{
}