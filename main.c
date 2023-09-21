/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "bootloader_command_codes.h"
#include "bootloader_command_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_SECTOR2_BASE_ADRR			(0x08008000U)

#define BL_RX_DATA_LENGHT (100U)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t bootloader_rx_data[BL_RX_DATA_LENGHT];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

void print_m(const char *data, ...);

void bootloader_uart_data_read(void);
void bootloader_jump_userApp(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Buffer[20];
char uart_data;
char resultBuffer[20];
uint8_t index_Buffer;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart_data, 1);
  //bootloader_jump_userApp();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void print_m(const char *data, ...)
{
	va_list arg;
	const char *tempData = data;
	char c;
	int d;
	char integerBuffer[20];
	char *s;
	va_start(arg, data);

	while(*tempData != '\0')
	{
		if(*tempData == '%')
		{
			if(*(tempData + 1) == 'c')
			{
				c = va_arg(arg , int);
				HAL_UART_Transmit(&huart1,(uint8_t *)&c , 1, 1000);
				tempData++;
			}
			else if(*(tempData + 1) == 'd')
			{
				d = va_arg(arg, int);
				itoa(d, integerBuffer,10);
				HAL_UART_Transmit(&huart1,(uint8_t *)integerBuffer , strlen(integerBuffer), 2000);
				tempData++;
			}
			else if(*(tempData + 1) == 's')
			{
				s = va_arg(arg, char*);
				HAL_UART_Transmit(&huart1,(uint8_t *)s , strlen(s), 2000);
				tempData++;
			}
			else
			{
				HAL_UART_Transmit(&huart1,(uint8_t *)"%" , 1, 2000);
			}
		}
		else
		{
			HAL_UART_Transmit(&huart1,(uint8_t *)tempData , 1, 2000);
		}
		tempData++;
	}

	//USART_Transmit(&huart, (uint8_t*)data, 20);
	va_end(arg);

}
void bootloader_uart_data_read(void)
{
	uint8_t bl_rx_lenght = 0;

	while(1)
	{
		memset(bootloader_rx_data, 0, BL_RX_DATA_LENGHT);
		HAL_UART_Receive(&huart1, bootloader_rx_data, 1, HAL_MAX_DELAY);

		bl_rx_lenght = bootloader_rx_data[0];

		HAL_UART_Receive(&huart1, &bootloader_rx_data[1], bl_rx_lenght, HAL_MAX_DELAY);

		switch(bootloader_rx_data[1])
		{
			case BL_GET_VER:
				bootloader_get_ver_cmd(bootloader_rx_data);
				break;

			case BL_GET_HELP:
				bootloader_get_help_cmd(bootloader_rx_data);
				break;

			case BL_GET_CID:
				bootloader_get_cid_cmd(bootloader_rx_data);

			case BL_GET_RDP_STATUS:
				bootloader_get_rdpStatus_cmd(bootloader_rx_data);

			default:
				break;
		}
	}
}
void bootloader_jump_userApp(void)
{
	void(*bootloader_app_reset_p)(void);
	print_m("\nBootLoader: called bootloader jump_userApp\n");

	uint32_t mspValue = *(volatile uint32_t*)FLASH_SECTOR2_BASE_ADRR;
	__set_MSP(mspValue);

	uint32_t resetValue = *(volatile uint32_t*)(FLASH_SECTOR2_BASE_ADRR + 4);
	bootloader_app_reset_p = (void*)resetValue;

	bootloader_app_reset_p();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		if(uart_data == 0x0D)
		{
			if(!strcmp(Buffer, "boot"))
			{
				print_m("\nBOOT_APP\n\n");

				bootloader_uart_data_read();

			}
			else if(!strcmp(Buffer, "user"))
			{
				print_m("\nUser_APP\n\n");
				bootloader_jump_userApp();
			}
			index_Buffer = 0;
			memset(Buffer, 0, 20);
		}
		else
		{
			Buffer[index_Buffer] = uart_data;
			index_Buffer++;
		}
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart_data, 1);

	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
