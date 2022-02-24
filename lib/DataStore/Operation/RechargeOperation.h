//
// Created by 夏侯臻 on 2022/2/24.
//
#include "CardManageOperation.h"

#ifndef CAMPUSCARDBACKEND_RECHARGEOPERATION_H
#define CAMPUSCARDBACKEND_RECHARGEOPERATION_H

class RechargeOperation : public CardManageOperation {
public:
    typedef float Price;
    Price price;

    RechargeOperation(Operator uid, Time time, Price price) : CardManageOperation(uid, Recharge, time),
                                                              price(price) {}

    [[nodiscard]] std::string to_string() const override {
        auto&& res = CardManageOperation::to_string();
        res.push_back(',');
        res.append(std::to_string(price));
        return res;
    }
};

#endif //CAMPUSCARDBACKEND_RECHARGEOPERATION_H
