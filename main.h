//
// Created by 夏侯臻 on 2022/3/1.
//

#ifndef CAMPUSCARDBACKEND_MAIN_H
#define CAMPUSCARDBACKEND_MAIN_H

#include "lib/FileManager/FileManager.h"
#include "lib/DataStore/DataStore.h"
#include "lib/CardManage/CardManage.h"
#include "lib/CardManage/Consume/Consume.h"
#include "lib/Utils/MergeSort.h"
#include "lib/DataAnalyze/DataAnalyze.h"

namespace Main {
    using namespace Consume;
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
    static bool is_initialized = false;

    void init() {

        if (is_initialized) {
            printf("already initialized...");
            return;
        }

        // MARK:--- init operations
        auto *operations = new SortedOperations{nullptr};
        auto &consumptions = DataStore::getConsumptions();
        unsigned int num = 0;

        int per_indexes[MERGE_NUM + 1]; // 记录每部分排好序的数组的下标
        int per_indexes_index = 0;

        for (int window_index = 0; window_index < FileManager::CONSUME_CSV_QTY; ++window_index) {
            CSV temp;
            FileManager().getCSVDataSource(temp, 4, FileManager::CONSUME_CSV(window_index + 1));
            auto size = temp.size();
            per_indexes[per_indexes_index++] = (int) num;
            for (unsigned int i = 0; i < size; ++i)
                operations[num++] = new Consumption(window_index + 1, temp[i]);
            // no longer to be sorted
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

        MergeSort<BaseOperation *>::priority_sort(operations, per_indexes, 59);

        // sort complete
        RechargeOperation *rechargeOperation;
        Consumption *consumption;
        CardManageOperation *cardManageOperation;

        // start_index operation
        for (int i = 0; i < num; ++i) {
            if ((rechargeOperation = dynamic_cast<RechargeOperation *>(operations[i]))) {
                CardManage::recharge(rechargeOperation->uid, rechargeOperation->price, rechargeOperation->time);
            } else if ((consumption = dynamic_cast<Consumption *>(operations[i]))) {
                consume(consumptions[consumption->window - 1], consumption);
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
                        CardManage::account_cancellation(cardManageOperation->uid, cardManageOperation->time);
                        break;
                    case CardManageOperation::Recharge:
                        throw;// no recharge_account
                }
            } else {
                throw; // "Unknown Operation"
            }
            delete operations[i];
        }
        // ---

        delete[] operations;
        is_initialized = true;
    }
}


#endif //CAMPUSCARDBACKEND_MAIN_H
