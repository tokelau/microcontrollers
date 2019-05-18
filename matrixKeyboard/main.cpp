#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


char buffer[50] = {'\0'};

void send(char* charBuffer, unsigned int count) {
	while (count--) {
		USART_SendData(USART1, *charBuffer++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}

void delay(void) {
	volatile uint32_t i;
	for (i = 0; i <= 0x500000; i++);
}

void usartIni(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef initStruct;

//	TX
	initStruct.GPIO_Pin = GPIO_Pin_9;
	initStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &initStruct);

//	RX
	initStruct.GPIO_Pin = GPIO_Pin_10;
	initStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &initStruct);

//	UART
	USART_InitTypeDef usartStruct;
	usartStruct.USART_BaudRate = 115200;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &usartStruct);
	USART_Cmd(USART1, ENABLE);
}

void clear() {
	for (unsigned int i = 0; i < sizeof(buffer) - 2; i++) {
		buffer[i] = ' ';
	}

	buffer[sizeof(buffer) - 2] = '\r';
	buffer[sizeof(buffer) - 1] = '\n';
}

int main(void) {
	usartIni();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	initStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &initStruct);

	initStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	initStruct.GPIO_Mode = GPIO_Mode_IPU;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &initStruct);

	uint16_t pinCols[3] = {GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
	uint16_t pinRows[4] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3};

	while (true) {
//		Устанавливаем HIGH на все строки.
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);

		for (int j = 0; j < 3; j++) {
			if (!GPIO_ReadInputDataBit(GPIOB, pinCols[j])) {
				clear();
				for (int i = 0; i < 4; i++) {
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, i == 0 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_1, i == 1 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_2, i == 2 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOA, GPIO_Pin_3, i == 3 ? Bit_RESET : Bit_SET);


					if (!GPIO_ReadInputDataBit(GPIOB, pinCols[j])) {
						sprintf(buffer, "row: %d, col: %d\r\n", i, j);
						send(buffer, sizeof(buffer));
						break;
					}
				}
			}
		}

		delay();
	}
}
