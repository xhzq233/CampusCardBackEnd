//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include "Account/Account.h"
#include "Operation/Consumption.h"
#include "Operation/CardManageOperation.h"
#include "Operation/RechargeOperation.h"
#include "../Container/CircularArray.h"

/* 'Data Base' liked object */
class DataStore {
public:

    typedef std::vector<Account> Accounts;

    // 数组下标
    typedef std::vector<unsigned int> Subscripts;


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

    /* sorted by < */
    static Consumptions &getConsumptions();

    /* notice that WindowPositions[0] represents the position of Window1, subscripts ranged from 0 to 98 */
    static const WindowPositions &getWindowPositions();

    /* Account insert func, designed by half find and insert */
    static void insertAccount(const Account &data);

    /* Query account by uid */
    static std::vector<Account>::iterator queryAccountByUid(unsigned int uid);

    /* Query account by cid */
    static std::vector<Account>::iterator queryAccountByCid(unsigned int uid);

    /* Consumption push back, window ranged from 1 to 99 */
    static void pushConsumption(Window window, Consumption *data);

    /* OnSpecifiedTime */
    static void insertConsumptionOnSpecifiedTime(Window window, Consumption *data, const FileManager::Time &time);

    /* on specified window and card id, return Subscripts matched */
    static Subscripts queryConsumption(Window window, unsigned int cid);

    /* on specified card id, return Subscripts matched */
    static Subscripts queryConsumption(unsigned int cid);

    /* on specified account id, return Subscripts matched */
    static Subscripts queryConsumptionByUid(unsigned int uid);

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
    /**
     * #模糊匹配的格式中，？代表一个字符或一个汉字，*表示多个字符或多个汉字，或代表空；
     * 汉字：[\u4e00-\u9fa5]
     * 将?替换成 .
     * 将*替换成 .{2,}
    * */
    static std::regex customRegex2CommonRegexSyntax(std::string &regex);

    // query on specified multi string, return Subscripts matched
    static Subscripts query(FileManager::Strings &container, const std::regex &regex);

    // query on CSV data specified column, return Subscripts matched
    static Subscripts query(FileManager::CSV &container, unsigned int columnIndex, const std::regex &regex);

    DataStore() = delete;

    ~DataStore() = delete;
};

static unsigned int serialNumber = 12345;                                    //流水号

#endif //CAMPUSCARDBACKEND_DATASTORE_H
