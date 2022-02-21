//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include <vector>
#include "Card.h"
#include "Window.h"
#include "FileManager.h"
#include "Account.h"
#include "Consume.h"

class DataStore {
public:
    typedef std::vector<WindowPosition> WindowPositions;
    typedef std::vector<Account> Accounts;
    typedef std::vector<Consume> Consumes;

    /*
     * Notice!!!!
     * do not use accounts.insert() or push_back()
     * */
    static Accounts &getAccounts() {
        static Accounts accounts = accounts_init();
        return accounts;
    }

    /*
     * generic insert func
     * */
    template<typename T>
    static void insert(const T &data) {
        static_assert(std::is_base_of<Account, T>::value || std::is_base_of<Consume, T>::value, "unsupported type");

        if (std::is_base_of<Account, T>::value) {
            auto itr = halfFind();
            getAccounts().emplace(itr, data);
        } else if (std::is_base_of<Consume, T>::value) {
            getConsumes().push_back(data);
        } else {
            throw;
        }
    }

    typedef unsigned int index;

    static index halfFind();

    static Consumes &getConsumes() {
        static Consumes consumes = consumes_init();
        return consumes;
    }

    /* const data because it defined by file ,unmodifiable */
    static const WindowPositions &getWindowPositions() {
        static const WindowPositions windowPositions = windows_init();
        return windowPositions;
    }

    static void localize() {

    }

private:

    static Consumes consumes_init();

    static WindowPositions windows_init();

    static Accounts accounts_init();

    constexpr static const char WINDOW_QTY = 99;
    constexpr static const int MAXSIZE = 60000;
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
