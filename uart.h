/*
 * uart.h
 *
 *  Created on: Jul 19, 2024
 *      Author: alper
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f446xx.h"
/*
 * RS485 haberleşmesi DE ve RE pini için varsayılan olarak PC6 pini tanımlanmıştır farklı pin kullanımı için değiştirilebilir.
 */
#define RS485_CTRL_PORT						GPIOC
#define RS485_CTRL_PIN						GPIO_PIN_6
/*
 * @def_group MODE_Types
 */
#define USART_MODE_TX      					((uint32_t)(0x00000008))
#define USART_MODE_RX      					((uint32_t)(0x00000004))
#define USART_MODE_RX_TX    				((uint32_t)(0x0000000C))
/*
 * @def_group WordLength_Types
 */
#define USART_WORDLENGTH_8BITS				((uint32_t)(0x00000000))
#define USART_WORDLENGTH_9BITS				((uint32_t)(0x00001000))
/*
 * @def_group Parity_Types
 */
#define USART_PARITY_ON_EVEN				((uint32_t)(0x0000600))
#define USART_PARITY_ON_ODD					((uint32_t)(0x0000400))
#define USART_PARITY_OFF					((uint32_t)(0x0000000))
/*
 * @def_group StopBits_Types
 */
#define USART_STOPBITS_1					((uint32_t)(0x0000000))
#define USART_STOPBITS_HALF					((uint32_t)(0x0001000))
#define USART_STOPBITS_2					((uint32_t)(0x0002000))
#define USART_STOPBITS_1_HALF				((uint32_t)(0x0003000))
/*
 * @def_group StopBits_Types HardWareFlowControl_Types
 */

#define USART_HW_NONE						((uint32_t)(0x0000000))
#define USART_HW_CTS						((uint32_t)(0x0000200))
#define USART_HW_RTS						((uint32_t)(0x0000100))
#define USART_HW_CTS_RTS					((uint32_t)(0x0000300))

#define __USART_DIV_VALUE_16(__CLK__,__BAUD__) (25*(__CLK__)/(4*(__BAUD__)))

typedef enum
{
	RS485,
	RS422

}RSXXX_ModeTypeDef_t;

typedef enum
{
	USART_DISABLE = 0x00,
	USART_ENABLE = !USART_DISABLE

}USART_FunctionalState_t;

using GPIODEF = \
struct USART_InitTypedef_t {

	uint32_t Mode;     					/*UART modlarını ayarlar @def_group MODE_Types mocroları ile ulaşılabilir*/
	uint32_t BaudRate; 					/* UART Baud Rate (veri iletim hızı) ayarlaması yapılır.*/
	uint32_t Parity;   					/*UART Parity bit ayarını yapar.*/
	uint32_t StopBits; 					/*UART Stop bit ayarını yapar.*/
	uint32_t WordLength;				/*UART Word Length (veri kelime uzunluğu) ayarını yapar.*/
	uint32_t HardWareFlowControl;		/*UART Hardware Flow control (donanım akış kontrolü ) ayarını yapar*/

};
struct UART_HandleTypedef_t {

	GPIODEF Init;
	USART_TypeDef *Instance;

};

class UARTBase{
public:
	UARTBase(UART_HandleTypedef_t *USART_Handle, RSXXX_ModeTypeDef_t RSXXX = RS422);
	void UART_Recieve(UART_HandleTypedef_t *USART_Handle,uint8_t *pData,uint16_t dataSize);
	void USART_Transmit(UART_HandleTypedef_t *USART_Handle,uint8_t *pData,uint16_t dataSize);

private:
	void USART_PerphCmd(UART_HandleTypedef_t *USART_Handle,USART_FunctionalState_t State);
	void init(UART_HandleTypedef_t *USART_Handle, RSXXX_ModeTypeDef_t RSXXX);
	void set_rs485_control(bool enable);
	RSXXX_ModeTypeDef_t mode;
};

#endif /* INC_UART_H_ */
