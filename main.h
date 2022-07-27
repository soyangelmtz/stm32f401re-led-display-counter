#include "main.h"
#include "stm32f4xx.h"
#include <stdlib.h>

#define	GPIOD_CLOCK		(1<<3)
#define	GPIOC_CLOCK		(1<<2)

int D1=0x00;
int D2=0x10;
int D3=0x20;
int D4=0x30;
int D5=0x40;
int D6=0x50;
int D7=0x60;
int D8=0x70;
int counter;

int adcValue;


int temp;

RTC_TimeTypeDef sTime1;
RTC_DateTypeDef sDate1;

ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
void displayNumber(int numero); // escritura del digito
void setDisplay(int dig1, int dig2, int dig3, int dig4, int dig5, int dig6, int dig7, int dig8); // se genera el barrido
int numeros[10]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};
void SysInit(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
int Horas;
int Minutos;
int Segundos;
int horaActual;
int Tiempo;

void LeerADC();
void EscribirElTiempo();
void LeerElTiempo();

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
	RCC->AHB1ENR |= GPIOD_CLOCK;
	GPIOD->MODER |= 0x5555;
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
  MX_ADC1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
	  LeerADC();
		EscribirElTiempo();
	  LeerElTiempo();
	  displayNumber(Tiempo);
		
  }
}

    void displayNumber(int numero)
{
	
	int dig1,dig2,dig3,dig4,dig5,dig6,dig7,dig8;
		dig1=numero%10;						//decenas de millones
		dig2=(numero%100)/10;;			//millones
		dig3=(numero%1000)/100;		//centenas de miles
		dig4=(numero%10000)/1000;	//decenas de miles
		dig5=(numero%100000)/10000;	//miles
		dig6=(numero%1000000)/100000;	//centenas
		dig7=(numero%10000000)/1000000;//decenas
		dig8=(numero%100000000)/10000000;//Unidades
		setDisplay(dig1,dig2,dig3,dig4,dig5,dig6,dig7,dig8);
	
}

void setDisplay(int dig1, int dig2, int dig3, int dig4, int dig5, int dig6, int dig7, int dig8)
{
	
	GPIOD->ODR=numeros[dig1]+D1;
	HAL_Delay(1);
	GPIOD->ODR=numeros[dig2]+D2;
	HAL_Delay(1);
	//GPIOD->ODR=numeros[dig3]+D3; se apaga el digito 3 para apreciarlo en los displays
	//HAL_Delay(1);
	GPIOD->ODR=numeros[dig4]+D4;
	HAL_Delay(1);
	GPIOD->ODR=numeros[dig5]+D5;
	HAL_Delay(1);
	//GPIOD->ODR=numeros[dig6]+D6; "" "" "" se apaga el digito 6
	//HAL_Delay(1);
	GPIOD->ODR=numeros[dig7]+D7;
	HAL_Delay(1);
	GPIOD->ODR=numeros[dig8]+D8;
	HAL_Delay(1);
	
}

void EscribirElTiempo(void)
{
	switch(counter)
	  {
			
	  	  case 1:
			  sTime1.Seconds = (adcValue*60/4096); // se obtienen los segundos 
			  HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			  break;
	  	  case 2:
	  		  sTime1.Minutes = (adcValue*60/4096); // se obtienen los minutos 
			  HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			  break;
	 	  case 3:
			  sTime1.Hours = (adcValue*24/4096); // se obtienen las horas
			  HAL_RTC_SetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
			  break;
	 	  default:
	  		  break;
			
		}
	}
	  void LeerElTiempo(void) {
				
	  		HAL_RTC_GetTime(&hrtc, &sTime1, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate1, RTC_FORMAT_BIN);

	
				Horas = sTime1.Hours; 
				Minutos = sTime1.Minutes; 
				Segundos = sTime1.Seconds; 
				
				Horas = Horas * 1000000; Minutos = Minutos * 1000; 
				Tiempo=Horas+Minutos+Segundos;
			
				
	    }

void LeerADC(void)
	
{
	HAL_ADC_Start(&hadc1); // ACTIVA EL ADC
     		  	  if(HAL_ADC_PollForConversion(&hadc1,5) == HAL_OK) // Se realiza la conversión del ADC
		  	  {
						
		  		  adcValue = HAL_ADC_GetValue(&hadc1); // Obtiene el valor del ADC y lo guarda en la variable adcValue
						
		  	  }
		  	  HAL_ADC_Stop (&hadc1); // se detiene el ADC
}


					

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 1;
  sDate.Year = 21;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 
                           PD4 PD5 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
