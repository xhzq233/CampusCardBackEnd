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
    unsigned int r_index;
    unsigned int l_index;
    std::vector<unsigned int> res = account->cards.getAllCid();
    float total = 0;
    for (int i = 0; i < DataStore::WINDOW_QTY; ++i) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[i];
        if (!consumptions_in_window.count()) {
            continue;
        }
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
        consumptions_in_window.for_loop(l_index, r_index, [&](auto index, auto value) {
            for (auto &&cid: res) {
                if (value->cid == cid) {
                    printf("%.2f\n", value->price);
                    total += value->price;
                }
            }
        });
    }
    return total;
}

std::vector<unsigned int> DataAnalyze::analyze(unsigned int uid) {
    auto account = *DataStore::queryAccountByUid(uid);
    if (!account) {
        return {};
    }
    std::vector<unsigned int> allCid = account->cards.getAllCid();
    std::map<unsigned int, unsigned int> count;
    for (int i = 0; i < DataStore::WINDOW_QTY; ++i) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[i];
        if (!consumptions_in_window.count()) {
            continue;
        }
        consumptions_in_window.for_loop([&](auto index, auto value) {
            for (auto &&cid: allCid) { //找出和指定uid相隔5个以内的uid
                if (value->cid == cid) {
                    for (int j = -5; j < 6; ++j) {
                        if (!j) {
                            continue;
                        }
                        auto a = DataStore::getAccountsMapByCid()[consumptions_in_window[(index + j) %
                                                                                         consumptions_in_window.size]->cid];
                        if (count.find(a->uid) != count.end()) {
                            count[a->uid] = 1;
                        } else {
                            ++count[a->uid];
                        }
                    }
                }
            }
        });
    }
    int size = count.size() < 5 ? (int) (count.size()) : 5;
    std::vector<unsigned int> res(size);
    int index = 0;
    for (auto &&c: count) {
        res[index] = c.second;
        if (++index == size) {
            break;
        }
    }
    return res;
}