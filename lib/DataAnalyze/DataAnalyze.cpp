#include "DataAnalyze.h"

using namespace DataAnalyze;

std::vector<unsigned int> DataAnalyze::fuzzyQueryOnUid(const std::regex &re) {
    std::vector<unsigned int> res;
    for (auto account: DataStore::getAccounts()) {
        if (std::regex_match(std::to_string(account->uid), re)) {
            res.emplace_back(account->uid);
        }
    }
    return res;
}

std::vector<unsigned int> fuzzyQueryOnName(const std::regex &re) {
    std::vector<unsigned int> res;
    for (auto account: DataStore::getAccounts()) {
        if (std::regex_match(account->name, re)) {
            res.emplace_back(account->uid);
        }
    }
    return res;
}


float DataAnalyze::accumulatedConsumption(unsigned int uid, Time begin, Time end) {
    auto account = *DataStore::queryAccountByUid(uid);
    if (!account) {
        return 0;
    }
    unsigned int cid = account->cards.begin().cid;
    float total = 0;
    for (int i = 0; i < DataStore::WINDOW_QTY; ++i) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[i];
        if (!consumptions_in_window.count()) {
            continue;
        }
        unsigned int r_index;
        unsigned int l_index;
        if (end == -1) {
            r_index = consumptions_in_window.current_index;
        } else if (begin > end) {
            throw;
        } else {
            r_index = consumptions_in_window.halfSearch([&](auto value) -> bool {
                return value->time < end;
            });
        }
        l_index = consumptions_in_window.halfSearch([&](auto value) -> bool {
            return value->time < begin;
        });
        consumptions_in_window.for_loop(l_index, r_index, [&](auto value) {
            if (value->cid == cid) {
                printf("%.2f\n", value->price);
                total += value->price;
            }
        });
    }
    return total;
}
