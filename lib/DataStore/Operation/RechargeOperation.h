#include "CardManageOperation.h"

#ifndef CAMPUSCARDBACKEND_RECHARGEOPERATION_H
#define CAMPUSCARDBACKEND_RECHARGEOPERATION_H

class RechargeOperation : public CardManageOperation {
public:
    typedef int RechargeCount;// can only recharge integers by default
    RechargeCount price;

    RechargeOperation(Operator uid, Time time, RechargeCount price) : CardManageOperation(uid, Recharge, time),
                                                                      price(price) {}

    // from strings
    explicit RechargeOperation(const std::vector<std::string> &strings) :
            RechargeOperation(
                    std::stoi(strings[2]),
                    std::stoull(strings[0]),
                    std::stoi(strings[3])
            ) {}

    [[nodiscard]] std::string to_string() const override {
        auto &&res = CardManageOperation::to_string();
        res.push_back(',');
        res.append(std::to_string(price));
        return res;
    }
};

#endif //CAMPUSCARDBACKEND_RECHARGEOPERATION_H
