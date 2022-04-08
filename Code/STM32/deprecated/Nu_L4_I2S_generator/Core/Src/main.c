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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UINT24_MAX	16777215
#define SAMPLE_RATE	48000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int32_t sine_1k[] =
{
	8388608,9483540,10559739,11598789,12582911,13495268,14320249,15043737,
	15653355,16138670,16491380,16705449,16777215,16705449,16491380,16138670,
	15653355,15043737,14320249,13495268,12582911,11598789,10559739,9483540,
	8388608,7293675,6217476,5178426,4194304,3281947,2456966,1733478,
	1123860,638545,285835,71766,0,71766,285835,638545,
	1123860,1733478,2456966,3281947,4194304,5178426,6217476,7293675
};

int32_t sine_100[] =
{
	4194304,4249205,4304098,4358971,4413816,4468624,4523385,4578089,
	4632728,4687291,4741770,4796155,4850437,4904607,4958655,5012572,
	5066348,5119976,5173444,5226745,5279869,5332807,5385550,5438089,
	5490415,5542518,5594391,5646023,5697407,5748534,5799394,5849979,
	5900280,5950290,5999998,6049397,6098477,6147232,6195652,6243729,
	6291455,6338822,6385821,6432445,6478685,6524534,6569983,6615026,
	6659653,6703858,6747634,6790971,6833864,6876305,6918286,6959800,
	7000840,7041400,7081472,7121048,7160124,7198691,7236744,7274275,
	7311278,7347748,7383677,7419059,7453890,7488161,7521868,7555005,
	7587566,7619546,7650939,7681739,7711942,7741543,7770535,7798915,
	7826677,7853817,7880329,7906210,7931455,7956060,7980020,8003332,
	8025990,8047993,8069335,8090013,8110023,8129363,8148028,8166016,
	8183323,8199947,8215884,8231133,8245690,8259552,8272718,8285185,
	8296951,8308015,8318373,8328025,8336968,8345202,8352724,8359534,
	8365630,8371012,8375677,8379627,8382859,8385373,8387170,8388248,
	8388607,8388248,8387170,8385373,8382859,8379627,8375677,8371012,
	8365630,8359534,8352724,8345202,8336968,8328025,8318373,8308015,
	8296951,8285185,8272718,8259552,8245690,8231133,8215884,8199947,
	8183323,8166016,8148028,8129363,8110023,8090013,8069335,8047993,
	8025990,8003332,7980020,7956060,7931455,7906210,7880329,7853817,
	7826677,7798915,7770535,7741543,7711942,7681739,7650939,7619546,
	7587566,7555005,7521868,7488161,7453890,7419059,7383677,7347748,
	7311278,7274275,7236744,7198691,7160124,7121048,7081472,7041400,
	7000840,6959800,6918286,6876305,6833864,6790971,6747634,6703858,
	6659653,6615026,6569983,6524534,6478685,6432445,6385821,6338822,
	6291455,6243729,6195652,6147232,6098477,6049397,5999998,5950290,
	5900280,5849979,5799394,5748534,5697407,5646023,5594391,5542518,
	5490415,5438089,5385550,5332807,5279869,5226745,5173444,5119976,
	5066348,5012572,4958655,4904607,4850437,4796155,4741770,4687291,
	4632728,4578089,4523385,4468624,4413816,4358971,4304098,4249205,
	4194304,4139402,4084509,4029636,3974791,3919983,3865222,3810518,
	3755879,3701316,3646837,3592452,3538170,3484000,3429952,3376035,
	3322259,3268631,3215163,3161862,3108738,3055800,3003057,2950518,
	2898192,2846089,2794216,2742584,2691200,2640073,2589213,2538628,
	2488327,2438317,2388609,2339210,2290130,2241375,2192955,2144878,
	2097152,2049785,2002786,1956162,1909922,1864073,1818624,1773581,
	1728954,1684749,1640973,1597636,1554743,1512302,1470321,1428807,
	1387767,1347207,1307135,1267559,1228483,1189916,1151863,1114332,
	1077329,1040859,1004930,969548,934717,900446,866739,833602,
	801041,769061,737668,706868,676665,647064,618072,589692,
	561930,534790,508278,482397,457152,432547,408587,385275,
	362617,340614,319272,298594,278584,259244,240579,222591,
	205284,188660,172723,157474,142917,129055,115889,103422,
	91656,80592,70234,60582,51639,43405,35883,29073,
	22977,17595,12930,8980,5748,3234,1437,359,
	0,359,1437,3234,5748,8980,12930,17595,
	22977,29073,35883,43405,51639,60582,70234,80592,
	91656,103422,115889,129055,142917,157474,172723,188660,
	205284,222591,240579,259244,278584,298594,319272,340614,
	362617,385275,408587,432547,457152,482397,508278,534790,
	561930,589692,618072,647064,676665,706868,737668,769061,
	801041,833602,866739,900446,934717,969548,1004930,1040859,
	1077329,1114332,1151863,1189916,1228483,1267559,1307135,1347207,
	1387767,1428807,1470321,1512302,1554743,1597636,1640973,1684749,
	1728954,1773581,1818624,1864073,1909922,1956162,2002786,2049785,
	2097152,2144878,2192955,2241375,2290130,2339210,2388609,2438317,
	2488327,2538628,2589213,2640073,2691200,2742584,2794216,2846089,
	2898192,2950518,3003057,3055800,3108738,3161862,3215163,3268631,
	3322259,3376035,3429952,3484000,3538170,3592452,3646837,3701316,
	3755879,3810518,3865222,3919983,3974791,4029636,4084509,4139402
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SAI1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SAI1_Init();
  /* USER CODE BEGIN 2 */

    uint16_t size = sizeof(sine_1k) / sizeof(uint32_t);
	uint16_t freq = SAMPLE_RATE / size;

	for(uint16_t i = 0; i < size; i++)
	{
		sine_1k[i] = sine_1k[i] - (UINT24_MAX / 2) - 1;
	}

	HAL_StatusTypeDef stat = HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t*)sine_1k, size);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODESLAVE_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_MONOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_24BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{

}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{

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
