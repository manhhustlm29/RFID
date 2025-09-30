/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int time6_count = 0;
int data_receive_flag = 0;
char cmdBuffer[100];
char recByte;
int cmdIndex = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct Time{
uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t weekday;
uint8_t day;
uint8_t month;
uint8_t year;
};

typedef struct Time Time;
//

void SetTime(Time timeSet){
	HAL_I2C_Mem_Write(&hi2c3, 0xD0, 0, 1, (uint8_t *)&timeSet, 7, 1000);
}

Time GetTime(){
	Time timeGet;
	HAL_I2C_Mem_Read(&hi2c3, 0xD1, 0, 1, (uint8_t *)&timeGet, 7, 1000);
	return timeGet;
}

void DisplayCardID(uint8_t *CardID){
	char buff[100];
	memset(buff,0,sizeof(buff));
	sprintf(buff,"%02d%02d%02d%02d%02d\n",
		CardID[0],CardID[1],CardID[2],CardID[3],CardID[4]
	);
	SH1106_GotoXY (12,10); // goto 10, 10
	SH1106_Puts(buff, &Font_11x18, 1);
	SH1106_UpdateScreen(); // update screen
}

void Display(char* str){
		SH1106_Clear();
		SH1106_GotoXY (12,10); // goto 10, 10
		SH1106_Puts(str, &Font_11x18, 1);
		SH1106_UpdateScreen(); // update screen
}

void DisplayTime(Time time){
	static char buf[100];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%02d:%02d:%02d-%02d-%02d/%02d/%02d \n",
	time.hour,
	time.min,
	time.sec,
	time.weekday,
	time.day,
	time.month,
	time.year);
	SH1106_GotoXY (12,10); // goto 10, 10
	SH1106_Puts(buf, &Font_11x18, 1);
	SH1106_UpdateScreen(); // update screen
}



void TestDS1307()
{
char buff[30];
Time Set_time, Get_Time;
Set_time.sec = 10;
Set_time.min = 30;
Set_time.hour = 7;
Set_time.day = 9;
Set_time.month = 11;
Set_time.year = 24;
Set_time.weekday = 0;
//write time to DS1307
HAL_I2C_Mem_Write(&hi2c3, 0xD0, 0, 1, (uint8_t *)&Set_time, 7, 1000);
while (1) {
//read time back
HAL_I2C_Mem_Read(&hi2c3, 0xD1, 0, 1, (uint8_t *)&Get_Time, 7, 1000);
sprintf(buff,"%02d:%02d:%02d-%02d-%02d/%02d/%02d \n",
Get_Time.hour,
Get_Time.min,
Get_Time.sec,
Get_Time.weekday,
Get_Time.day,
Get_Time.month,
Get_Time.year);
HAL_UART_Transmit(&huart1, buff, strlen(buff), 1000);
HAL_Delay(500);
}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        if(recByte != 'm'){
        	cmdBuffer[cmdIndex++] = recByte;
        }
        else{
        	cmdBuffer[cmdIndex] = 0;
        	cmdIndex = 0;
        	data_receive_flag = 1;
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&recByte, 1);
    }
}


Time time1;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	char logs[100][100];
	char cardList[100][100];
	int numCard = 0;
	int numLog = 0;

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
  MX_I2C3_Init();
  MX_SPI4_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
//  TestDS1307();                      asdfsdf


//  char buf[100];
//  uint8_t X = 0, Y = 0;
//  SH1106_Init ();
//  sprintf (buf, "%s", "RC522 RFID");
//  SH1106_GotoXY (12,10); // goto 10, 10
//  SH1106_Puts(buf, &Font_11x18, 1);
//  SH1106_UpdateScreen(); // update screen
//  HAL_Delay(1000);

  HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SH1106_Init();

  Time currentTime;
  currentTime.year = 2025;
  currentTime.weekday = 00;
  currentTime.month = 05;
  currentTime.day = 17;
  currentTime.hour = 13;
  currentTime.min = 46;
  currentTime.sec = 0;

  SetTime(currentTime);

  HAL_UART_Receive_IT(&huart1, (uint8_t*)&recByte, 1);
