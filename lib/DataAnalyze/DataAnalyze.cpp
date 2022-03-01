#include "DataAnalyze.h"

using namespace DataAnalyze;

std::vector<unsigned int> DataAnalyze::fuzzyQuery(const std::string &str) {
    std::vector<unsigned int> res;
    for (auto account:DataStore::getAccounts()) {
        if (std::regex_match(std::to_string(account.uid),std::regex(str)))
        {
            res.push_back(account.uid);
        }
    }
    return res;
}

float DataAnalyze::accumulatedConsumption(unsigned int uid, Time begin, Time end) {
    unsigned int cid = DataStore::queryAccountByUid(uid)->cards.begin().cid;
    float total = 0;
    for (int i = 0; i < 99; ++i) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[i];
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
                total += value->price;
            }
        });
    }
    return total;
}