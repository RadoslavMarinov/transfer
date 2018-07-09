/*
 * main.c
 *
 *  Created on: 4.07.2018 ã.
 *      Author: Radoslav Marinov
 */
#include "main-primary.h"
#include "main.h"
//#include "application/zw-transmitter/zw-transmitter.h"

void errorHandler();
static void zwTransmitter_initInterrupts();
static void zwTransmitter_initUartProtocol();
static void zwTransmitter_initUartPins();
void zwTransmitter_initUart();

uint8_t aTxBuffer[] = {0, 1, 2, 3};
static main_SelfData main_self;
UART_HandleTypeDef uart1Handle;


int main(void)
{
	HAL_Init();
	main_clockSystemInit();
	zwTransmitter_initUart();

	while (HAL_UART_GetState(&uart1Handle) != HAL_UART_STATE_READY)
	{
		;
	}
	if(HAL_UART_Transmit_IT(&uart1Handle, (uint8_t*)aTxBuffer, sizeof(aTxBuffer))!= HAL_OK)
	{
		aTxBuffer[0] = 5;
	}
	while(1)
	{
		;
	}
}

void main_clockSystemInit()
{
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_OscInitTypeDef RCC_OscInitStruct;

	  /* Enable Power Control clock */
	  __HAL_RCC_PWR_CLK_ENABLE();

	  /* The voltage scaling allows optimizing the power consumption when the device is
	     clocked below the maximum system frequency, to update the voltage scaling value
	     regarding system frequency refer to product datasheet.  */
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	  /* Enable HSE Oscillator and activate PLL with HSE as source */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 8;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 7;
	  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		  MAIN_raiseError(HAL_RCC_OscConfig_err);
	  }


	  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	     clocks dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	  {
		  MAIN_raiseError(HAL_RCC_ClockConfig);
	  }
}



void zwTransmitter_initUart()
{
	zwTransmitter_initUartPins();
	zwTransmitter_initUartProtocol();
	zwTransmitter_initInterrupts();
}


static void zwTransmitter_initUartPins()
{
	GPIO_InitTypeDef  zwTransmitter_gpioInitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	zwTransmitter_gpioInitStruct.Pin 		= GPIO_PIN_9;
	zwTransmitter_gpioInitStruct.Mode       = GPIO_MODE_AF_PP;
	zwTransmitter_gpioInitStruct.Pull       = GPIO_NOPULL;
	zwTransmitter_gpioInitStruct.Speed      = GPIO_SPEED_FAST;
	zwTransmitter_gpioInitStruct.Alternate  = GPIO_AF7_USART1;

	HAL_GPIO_Init(GPIOA,  &zwTransmitter_gpioInitStruct);
}

static void zwTransmitter_initUartProtocol()
{

	__HAL_RCC_USART1_CLK_ENABLE();

	uart1Handle.Instance          = USART1;
	uart1Handle.Init.BaudRate     = 115200;
	uart1Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	uart1Handle.Init.StopBits     = UART_STOPBITS_1;
	uart1Handle.Init.Parity       = UART_PARITY_NONE;
	uart1Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	uart1Handle.Init.Mode         = UART_MODE_TX_RX;
	uart1Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&uart1Handle) != HAL_OK)
	{
		errorHandler();
	}
}


static void zwTransmitter_initInterrupts()
{
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void errorHandler()
{
	while(1)
	{
		;
	}
}






















//void USART1_IRQHandler()
//{
//	MAIN_raiseError(HAL_RCC_OscConfig_err);
//}
//void main_gpioInit()
//{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//
//	MAIN_gpioPortAClockEnable();
//
//
//	GPIO_InitStruct.Pin = ZW_TX_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//Output Push Pull
//	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA, ZW_TX_PIN, GPIO_PIN_RESET);
//}


