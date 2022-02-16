#include "Consume.h"

Consume *Consume::getInstance()
{
    if (!instance)
    {
        instance = new Consume();
    }
    return instance;
}

Consume::Consume()
{
    data.reserve(MAXSIZE);
    windows.reserve(99);
    initPosition();
    cout << "输入当前的窗口号:";
    cin >> curWindow;
}

void Consume::initPosition()
{
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container, FileManager::CAFE_POSITION_NAME);
    for (auto &&str : container)
    {
        int index = str.find(',');
        windows[stoi(str.substr(0, index).c_str())] = stoi(str.substr(index + 1, str.size() - index - 2).c_str());
    }
}

void Consume::consumeByFile()
{
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container, FileManager::CONSUME_CSV(curWindow));
    int index[3] = {7, 16, 25};//三个分隔符,的位置
    for (auto &&str : container)
    {
        int cid = atoi(str.substr(0, index[0]).c_str());
        int date = atoi(str.substr(index[0] + 1, index[1] - index[0] - 1).c_str());
        int time = atoi(str.substr(index[1] + 1, index[2] - index[1] - 1).c_str());
        float price = atof(str.substr(index[2] + 1, str.size() - index[2] - 1).c_str());
        consume(CardManage::getInstance()->getCardByCid(cid), price, date, time);
    }
}

bool Consume::consume(Card &card, unsigned int price, int date = 0, int _time = 0)
{
    if (card.condition && card.getBalance() > price)
    {
        int hour;
        if (!date)
        {
            //获得当前时间
            time_t now = time(nullptr);
            hour = gmtime(&now)->tm_hour;
        }
        else
        {
            hour = _time / 1e6;
        }
        //免密支付的时间
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 || hour <= 19)
        {
            if (price > 20 && inputPassword(card))
            {
                card.consume(price);
                FileManager::getInstance() << "[Consume:log] [time:] [content: consume.to_string()] " << FileManager::endl;
                return true;
            }
        }
        else
        {
            if (inputPassword(card))
            {
                return true;
            }
        }
    }
    return false;
}

bool Consume::inputPassword(Card &card)
{
    unsigned int password;
    std::cin >> password;
    if (card.checkPassword(password))
        return true;
    return false;
}

bool Consume::init()
{
    data.reserve(MAXSIZE);
    windows.reserve(99);
    curIndex = 0;

    vector<vector<string>> container(99, {"", ""});
    ///预定好尺寸
    FileManager::getInstance().getCSVDataSource(container, Pair((unsigned int)99, (unsigned int)2),
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标
    for (auto &&row : container)
    {
        windows[stoi(row[0])] = stoi(row[1]);
    }

    //选择窗口
    std::cin >> curWindow;
    //选择时间？
    return false;
}