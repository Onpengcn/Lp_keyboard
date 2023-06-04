/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keymap.h"
#include "usbd_custom_hid_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CE_Pin GPIO_PIN_3
#define CE_GPIO_Port GPIOB
#define PL_Pin GPIO_PIN_4
#define PL_GPIO_Port GPIOB

#define KEY_REPORT_SIZE 16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void DelayUs(uint32_t _us)
{
    for (int i = 0; i < _us; i++)
        for (int j = 0; j < 8; j++)
            __NOP();
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

uint8_t spiBuffer[IO_NUMBER / 8 ];
uint8_t debounceBuffer[IO_NUMBER / 8];
unsigned char hidBuffer[16];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void ApplyDebounceFilter(uint32_t _filterTimeUs);
uint8_t* ScanKeyStates();
void keymap_t_hbuf();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  ScanKeyStates();  // Around 40us use 4MHz SPI clk
	  ApplyDebounceFilter(100);
	  keymap_t_hbuf();  // When Fn pressed use layer-2

	  // Report HID key states
	  USBD_CUSTOM_HID_SendReport_FS(hidBuffer, KEY_REPORT_SIZE);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
uint8_t* ScanKeyStates()
{
    memset(spiBuffer, 0xFF, IO_NUMBER / 8);
    PL_GPIO_Port->BSRR = PL_Pin; // Latch

    hspi1.pRxBuffPtr = (uint8_t*) spiBuffer;
    hspi1.RxXferCount = IO_NUMBER / 8;
    __HAL_SPI_ENABLE(&hspi1);
    while (hspi1.RxXferCount > 0U)
    {
        if (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE))
        {
            /* read the received data */
            (*(uint8_t*) hspi1.pRxBuffPtr) = *(__IO uint8_t*) &(hspi1.Instance->DR);
            hspi1.pRxBuffPtr += sizeof(uint8_t);
            hspi1.RxXferCount--;

        }
    }
    __HAL_SPI_DISABLE(&hspi1);

    PL_GPIO_Port->BRR = PL_Pin; // Sample
    return (void*)0;
}

void ApplyDebounceFilter(uint32_t _filterTimeUs)
{
    memcpy(debounceBuffer, spiBuffer, IO_NUMBER / 8);

    DelayUs(_filterTimeUs);
    ScanKeyStates();

    uint8_t mask;
    for (int i = 0; i < IO_NUMBER / 8; i++)
    {
        mask = debounceBuffer[i] ^ spiBuffer[i];
        spiBuffer[i] |= mask;
    }
}

void keymap_t_hbuf()
{
	int16_t index, bitIndex;

	    memset(debounceBuffer, 0, IO_NUMBER / 8 + 1);
	    //for (int16_t i = 0; i < IO_NUMBER / 8; i++)
		for (int16_t i = 0; i < 2; i++)
	    {
	        for (int16_t j = 0; j < 8; j++)
	        {
	            index = (int16_t) (leapmotor_keymap[0][i * 8 + j] / 8);
	            bitIndex = (int16_t) (leapmotor_keymap[0][i * 8 + j] % 8);
	            if ( ! (spiBuffer[index] & (0x80 >> bitIndex))) // accrouding real key pressed to find
	                debounceBuffer[i] |= 0x80 >> j;
//	            if (spiBuffer[index] & (0x80 >> bitIndex)) // accrouding real key pressed to find
//	                debounceBuffer[i] |= 0x80 >> j;
	        }
//	        debounceBuffer[i] = ~debounceBuffer[i];
	    }

	    memset(hidBuffer, 0, KEY_REPORT_SIZE);
	    //for (int i = 0; i < IO_NUMBER / 8; i++)
		for (int i = 0; i < 2; i++)
	    {
	        for (int j = 0; j < 8; j++)
	        {
	            index = (int16_t) (leapmotor_keymap[1][i * 8 + j] / 8 + 1); // +1 for modifier
	            bitIndex = (int16_t) (leapmotor_keymap[1][i * 8 + j] % 8);
	            if (bitIndex < 0)
	            {
	                index -= 1;
	                bitIndex += 8;
	            } else if (index > 100)
	                continue;

	            if (debounceBuffer[i] & (0x80 >> j))
	                hidBuffer[index] |= 1 << (bitIndex); // +1 for Report-ID

	        }
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
