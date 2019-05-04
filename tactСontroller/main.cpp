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



int main(int argc, char* argv[])
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Pin = GPIO_Pin_3;
	initStruct.GPIO_Mode = GPIO_Mode_IPU;
	initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &initStruct);

	Led led5(GPIOB, GPIO_Pin_5);
	Led led8(GPIOB, GPIO_Pin_8);
	Led led9(GPIOB, GPIO_Pin_9);
	int k = 0x50000;
	int downs = 0;
	bool state = false;

	while (1) {
//		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) {
//			led5.Off();
//			led8.Off();
//			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
//				led9.Off();
//			} else {
//				led9.On();
//			}
//		}
//		else{
//			led5.On();
//			led8.On();
//		}

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) {
			k += 10;
			if (k > 0x50000) {
				k = 0x50000;
			}
		}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
			k -= 10;
			if (k < 0) {
				k = 0x50000;
			}
		}

		downs += 1;
		downs = downs%k;
		if (downs == 0) {
			if (state) {
				led9.On();
			} else {
				led9.Off();
			}
			state = !state;
		}
	}
	return 1;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
