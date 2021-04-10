/*
  ******************************************************************************
  * @file      DS18B20.c
  * @author    Christopher Stevenson
  * @brief     Driver for Dallas DS18B20 temperature sensor for STM32 boards.
  *
  ******************************************************************************
  * Note:
  ******************************************************************************
*/


#include "stdio.h"
#include <string.h>
#include "DS18B20.h"


TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;



void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6))<time);

}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


uint8_t DS18B20_Start(void)
{
	uint8_t Responce = 0;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
	HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);
	delay(480);

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
	delay(80);
	if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Responce = 1;
	else Responce = -1;
	delay (400);
	return Responce;
}

void DS18B20_Write (uint8_t data)
{
	for (int i=0; i<8; i++)
	{
		if ((data & (1 <<i)) !=0)
		{
			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);
			delay(1);
			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
			delay(60);
		}
		else
		{
			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);
			delay(60);
			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		}
	}
}

uint8_t DS18B20_Read (void)
{
	uint8_t value = 0;

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);

	for (int i=0; i<8; i++)
	{
		Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
		HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);
		delay(2);
		Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		if (HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))
		{
			value |= 1<<i;
		}
		delay(60);
	}
	return value;
}


void DS18B20_Print_Presence(uint8_t Presence)
{
	if ((Presence) !=(1))
	{
		  strcpy((char*)DS18B20_uart_buf, "Sensor Not Present\r\n");
		  HAL_UART_Transmit(&huart2, DS18B20_uart_buf, strlen((char*)DS18B20_uart_buf),HAL_MAX_DELAY);

	}
	else
	{
		  strcpy((char*)DS18B20_uart_buf, "Sensor Present\r\n");
		  HAL_UART_Transmit(&huart2, DS18B20_uart_buf, strlen((char*)DS18B20_uart_buf),HAL_MAX_DELAY);
	}
}

float DS18B20_Return_Temp(void)
{
    HAL_GPIO_WritePin (DS18B20_POWER_PORT, DS18B20_POWER_PIN, 1);

	Presence = DS18B20_Start ();
	HAL_Delay (1);
	DS18B20_Write (0xCC);
	DS18B20_Write (0x44);
	HAL_Delay(800);

	Presence = DS18B20_Start ();
	HAL_Delay(1);
	DS18B20_Write (0xCC);
	DS18B20_Write (0xBE);

	Temp_byte1 = DS18B20_Read();
	Temp_byte2 = DS18B20_Read();
	TEMP = (Temp_byte2<<8)|Temp_byte1;
	DS18B20_Temperature = (float)TEMP/16;
	return DS18B20_Temperature;
}
