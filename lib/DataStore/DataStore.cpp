//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"
#include <thread>

using Accounts = DataStore::Accounts;
using Consumes = DataStore::Consumes;
using WindowPositions = DataStore::WindowPositions;
using CSV = FileManager::CSV;

Accounts &DataStore::accounts_init() {
    static Accounts res;
    CSV temp;
    FileManager::getInstance().getCSVDataSource(temp, 2, FileManager::OPEN_ACCOUNT_CSV_NAME);
    for (auto &&info: temp) {
        res.emplace_back(Account(info));
    }
    std::sort(res.begin(), res.end());
    return res;
}

//  sentence under here used on debug
//  std::thread::id main_thread_id = std::this_thread::get_id();
void subwork_of_init_consumes(int index, Consumes *consumes) {
//    if (main_thread_id == std::this_thread::get_id())
//        std::cout << "This is the main thread.\n";
//    else
//        std::cout << "This is not the main thread.\n";

    CSV temp;
    FileManager().getCSVDataSource(temp, 4, FileManager::CONSUME_CSV(index + 1));

    for (int i = 0; i < temp.size(); ++i) {
        auto *consume = new Consumption(index + 1, temp[i]);
        consumes->at(index)[i] = consume;
    }

    // sort by from lower to bigger
    // only sort where have values
    std::sort(consumes->at(index).begin(), consumes->at(index).begin() + (int) temp.size(),
              [&](const Consumption *l, const Consumption *r) -> bool {
                  return l->date < r->date;
              });
}


Consumes &DataStore::consumes_init() {
    //99 x 60000
    static Consumes res(WINDOW_QTY, std::vector<Consumption *>(MAXSIZE, nullptr));
    std::thread threads[FileManager::CONSUME_CSV_QTY];

    std::cout << "Spawning threads...\n";
    for (int i = 0; i < FileManager::CONSUME_CSV_QTY; ++i)
        threads[i] = std::thread(subwork_of_init_consumes, i, &res);   // move-assign threads
    std::cout << "Done spawning threads. Now waiting for them to join:\n";
    for (auto &thread: threads)
        thread.join();
    std::cout << "All threads joined!\n";

    return res;
}

const WindowPositions &DataStore::windows_init() {
    static std::vector<WindowPosition> res(WINDOW_QTY, 0);

    FileManager::CSV container;
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, WINDOW_QTY, 2,
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
    static const WindowPositions &windowPositions = windows_init();
    return windowPositions;
}

Consumes &DataStore::getConsumes() {
    static Consumes &consumes = consumes_init();
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
        if (accounts[mid] < data) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    accounts.emplace(accounts.begin() + mid, data);
}

std::vector<Account>::iterator DataStore::queryByUid(unsigned int uid) {
    auto &accounts = getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].uid > uid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if ((accounts.begin() + mid)->uid == uid) {
        return accounts.begin() + mid;
    } else {
        return accounts.end();
    }
}

std::vector<Account>::iterator DataStore::queryByCid(unsigned int cid) {
    auto &accounts = DataStore::getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].cards.begin()->cid > cid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if ((accounts.begin() + mid)->cards.begin()->cid == cid) {
        return accounts.begin() + mid;
    } else {
        return accounts.end();
    }
}

void DataStore::insertConsume(Window window, Consumption *data) {
    auto &consumes = getConsumes();
    auto &consumes_in_window = consumes[window];
    int left = 0, right = (int) consumes_in_window.size() - 1, mid;
    //half search
    while (left <= right) {
        mid = (left + right) / 2;
        if (*(consumes_in_window[mid]) < *data) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    consumes_in_window.emplace(consumes_in_window.begin() + mid, data);
}

