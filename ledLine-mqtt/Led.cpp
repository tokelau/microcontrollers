/*
 * Led.cpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Zoya
 */

#include "Led.h"

Led::Led(GPIO_TypeDef *gpio, int PIN) {
	//this->power = false;
	this->PIN = PIN;
	this->gpio = gpio;

	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	initStruct.GPIO_Pin = PIN;
	initStruct.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_Init(this->gpio, &initStruct);
//	GPIO_ResetBits(this->gpio, PIN);
	// TODO Auto-generated constructor stub

}

void Led::On(){
	//int i;
	 GPIO_WriteBit(this->gpio, this->PIN, Bit_SET);
	// for(i = 0; i<0x500000; i++);
}
void Led::Off(){
	//int i;
	 GPIO_WriteBit(this->gpio, this->PIN, Bit_RESET);
	// for(i = 0; i<0x500000; i++);

}
Led::~Led() {
	// TODO Auto-generated destructor stub
}

