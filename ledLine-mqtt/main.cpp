/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "diag/Trace.h"
#include "Led.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void usart_init() {
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	//Tx
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);

	//Rx
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	usart.USART_BaudRate = 9600;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
}

int
main(int argc, char* argv[])
{
	usart_init();
	char rxbuf[64];
	char txbuf[64];
	int rxbuf_pos = 0;
	int percent = 0;

//	Led led12(GPIOB, GPIO_Pin_12);
	Led* leds[8];
	leds[0] = new Led(GPIOB, GPIO_Pin_12);
	leds[1] = new Led(GPIOB, GPIO_Pin_15);
	leds[2] = new Led(GPIOB, GPIO_Pin_13);
	leds[3] = new Led(GPIOB, GPIO_Pin_14);
	leds[4] = new Led(GPIOA, GPIO_Pin_4);
	leds[5] = new Led(GPIOA, GPIO_Pin_3);
	leds[6] = new Led(GPIOA, GPIO_Pin_2);
	leds[7] = new Led(GPIOA, GPIO_Pin_1);

	while (1) {
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		rxbuf[rxbuf_pos++] = USART_ReceiveData(USART1);

		if ((rxbuf[rxbuf_pos - 1] == '\n' || rxbuf[rxbuf_pos - 1] == '\r') && rxbuf_pos != 0) {
//			led.On();
//			USART_SendData(USART1, 'A');
//			for(int i = 0; i < rxbuf_pos; i++) {
//				sscanf(rxbuf, "%d", &percent);
//				USART_SendData(USART1, '\n');
//				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//			}
			sscanf(rxbuf, "%d", &percent);
			percent = (int)(((double) percent) / 100 * 8);
			sprintf(txbuf, "%d", percent);
			USART_SendData(USART1, txbuf[0]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			for (int i = 0; i < 8; i++) {
				leds[i]->Off();
			}
			for (int i = 0; i < percent; i++) {
				leds[i]->On();
			}
			rxbuf_pos = 0;

		}

	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
