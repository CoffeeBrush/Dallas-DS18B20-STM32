/*
  ******************************************************************************
  * @file      DS18B20.h
  * @author    Christopher Stevenson
  * @brief     Header file for Dallas DS18B20 temperature sensor on STM32 boards.
  *
  ******************************************************************************
  * Note:
  ******************************************************************************
*/

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#ifdef __cplusplus
extern "C" {
#endif

/******Includes*************/

#include "stm32l4xx_hal.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

/***************************/

#define DS18B20_PORT GPIOB
#define DS18B20_PIN GPIO_PIN_0
#define DS18B20_POWER_PORT GPIOA
#define DS18B20_POWER_PIN GPIO_PIN_12

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;
uint8_t DS18B20_uart_buf[24];
uint8_t Presence ;
float DS18B20_Temperature ;

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t DS18B20_Start(void);
void DS18B20_Write (uint8_t data);
uint8_t DS18B20_Read (void);
void Display_Temp(float Temp);
void DS18B20_Print_Presence(uint8_t Presence);
float DS18B20_Return_Temp(void);



#ifdef __cplusplus
}
#endif
#endif /* INC_DS18B20_H_ */
