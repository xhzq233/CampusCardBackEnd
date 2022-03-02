#include "DataAnalyze.h"

using namespace DataAnalyze;

std::vector<unsigned int> DataAnalyze::fuzzyQueryOnUid(std::string &str) {
    std::regex re = std::regex(str);
    std::vector<unsigned int> res;
    for (auto account: DataStore::getAccounts()) {
        if (std::regex_match(std::to_string(account->uid), re)) {
            res.emplace_back(account->uid);
        }
    }
    return res;
}


float DataAnalyze::accumulatedConsumption(unsigned int uid, Time begin, Time end) {
    auto account = DataStore::subscript2Account(DataStore::queryAccountByUid(uid));
    if (!account || begin > end) {
        return 0;
    }
    unsigned int r_index;
    unsigned int l_index;
    std::vector<unsigned int> allCid = account->cards.getAllCid();
    float total = 0;
    for (int i = 0; i < DataStore::WINDOW_QTY; ++i) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[i];
        if (!consumptions_in_window.count()) {
            continue;
        }
        r_index = consumptions_in_window.halfSearch(BaseOperation(end));
        l_index = consumptions_in_window.halfSearch(BaseOperation(begin));
        consumptions_in_window.for_loop(l_index, r_index, [&](auto index, auto value) {
            for (auto &&cid: allCid) {
                if (value->cid == cid) {
                    total += value->price;
                }
            }
        });
    }
    return total;
}

k_min_students_res DataAnalyze::analyze(unsigned int uid) {

    auto account = DataStore::subscript2Account(DataStore::queryAccountByUid(uid));
    if (!account) {
        return {};
    }
    std::vector<unsigned int> allCid = account->cards.getAllCid();
    std::unordered_map<unsigned int, unsigned char> student_count_map;
    for (int window = 0; window < DataStore::WINDOW_QTY; ++window) {
        auto &consumptions_in_window = *DataStore::getConsumptions()[window];
        if (!consumptions_in_window.count()) {
            continue;
        }
        consumptions_in_window.for_loop([&](auto index, auto value) {
            auto &map = DataStore::getAccountsMapByCid();
            for (auto &&cid: allCid) { //找出和指定uid相隔5个以内的uid
                if (value->cid == cid) {
                    for (int student = -5; student < 6; ++student) {
                        if (!student) {
                            continue;
                        }
                        int current =
                                (int) index + student < 0 ? consumptions_in_window.size + student : index + student;
                        auto &cur_cid = consumptions_in_window[current]->cid;
                        if (map.count(cur_cid) == 0)
                            continue;
                        auto acc = map[cur_cid];
                        if (student_count_map.count(acc->uid) == 0) {
                            student_count_map[acc->uid] = 1;
                        } else
                            ++student_count_map[acc->uid];
                    }
                }
            }
        });
    }
    std::priority_queue<priority_value> queue;

    k_min_students_res res;

    for (auto &&c: student_count_map)
        queue.push({c.first, c.second});

    for (auto &&re: res) {
        if (queue.empty()) break;
        auto value = queue.top();
        re = value;
        queue.pop();
    }
    return res;
}

std::pair<Window, unsigned int> DataAnalyze::mostFrequentWindowOfSomeone(unsigned int uid) {
    auto account = DataStore::subscript2Account(DataStore::queryAccountByUid(uid));
    if (!account) {
        return {0, 0};
    }
    auto cid = account->cards.begin().cid;
    std::unordered_map<Window, unsigned int> windows_count;
    auto &cons = DataStore::getConsumptions();
    for (auto &con: cons) {
        if (con->count() == 0)
            continue;
        con->for_loop([&](auto _, auto value) {
            if (value->cid == cid) {
                if (windows_count.count(value->window) == 0)
                    windows_count[value->window] = 1;
                else ++windows_count[value->window];
            }
        });
    }
    auto max_count = 0U;
    Window max_window = 1U;
    for (const auto &item: windows_count)
        if (item.second > max_count) {
            max_window = item.first;
            max_count = item.second;
        }
    return {max_window, max_count};
}
