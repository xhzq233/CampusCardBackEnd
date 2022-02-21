#include "Consume.h"

string to_time(int date, int time) //将date和time转换为日志的标准格式
{
    const char *week[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    int year = date / 10000;
    int month = (date % 10000) / 100;
    int day = date % 100;
    int w;
    if (month == 1 || month == 2)
    {
        w = (day + 2 * (month + 12) + 3 * (month + 13) / 5 + year - 1 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) % 7 + 1;
    }
    else
    {
        w = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7 + 1;
    }
    int hour = time / 1000000;
    int minute = time / 10000 % 100;
    int second = time % 10000 / 100;
    stringstream s;
    s << week[w] << ' ' << months[month - 1] << ' ' << day << ' ' << hour << ':' << minute << ':' << second << ' ' << year;
    return s.str();
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
        int index = (int)str.find(',');
        windows[stoi(str.substr(0, index))] = stoi(str.substr(index + 1, str.size() - index - 2));
    }
}

void Consume::consumeByFile()
{
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container, FileManager::CONSUME_CSV(curWindow));
    int index[3] = {7, 16, 25}; //三个分隔符,的位置
    for (auto &&str : container)
    {
        int cid = stoi(str.substr(0, index[0]));
        int date = stoi(str.substr(index[0] + 1, index[1] - index[0] - 1));
        int time = stoi(str.substr(index[1] + 1, index[2] - index[1] - 1));
        float price = stof(str.substr(index[2] + 1, str.size() - index[2] - 1));
        consume(CardManage::getInstance()->getCardByCid(cid), price, date, time);
    }
}

void Consume::log(const string &title, const string &content, const time_t &_time)
{
    FileManager::getInstance() << FileManager::toStandardLogString(title.c_str(), content.c_str(), _time) << FileManager::endl;
}

void Consume::log(const string &title, const string &content, const char *_time)
{
    FileManager::getInstance() << FileManager::toStandardLogString(title.c_str(), content.c_str(), _time) << FileManager::endl;
}

bool Consume::consume(Card &card, float price)
{
    if (card.condition && card.getBalance() > price)
    {
        //获得当前时间
        time_t now = time(nullptr);
        int hour = gmtime(&now)->tm_hour;
        //免密支付的时间
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 || hour <= 19)
        {
            if (price < 20)
            {
                card.consume(price);
                Consume::log("消费", string("s"), time(nullptr));
                return true;
            }
            else if (price > 20 && inputPassword(card))
            {
                card.consume(price);
                Consume::log("消费", string("s"), time(nullptr));
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

bool Consume::consume(Card &card, float price, int date, int _time)
{
    if (card.condition && card.getBalance() > price)
    {
        int hour = _time / 1e6;
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 || hour <= 19)
        {
            if (price < 20)
            {
                card.consume(price);
                Consume::log("消费", string("s"), move(to_time(date, _time)).c_str());
                return true;
            }
            else if (price > 20 && inputPassword(card))
            {
                card.consume(price);
                Consume::log("消费", string("s"), move(to_time(date, _time)).c_str());
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