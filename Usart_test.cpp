/*
 * Usart_test.cpp
 *
 *  Created on: 19 Tem 2024
 *      Author: alper
 */
#include "main.h"
#include "uart.h"

UART_HandleTypedef_t Handle;


void RS485_GPIO_Config(void);
void UARTConfig(void);

int main(void)
{
	uint8_t data [] = { 0x66, 0x65, 0x72, 0x67, 0x61, 0x6E, 0x69}; // f: 0x66 e: 0x65 r: 0x72 g: 0x67 a: 0x61 n: 0x6E i: 0x69
	UARTConfig();


	/****************      RS422     ************************/
	UARTBase huart1(&Handle);


	huart1.USART_Transmit(&Handle, data , 7);
	huart1.UART_Recieve(&Handle, data, 1);

	/****************      RS485     ************************/
	UARTBase huart2(&Handle,RS485);


	huart2.USART_Transmit(&Handle, data , 7);
	huart2.UART_Recieve(&Handle, data, 1);

	return 0;
}

void UARTConfig(void)
{
	Handle.Instance = USART2;
	Handle.Init.BaudRate = 9600;
	Handle.Init.HardWareFlowControl = USART_HW_CTS_RTS;
	Handle.Init.Mode = USART_MODE_RX_TX;
	Handle.Init.Parity = USART_PARITY_OFF;
	Handle.Init.StopBits = USART_STOPBITS_1;
	Handle.Init.WordLength = USART_WORDLENGTH_8BITS;
}

