//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"

using Accounts = DataStore::Accounts;
using Consumes = DataStore::Consumes;
using WindowPositions = DataStore::WindowPositions;
using CSV = FileManager::CSV;

Accounts &DataStore::accounts_init() {
    static Accounts res;
    CSV temp;
    FileManager::getInstance().getCSVDataSource(temp, 2, FileManager::OPEN_ACCOUNT_CSV_NAME);
    for (auto &&j: temp) {
        res.emplace_back(Account(j));
    }

    std::sort(res.begin(), res.end());
    return res;
}

Consumes &DataStore::consumes_init() {

    static Consumes res(FileManager::CONSUME_CSV_QTY, std::vector<Consume>());

    CSV temp;
    for (unsigned int i = 0; i < FileManager::CONSUME_CSV_QTY; ++i) {
        FileManager::getInstance().getCSVDataSource(temp, 4, FileManager::CONSUME_CSV(i + 1));
        res[i].reserve(temp.size());
        for (auto &&j: temp) {
            res[i].emplace_back(Consume(i, j));
        }
        std::sort(res[i].begin(), res[i].end());
        temp.clear();
    }
    return res;
}

const WindowPositions &DataStore::windows_init() {
    static std::vector<WindowPosition> res(WINDOW_QTY);

    FileManager::CSV container;
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, Pair((unsigned int) WINDOW_QTY, (unsigned int) 2),
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标

    //why auto&&?
    //Reference:
    //https://stackoverflow.com/questions/13241108/why-does-a-range-based-for-statement-take-the-range-by-auto
    for (auto &&row: container)
        res[stoi(row[0])] = stoi(row[1]);

    return res;
}

void DataStore::localize() {

}

const WindowPositions &DataStore::getWindowPositions() {
    static const WindowPositions windowPositions = windows_init();
    return windowPositions;
}

Consumes &DataStore::getConsumes() {
    static Consumes consumes = consumes_init();
    return consumes;
}

Accounts &DataStore::getAccounts() {
    static Accounts accounts = accounts_init();
    return accounts;
}

void DataStore::insertAccount(const Account &data) {
    auto accounts = getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    //half search
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].uid > data.uid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    accounts.emplace(accounts.begin() + mid, data);
}