//  TestDS1307();
  TM_MFRC522_Init();

  while (1)
  {

//	  Time timee = GetTime();
//	  char buff[1024] = {0};
//	  sprintf(buff,"%02d:%02d:%02d-%02d-%02d/%02d/%02d \n",
//			  timee.hour,
//			  timee.min,
//			  timee.sec,
//			  timee.weekday,
//			  timee.day,
//			  timee.month,
//			  timee.year);
//	  HAL_UART_Transmit(&huart1, buff, strlen(buff), 1000);
//		time1 = GetTime();
//		DisplayTime(time1);

//	  HAL_Delay(500);


//	  char buf[100] = {0};
//	  uint8_t CardID[5];
//	  HAL_Delay(500);
////	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
//	  if(TM_MFRC522_Check(CardID) == MI_OK){
//		  SH1106_Clear();
////		  sprintf(buf,"%s","Found!\r\n");
////			SH1106_GotoXY (12,10); // goto 10, 10
////			SH1106_Puts(buf, &Font_11x18, 1);
////			SH1106_UpdateScreen(); // update screen
//
//			DisplayCardID(CardID);
//
//	  }
//	  else{
//		  sprintf(buf,"%s","Not Found!\r\n");
//			SH1106_GotoXY (12,10); // goto 10, 10
//			SH1106_Puts(buf, &Font_11x18, 1);
//			SH1106_UpdateScreen(); // update screen
//	  }

//	  SH1106_Clear();
	  uint8_t CardID[5];
	  HAL_Delay(100);
	  if(TM_MFRC522_Check(CardID) == MI_OK){
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
		  char cardID[11] = {0};
		  sprintf(cardID,"%d%d%d%d%d",CardID[0],CardID[1],CardID[2],CardID[3],CardID[4]);
		  int flag = 0;
		  for(int i = 0; i< numCard;i++){
			  if(strcmp(cardList[i], cardID) == 0){ // strcmp??
				  flag = 1;
				  Display("Welcome");
				  char tmp[100] = {0};
				  struct Time time = GetTime();
				 sprintf(tmp,"%s %02d:%02d:%02d-%02d-%02d/%02d/%02d",
				 				  cardID,
				 				  time.hour,
				 				  time.min,
				 				  time.sec,
				 				  time.weekday,
				 				  time.day,
				 				  time.month,
				 				  time.year);
				 		  strcpy(logs[numLog++], tmp);
				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
				  break;
			  }
		  }
		  if(flag == 0){

			  Display("Reject");
		  }



	  }
	  else{
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	  }


	  if(data_receive_flag == 1){
		  if(strncmp(cmdBuffer,"add ",4) == 0){
			  char* cardIDAdd = cmdBuffer + 4;
			  int flag = 0;
			  char buff[100];
			  for(int i = 0; i< numCard;i++){
				  if(strcmp(cardList[i], cardIDAdd) == 0){ // strcmp??
					  flag = 1;
					  memset(buff,0,sizeof(buff));
					  sprintf(buff,"%s","Card ID has existed\n");
					  HAL_UART_Transmit(&huart1, buff, strlen(buff), 1000);
					  break;
				  }
			  }
			  if(flag == 0){
			      strcpy(cardList[numCard++], cardIDAdd);
				  memset(buff,0,sizeof(buff));
				  sprintf(buff,"%s","Add Card ID successfully!\n");
				  HAL_UART_Transmit(&huart1, buff, strlen(buff), 1000);
			  }
		  }
		  else if(strncmp(cmdBuffer,"list log",8) == 0){
			  for(int i = 0; i < numLog; i++){
				  HAL_UART_Transmit(&huart1, logs[i], strlen(logs[i]), 1000);
				  HAL_UART_Transmit(&huart1, "\n", 1, 1000);
			  }
		  }


		  data_receive_flag = 0;
	  }







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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 400000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 89;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 99;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PG13 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
