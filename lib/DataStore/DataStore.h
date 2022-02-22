//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include "Account/Account.h"
#include "Consume/Consume.h"

class DataStore {
public:
    typedef std::vector<WindowPosition> WindowPositions;
    typedef std::vector<Account> Accounts;

    /* each Window have consumes, use Consumes[Window] get each data */
    typedef std::vector<std::vector<Consume *>> Consumes;

    /*
     * Notice!!!!
     * do not use accounts.insert() or push_back()
     * use DataStore::insert instead
     * */
    static Accounts &getAccounts();

    /* Account insert func */
    static void insertAccount(const Account &data);

    /* Account insert func */
    static void insertConsume(Window window, Consume &data);

    static Consumes &getConsumes();

    /* const data because it defined by file ,unmodifiable */
    static const WindowPositions &getWindowPositions();

    /* localize file stored by DataStore */
    static void localize();

    constexpr static const char WINDOW_QTY = 99;
    constexpr static const int MAXSIZE = 60000;
private:

    //called if and only if initializing
    static Consumes &consumes_init();

    //called if and only if initializing
    static const WindowPositions &windows_init();

    //called if and only if initializing
    static Accounts &accounts_init();
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
