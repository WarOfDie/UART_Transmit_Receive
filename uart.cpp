/*
 * uart.cpp
 *
 *  Created on: Jul 19, 2024
 *      Author: alper
 */
#include "uart.h"
#include "stm32f4xx_hal.h"
UARTBase::UARTBase(UART_HandleTypedef_t *USART_Handle)
{
	init(USART_Handle);
}
/**
 * @brief  UART'ın modlarını USART_InitTypedef_t ile özelleştirilmiş şekilde başlatır.
 * @param  USART_Handle     UART_HandleTypedef_t yapısını işaret eden pointer yapı UART modülünün ayarlamalarını içerir.
 * @retval void
 */
void UARTBase::init(UART_HandleTypedef_t *USART_Handle)
{
	uint32_t tempReg = 0;
	uint32_t mantissaPart = 0;
	uint32_t fractionPart = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t pclk;
	uint32_t tempValue = 0;

	/***********    Word Length Mode Parity      **********/
	tempReg = USART_Handle->Instance->CR1;
	tempReg |= (USART_Handle->Init.WordLength) |(USART_Handle->Init.Mode) | \
			(USART_Handle->Init.Parity);
	USART_Handle->Instance->CR1 = tempReg;
	/**********          Stop Bits               ***********/
	tempReg = USART_Handle->Instance->CR2;
	tempReg &=~(0x3U << 12U);
	tempReg |= (USART_Handle->Init.StopBits);
	USART_Handle->Instance->CR2 = tempReg;
	/**********     Hardware Flow Control        ***********/
	tempReg = USART_Handle->Instance->CR3;
	tempReg |= USART_Handle->Init.HardWareFlowControl;
	USART_Handle->Instance->CR3 = tempReg;
	/**********          Baud Rate               ***********/
	if( USART_Handle->Instance == USART1 || USART_Handle->Instance == USART6 )
	{

		pclk = HAL_RCC_GetPCLK2Freq(); /*Baud rate ayarlanması için USART1 ve USART6'nın bağlı olduğu APB2 hattının clock frekansı alınır*/


	}
	else
	{

		pclk = HAL_RCC_GetPCLK1Freq(); /*Baud rate ayarlanması için USART1 ve USART6'nın haricindeki modüllerin bağlı  olduğu APB1 hattının clock frekansı alınır*/
	}

	USART_DIV_Value = __USART_DIV_VALUE_16(pclk, USART_Handle->Init.BaudRate);
	mantissaPart = (USART_DIV_Value / 100U);
	fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);

	fractionPart = (((fractionPart * 16U) + 50U) / 100U) &(0x0FU);

	tempValue |= (mantissaPart << 4);
	tempValue |= fractionPart;

	USART_Handle->Instance->BRR = tempValue;

	USART_PerphCmd(USART_Handle,USART_ENABLE);
}
/**
 * @brief  Belirilen miktada datayı gönderir.
 * @param  USART_Handle     UART_HandleTypedef_t yapısını işaret eden pointer yapı UART modülünün ayarlamalarını içerir.
 * @param  pData bufferın adresini tutar
 * @param  dataSize gönderilecek data miktarını tutar.
 * @retval void
 */
void UARTBase::USART_Transmit(UART_HandleTypedef_t *USART_Handle,uint8_t *pData,uint16_t dataSize){



	uint16_t *data16Bits;

	if(USART_Handle->Init.WordLength == USART_WORDLENGTH_9BITS && USART_Handle->Init.Parity == USART_PARITY_OFF)
	{
		data16Bits = (uint16_t*)(pData);
	}
	else
	{
		data16Bits = NULL;
	}
	while(dataSize >0)
	{
		while(!(USART_Handle->Instance->SR & (0x1U << 7U))); /* Veri shift registere geçene kadar bekler*/

		if(data16Bits == NULL)
		{
			USART_Handle->Instance->DR = (*pData & 0xFF);
			pData++;
			dataSize--;
		}
		else
		{
			USART_Handle->Instance->DR = uint16_t(*data16Bits & (0x1FFU));
			data16Bits++;
			dataSize -=2;
		}

	}
	while(!(USART_Handle->Instance->SR == (0x1U << 6)));  /*Veri gönderimi bitene kadar bekler*/



}
/**
 * @brief  Belirilen miktada datayı alır.
 * @param  USART_Handle     UART_HandleTypedef_t yapısını işaret eden pointer yapı UART modülünün ayarlamalarını içerir.
 * @param  pData bufferın adresini tutar
 * @param  dataSize gönderilecek data miktarını tutar.
 * @retval void
 */
void UARTBase::UART_Recieve(UART_HandleTypedef_t *USART_Handle,uint8_t *pData,uint16_t dataSize)
{

	uint16_t *bit16Buffer = 0;
	uint8_t *bit8Buffer = 0;


	if((USART_Handle->Init.WordLength == USART_WORDLENGTH_9BITS) && (USART_Handle->Init.Parity == USART_PARITY_OFF))
	{

		bit16Buffer = (uint16_t*)pData;
		bit8Buffer = NULL;
	}
	else
	{
		bit8Buffer = (uint8_t*)pData;
		bit16Buffer = NULL;
	}

	while(dataSize > 0)
	{

		while(!(USART_Handle->Instance->SR  & (0x1U << 5) ))		 /*data shift registere alındığında RXE biti birlenir data gelene kadar RXE bayrağı beklenir*/
			if(bit16Buffer == NULL)
			{
				if(USART_Handle->Init.WordLength == USART_WORDLENGTH_9BITS)
				{
					*bit8Buffer = (USART_Handle->Instance->DR & 0xFF);
					pData++;
					dataSize--;
				}
				else if ((USART_Handle->Init.WordLength == USART_WORDLENGTH_8BITS) && (USART_Handle->Init.Parity == USART_PARITY_OFF))
				{
					*bit8Buffer = (USART_Handle->Instance->DR & 0xFF);
					pData++;
					dataSize--;
				}
				else
				{
					*bit8Buffer = (USART_Handle->Instance->DR & 0x7F);
					pData++;
					dataSize--;
				}
			}
			else
			{
				*bit16Buffer = (USART_Handle->Instance->DR & 0x1FF);
				pData++;
				dataSize -=2;

			}
	}

}
/**
 * @brief  UART Modülünü aktifleştirir.
 * @param  USART_Handle     UART_HandleTypedef_t yapısını işaret eden pointer yapı UART modülünün ayarlamalarını içerir.
 * @param  USART_FunctionalState_t türünden functional state durumu alır.
 * @retval void
 */
void UARTBase :: USART_PerphCmd(UART_HandleTypedef_t *USART_Handle, USART_FunctionalState_t State)
{
	if(State == USART_ENABLE)
	{
		USART_Handle->Instance->CR1 |= (0x1U << 13U);
	}
	else
	{
		USART_Handle->Instance->CR1 &= ~(0x1U << 13U);
	}
}
