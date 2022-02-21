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
     * use DataStore::insert instead
     * */
    static Accounts &getAccounts() {
        static Accounts accounts = accounts_init();
        return accounts;
    }

    /*
     * Account insert func
     * */
    static void insertAccount(const Account &data) {
        auto itr = getAccounts().begin();
        getAccounts().emplace(itr, data);
    }

    static Consumes &getConsumes() {
        static Consumes consumes = consumes_init();
        return consumes;
    }

    /* const data because it defined by file ,unmodifiable */
    static const WindowPositions &getWindowPositions() {
        static const WindowPositions windowPositions = windows_init();
        return windowPositions;
    }

    /* localize file stored by DataStore */
    static void localize();

private:

    static Consumes consumes_init();

    static WindowPositions windows_init();

    static Accounts accounts_init();

    constexpr static const char WINDOW_QTY = 99;
    constexpr static const int MAXSIZE = 60000;
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
