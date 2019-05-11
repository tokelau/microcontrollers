/*
 * Led.h
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Zoya
 */

#ifndef LED_H_
#define LED_H_
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

class Led {

private:
	GPIO_TypeDef *gpio;
	int PIN;
public:

	Led(GPIO_TypeDef* gpio, int PIN);
	void On();
	void Off();
	virtual ~Led();
};

#endif /* LED_H_ */
