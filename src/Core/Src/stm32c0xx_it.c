/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32c0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32c0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USART1受信バッファ */
#define USART1_RX_BUFFER_SIZE    256
static volatile uint8_t s_uart_rx_buf[USART1_RX_BUFFER_SIZE];
static volatile uint16_t s_usart1_rx_head = 0;
static volatile uint16_t s_usart1_rx_tail = 0;
static volatile bool s_uart_cmd_flg = false;

/**
 * @brief USART1から1文字受信
 * @return 受信データ（バッファが空の場合は-1）
 */
int16_t usart1_getchar(void)
{
    if (s_usart1_rx_head == s_usart1_rx_tail) {
        return -1;  // バッファ空
    }

    uint8_t data = s_uart_rx_buf[s_usart1_rx_tail];
    s_usart1_rx_tail = (s_usart1_rx_tail + 1) % USART1_RX_BUFFER_SIZE;
    return (int16_t)data;
}

/**
 * @brief USART1受信バッファに格納されたデータ数
 */
uint16_t usart1_available(void)
{
    if (s_usart1_rx_head >= s_usart1_rx_tail) {
        return s_usart1_rx_head - s_usart1_rx_tail;
    } else {
        return USART1_RX_BUFFER_SIZE - s_usart1_rx_tail + s_usart1_rx_head;
    }
}

/**
 * @brief USART1受信コマンドフラグを取得
 */
bool usart1_is_cmd_ready(void)
{
    return s_uart_cmd_flg;
}

/**
 * @brief USART1受信コマンドを取得してバッファをクリア
 */
void usart1_get_cmd(uint8_t *buf, uint16_t buf_size)
{
    uint16_t len = 0;
    uint16_t pos = s_usart1_rx_tail;

    /* コマンドバッファ内のデータを抽出 */
    while (pos != s_usart1_rx_head && len < buf_size - 1) {
        uint8_t ch = s_uart_rx_buf[pos];
        if (ch == '\r' || ch == '\n') {
            break;
        }
        buf[len++] = ch;
        pos = (pos + 1) % USART1_RX_BUFFER_SIZE;
    }
    buf[len] = '\0';

    /* テールポインタをデリミタの次に移動 */
    if (pos != s_usart1_rx_head) {
        s_usart1_rx_tail = (pos + 1) % USART1_RX_BUFFER_SIZE;
    }

    s_uart_cmd_flg = false;
}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32C0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32c0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_memtomem_dma1_channel1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */

  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */

  /* USER CODE END TIM17_IRQn 0 */
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}

/**
  * @brief This function handles I2C1 interrupt (combined with EXTI 23).
  */
void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  /* USER CODE END I2C1_IRQn 0 */
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}

/**
  * @brief This function handles USART1 interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  /* 受信データ有効フラグをチェック */
  if (LL_USART_IsActiveFlag_RXNE(USART1)) {
      uint8_t data = LL_USART_ReceiveData8(USART1);
      uint16_t next_head = (s_usart1_rx_head + 1) % USART1_RX_BUFFER_SIZE;

      /* バッファオーバーフロー防止 */
      if (next_head != s_usart1_rx_tail) {
          s_uart_rx_buf[s_usart1_rx_head] = data;
          s_usart1_rx_head = next_head;

          /* デリミタ（\rか\n）を検出したらコマンド受信完了フラグをセット */
          if (data == '\r' || data == '\n') {
              s_uart_cmd_flg = true;
          }
      }
  }

  /* オーバーラン、フレーミングエラー等をクリア */
  if (LL_USART_IsActiveFlag_ORE(USART1)) {
      LL_USART_ClearFlag_ORE(USART1);
  }
  if (LL_USART_IsActiveFlag_FE(USART1)) {
      LL_USART_ClearFlag_FE(USART1);
  }
  if (LL_USART_IsActiveFlag_PE(USART1)) {
      LL_USART_ClearFlag_PE(USART1);
  }

  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
