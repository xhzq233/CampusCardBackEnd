//
// Created by 夏侯臻 on 2022/3/1.
//

#ifndef CAMPUSCARDBACKEND_MAIN_H
#define CAMPUSCARDBACKEND_MAIN_H

#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"
#include "lib/CardManage/CardManage.h"
#include "lib/Consume/Consume.h"
#include "lib/Utils/MergeSort.h"
#include "lib/DataAnalyze/DataAnalyze.h"

namespace Main {
    typedef std::function<void(void)> VoidCallBack;

    void testTimeWrapper(const VoidCallBack &func) {

        auto t1 = std::chrono::high_resolution_clock::now();

        func();

        auto t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> ms = t2 - t1;
        printf("use time: %f ms.\n", ms.count());
    }

    using CSV = FileManager::CSV;

    constexpr static const unsigned int RESERVED_SIZE = 2'200'000;
    constexpr static const unsigned int MERGE_NUM = 64;
    typedef BaseOperation *SortedOperations[RESERVED_SIZE];

    void init() {

        // MARK:--- init operations
        auto *operations = new SortedOperations{nullptr};
        auto &res = DataStore::getConsumptions();
        unsigned int num = 0;

        int per_indexes[MERGE_NUM + 1]; // 记录每部分排好序的数组的下标
        int per_indexes_index = 0;
        for (auto re: res) {
            if (re->count() == 0)
                continue;
            per_indexes[per_indexes_index++] = (int) num;
            re->for_loop([&](auto value) {
                operations[num++] = value;
            });
        }
        per_indexes[per_indexes_index++] = (int) num;
        CSV temp;

        FileManager::getInstance().getCSVDataSource(temp, 3, FileManager::CARD_MANAGE_CSV_NAME);
        for (const auto &item: temp)
            operations[num++] = new CardManageOperation(item);
        per_indexes[per_indexes_index++] = (int) num;
        temp.clear();

        FileManager::getInstance().getCSVDataSource(temp, 4, FileManager::CARD_RECHARGE_CSV_NAME);
        for (const auto &item: temp)
            operations[num++] = new RechargeOperation(item);
        per_indexes[per_indexes_index++] = (int) num;
        for (int i = per_indexes_index; i < MERGE_NUM + 1; ++i) {
            per_indexes[i] = (int) num;
        }
        temp.clear();

        MergeSort<BaseOperation *>::sort(operations, per_indexes, MERGE_NUM);

        // separate complete
        RechargeOperation *rechargeOperation;
        Consumption *consumption;
        CardManageOperation *cardManageOperation;

        // start_index operation
        using namespace Consume;
        for (int i = 0; i < num; ++i) {
            if ((rechargeOperation = dynamic_cast<RechargeOperation *>(operations[i]))) {
                CardManage::recharge(rechargeOperation->uid, rechargeOperation->price, rechargeOperation->time);
            } else if ((consumption = dynamic_cast<Consumption *>(operations[i]))) {
                consume(*consumption);
            } else if ((cardManageOperation = dynamic_cast<CardManageOperation *>(operations[i]))) {
                switch (cardManageOperation->operationName) {
                    case CardManageOperation::Reissue:
                        CardManage::reissue(cardManageOperation->uid, cardManageOperation->time);
                        break;
                    case CardManageOperation::Uncouple:
                        CardManage::unsetLost(cardManageOperation->uid, cardManageOperation->time);
                        break;
                    case CardManageOperation::SetLoss:
                        CardManage::setLost(cardManageOperation->uid, cardManageOperation->time);
                        break;
                    case CardManageOperation::Cancellation:
                        CardManage::deleteAccount(cardManageOperation->uid, cardManageOperation->time);
                        break;
                    case CardManageOperation::Recharge:
                        throw;// no recharge_account
                }
            } else {
                throw; // "Unknown Operation"
            }
        }
        // ---
        delete[] operations;
    }
}



#endif //CAMPUSCARDBACKEND_MAIN_H
