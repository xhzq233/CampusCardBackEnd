//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include "Account/Account.h"
#include "Consumption/Consumption.h"

/* 'Data Base' liked object */
class DataStore {
public:

    typedef std::vector<Account> Accounts;

    constexpr static const char WINDOW_QTY = 99;
    constexpr static const int MAXSIZE = 60000;

    /* each Window have Consumptions, use Consumptions[Window - 1] get each window Consumptions data */
    typedef Consumption *Consumptions[WINDOW_QTY][MAXSIZE];

    // notice that WindowPositions[0] represents the position of Window1
    typedef WindowPosition WindowPositions[WINDOW_QTY];

    /*
     * sorted by <
     * Notice!!!!
     * do not use accounts.insert() or push_back()
     * use DataStore::insert instead
     * */
    static Accounts &getAccounts();

    /* Account insert func, designed by half find and insert */
    static void insertAccount(const Account &data);

    /* Query account by uid */
    static std::vector<Account>::iterator queryAccountByUid(unsigned int uid);

    /* Query account by cid */
    static std::vector<Account>::iterator queryAccountByCid(unsigned int uid);

    /* Consumption insert func, window ranged from 1 to 99 */
    static void insertConsumption(Window window, Consumption *data);

    /* sorted by < */
    static Consumptions &getConsumptions();

    /* const data because it defined by file ,unmodifiable */
    static WindowPositions &getWindowPositions();

    /* localize file stored by DataStore ，be like cache */
    static void localize();

private:

    //called if and only if initializing
    static Consumptions &consumes_init();

    //called if and only if initializing
    static WindowPositions &windows_init();

    //called if and only if initializing
    static Accounts &accounts_init();
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
