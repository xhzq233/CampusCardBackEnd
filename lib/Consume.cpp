//
// Created by 夏侯臻 on 2022/1/14.
//

#include "Consume.h"

bool Consume::consume(Card& card, Cafe& cafe, unsigned int price)
{
	if (card.condition && card.getBalance() > price) {
		//获得当前时间
		time_t now = time(nullptr);
		tm* gmtm = gmtime(&now);
		//免密支付的时间
		if (gmtm->tm_hour >= 7 && gmtm->tm_hour <= 9 || gmtm->tm_hour >= 11 && gmtm->tm_hour <= 13 || gmtm->tm_hour >= 17 || gmtm->tm_hour <= 19) {
			if (price > 20&&inputPassword(card)) {
                FileManager::getInstance()<<"[Consume:log] [time: "<< ctime(&now)<<" ] [content: consume.to_string()] " <<FileManager::endl;
				return true;
			}
		}
		else {
			if (inputPassword(card)) {

				return true;
			}
		}
	}
	return false;
}

bool Consume::inputPassword(Card& card)
{
	unsigned int password;
	std::cin >> password;
	if (card.checkPassword(password)) return true;
	return false;
}


bool Consume::init() {
    data.reserve(MAXSIZE);
    windows.reserve(99);
    curIndex = 0;

    vector<vector<string>> container(99, {"", ""});
    ///预定好尺寸
    FileManager::getInstance().getCSVDataSource(container, Pair((unsigned int) 99, (unsigned int) 2),
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标
    for (auto && row:container) {
        windows[stoi(row[0])] = stoi(row[1]);
    }

    //选择窗口
    std::cin >> curWindows;
    //选择时间？
    return false;
}