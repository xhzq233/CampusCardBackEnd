//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include "Account/Account.h"
#include "Operation/Consumption.h"
#include "Operation/CardManageOperation.h"
#include "Operation/RechargeOperation.h"
#include "../Utils/CircularArray.h"

/* 'Data Base' liked object */
class DataStore {
public:
    static unsigned int getSerialNumber() {
        static unsigned int serialNumber = 12345;
        return ++serialNumber;
    }

    typedef std::vector<Account *> Accounts;
    typedef std::unordered_map<unsigned int, Account *> AccountsMap;
    using Time = FileManager::Time;
    // 数组下标
    typedef std::vector<const Consumption *> QueryResults;
    constexpr static const char WINDOW_QTY = 99;
    constexpr static const unsigned int MAXSIZE = 60000;

    /* each Window have Consumptions, use Consumptions[Window - 1] get each window Consumptions data */
    typedef CircularArray<Consumption *> *Consumptions[WINDOW_QTY];

    // notice that WindowPositions[0] represents the position of Window1
    typedef WindowPosition WindowPositions[WINDOW_QTY];

    /*
     * sorted by <
     * Notice!!!!
     * do not directly modify this ,
     * use DataStore::insert instead .
     * */
    static Accounts &getAccounts();

    /* identifier is cid of an Account, stores addresses of getAccounts() */
    static AccountsMap &getAccountsMapByCid();

    /* sorted by < */
    static Consumptions &getConsumptions();

    /* notice that WindowPositions[0] represents the position of Window1, subscripts ranged from 0 to 98 */
    static const WindowPositions &getWindowPositions();

    /* Account insert func, designed by half find and insert */
    static void insertAccount(Account *data);

    /* Query account by uid, return subscript matched, no res return -1 */
    static Subscript queryAccountByUid(unsigned int uid);

    /* account subscript to account pointer, no res return nullptr */
    static Account *subscript2Account(Subscript subscript);

    /* Consumption push back, window ranged from 1 to 99 */
    static void pushConsumption(Window window, Consumption *data);

    /* OnSpecifiedTime */
    static void insertConsumptionOnSpecifiedTime(Window window, Consumption *data);

    [[nodiscard]] static QueryResults
    queryConsumptionInTimeRange(Window window, Time left, Time right = -1);

    /* localize file stored by DataStore ，be like cache */
    static void localize();

private:
    //called if and only if initializing
    static Consumptions &consumes_init();

    //called if and only if initializing
    static const WindowPositions &windows_init();

    //called if and only if initializing
    static Accounts &accounts_init();

public:
    DataStore() = delete;

    ~DataStore() = delete;
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
