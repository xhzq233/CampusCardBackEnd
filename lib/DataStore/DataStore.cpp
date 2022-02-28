//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"
#include "../Utils/ThreadPool.h"

using Accounts = DataStore::Accounts;
using Consumptions = DataStore::Consumptions;
using WindowPositions = DataStore::WindowPositions;
using CSV = FileManager::CSV;

Accounts &DataStore::accounts_init() {
    static Accounts res;
    CSV temp;
    FileManager::getInstance().getCSVDataSource(temp, 2, FileManager::OPEN_ACCOUNT_CSV_NAME);
    for (auto &&info: temp) {
        res.emplace_back(Account(info, getSerialNumber()));
    }
    std::sort(res.begin(), res.end());
    return res;
}

Consumptions &DataStore::consumes_init() {
    //99 x 60000
    static Consumptions res;
    auto &windowPositions = DataStore::getWindowPositions();
    for (int i = 0; i < WINDOW_QTY; ++i) {
        res[i] = new CircularArray<Consumption *>(MAXSIZE, windowPositions[i]);
    }

    std::function<void(void)> tasks[FileManager::CONSUME_CSV_QTY];
    for (int i = 0; i < FileManager::CONSUME_CSV_QTY; ++i) {
        tasks[i] = [window_index = i, consumes = res]() {
            printf("work %d executing!\n", window_index);
            CSV temp;
            FileManager().getCSVDataSource(temp, 4, FileManager::CONSUME_CSV(window_index + 1));
            unsigned int size = temp.size();
            for (unsigned int i = 0; i < size; ++i) {
                consumes[window_index]->push_back(new Consumption(window_index + 1, temp[i]));
            }
            // no longer to be sorted
        };
    }
    JoinableMultiWork works(Device::MAX_THREAD - 1, FileManager::CONSUME_CSV_QTY, tasks);
    printf("All threads joined!\n");
    return res;
}

const WindowPositions &DataStore::windows_init() {
    static WindowPositions windowPositions{0};
    FileManager::CSV container;
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, WINDOW_QTY, 2,
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标

    //why auto&&?
    //Reference:
    //https://stackoverflow.com/questions/13241108/why-does-a-range-based-for-statement-take-the-range-by-auto
    for (const auto &row: container)
        windowPositions[stoi(row[0]) - 1] = stoi(row[1]); // window ranged 1-99, but subscripts ranged 0-98

    return windowPositions;
}

void DataStore::localize() {

}

const WindowPositions &DataStore::getWindowPositions() {
    static const WindowPositions &windowPositions = windows_init();
    return windowPositions;
}

Consumptions &DataStore::getConsumptions() {
    static Consumptions &consumes = consumes_init();
    return consumes;
}

Accounts &DataStore::getAccounts() {
    static Accounts &accounts = accounts_init();
    return accounts;
}

void DataStore::insertAccount(const Account &data) {
    auto &accounts = getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    //half search
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid] > data) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    accounts.emplace(accounts.begin() + left, data);
}

std::vector<Account>::iterator DataStore::queryAccountByUid(unsigned int uid) {
    auto &accounts = getAccounts();
    //half search
    int left = 0, right = (int) accounts.size() - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].uid == uid) {
            return accounts.begin() + mid;
        } else if (accounts[mid].uid > uid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    // result is not found
    return accounts.end();
}

std::vector<Account>::iterator DataStore::queryAccountByCid(unsigned int cid) {
    auto &accounts = getAccounts();
    //half search
    for (auto i = accounts.begin(); i != accounts.end(); ++i) {
        if (i->cards.begin().cid == cid)
            return i;
    }
    return accounts.end();
}

void DataStore::pushConsumption(Window window, Consumption *data) {
    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this for_loop: 1 - 99", window);
        return;
    }
    auto consumes_in_window = getConsumptions()[window - 1];
    consumes_in_window->push_back(data);
}

void DataStore::insertConsumptionOnSpecifiedTime(Window window, Consumption *data) {
    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this for_loop: 1 - 99", window);
        return;
    }
    auto &consumes_in_window = getConsumptions()[window - 1];
    consumes_in_window->insert(data);
}

using DataQuery = DataStore;

std::regex DataQuery::customRegex2CommonRegexSyntax(std::string &regex) {
    regex.replace(regex.find('?'), 1, ".");
    regex.replace(regex.find('*'), 1, ".{2,}");
    return std::regex(regex);
}

DataQuery::Subscripts
DataQuery::query(FileManager::Strings &container, const std::regex &regex) {
    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i], regex))
            res.emplace_back(i);
    }
    return res;
}

DataQuery::Subscripts
DataQuery::query(FileManager::CSV &container, unsigned int columnIndex, const std::regex &regex) {

    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i][columnIndex], regex))
            res.emplace_back(i);
    }
    return res;
}

DataQuery::QueryResults DataStore::queryConsumption(Window window, unsigned int cid) {
    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this for_loop: 1 - 99", window);
        return {};
    }
    QueryResults res;
    const auto &consumptions_in_window = *getConsumptions()[window - 1];
    consumptions_in_window.for_loop([&](auto value) {
        if (cid == value->cid)
            res.emplace_back(value);
    });
    return res;
}

DataQuery::QueryResults DataStore::queryConsumption(unsigned int cid) {
    QueryResults res;
    auto &consumptions = getConsumptions();
    for (auto &consumption: consumptions) {
        consumption->for_loop([&](auto value) {
            if (cid == value->cid)
                res.emplace_back(value);
        });
    }
    return res;
}

DataStore::QueryResults
DataStore::queryConsumptionInTimeRange(Window window, DataStore::Time left, DataStore::Time right) {

    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this for_loop: 1 - 99", window);
        return {};
    }
    QueryResults res;
    auto &consumptions_in_window = *getConsumptions()[window - 1];
    unsigned int r_index;
    unsigned int l_index;
    if (right == -1) {
        r_index = consumptions_in_window.current_index;
    } else if (left > right) {
        throw;
    } else {
        r_index = consumptions_in_window.halfSearch([&](auto value) -> bool {
            return value->time < right;
        });
    }
    l_index = consumptions_in_window.halfSearch([&](auto value) -> bool {
        return value->time < left;
    });

    consumptions_in_window.for_loop(l_index, r_index, [&](auto value) {
        res.template emplace_back(value);
    });

    return res;
}
