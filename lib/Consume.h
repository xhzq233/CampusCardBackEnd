//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_CONSUME_H
#define CAMPUSCARDBACKEND_CONSUME_H

#include<ctime>
#include<iostream>
#include"Card.h"
#include"Cafe.h"

class Consume {

	bool consume(Card& card, Cafe& cafe, unsigned int price);		//消费操作
	bool InputPassword(Card& card);									//输入密码
	void log();														//日志打印

};


#endif //CAMPUSCARDBACKEND_CONSUME_H
