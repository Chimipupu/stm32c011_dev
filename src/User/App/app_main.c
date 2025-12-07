/**
 * @file app_main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "app_main.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "dma.h"
#include "main.h"
#include "usart.h"
#include <stdarg.h>

#ifdef DMA_TEST

#define TEST_NG     (-1)
#define TEST_OK     (0)

#ifdef DMA_TEST
static uint32_t s_dma_src_buf[8];
static uint32_t s_dma_dst_buf[8];
static volatile bool s_dma_transfer_done = false;
static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma);
static int8_t dma_test(void);
#endif // DMA_TEST

static void test_main(void);

/**
 * @brief UART経由でprintf()相当の出力
 */
void DBG_UART_PRINTF(const char *format, ...)
{
#ifdef DEBUG_UART_USE
    char buffer[256];
    va_list args;
    int len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    for (int i = 0; i < len && i < sizeof(buffer); i++) {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, (uint8_t)buffer[i]);
    }
#endif
}

#if 0
void uart_tx_data(uint8_t *p_buf, uint32_t len)
{
#ifdef DEBUG_UART_USE
    uint8_t *p_ptr = p_buf;
    uint32_t i;

    for(i = 0; i < len; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, *p_ptr);
        p_ptr++;
    }
#endif
}
#endif

static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma)
{
    s_dma_transfer_done = true;
}

/**
 * @brief DMAテスト
 * 
 * @return int8_t   0:正常終了
                    -1:異常終了
 */
static int8_t dma_test(void)
{
    int8_t ret = TEST_NG;
    int8_t verify;
    const char test_str[] = "DMA TEST";
    const uint8_t str_len = 8;
    const uint32_t word_count = (uint32_t)((str_len + 3) / 4);

    memset(s_dma_src_buf, 0, sizeof(s_dma_src_buf));
    memcpy(s_dma_src_buf, test_str, str_len);
    memset(s_dma_dst_buf, 0, sizeof(s_dma_dst_buf));
    s_dma_transfer_done = false;

    HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1,
                            (uint32_t)s_dma_src_buf,
                            (uint32_t)s_dma_dst_buf,
                            word_count);

    // Verify
    DBG_UART_PRINTF("[DEBUG] DMA Src: %s\r\n", &s_dma_src_buf[0]);
    DBG_UART_PRINTF("[DEBUG] DMA Dst: %s\r\n", &s_dma_dst_buf[0]);
    verify = memcmp((const void *)s_dma_src_buf, (const void *)s_dma_dst_buf, str_len);
    ret = (verify == 0) ? TEST_OK : TEST_NG;

    return ret;
}

static void test_main(void)
{
    int8_t ret;
    static bool is_tested = false;

    if (is_tested != true) {
        DBG_UART_PRINTF("[DEBUG] DMA Test: START\r\n");
        ret = dma_test();
        if (ret == TEST_OK) {
            DBG_UART_PRINTF("[DEBUG] DMA Test: PASSED\r\n");
            is_tested = true;
        } else {
            DBG_UART_PRINTF("[DEBUG] DMA Test: FAILED\r\n");
            is_tested = false;
        }
    }
}
#endif // DMA_TEST

void app_main_init(void)
{
#ifdef DMA_TEST
    memset(s_dma_src_buf, 0, sizeof(s_dma_src_buf));
    memset(s_dma_dst_buf, 0, sizeof(s_dma_dst_buf));

    HAL_DMA_RegisterCallback(&hdma_memtomem_dma1_channel1,
                        HAL_DMA_XFER_CPLT_CB_ID,
                        dma_transfer_complete_cb
                        );
#endif // DMA_TEST
}

void app_main(void)
{
#if 0
    DBG_UART_PRINTF("App Main\r\n");
#endif

    /* USART1からコマンド受信時の処理 */
    if (usart1_is_cmd_ready()) {
        uint8_t cmd_buffer[256];
        usart1_get_cmd(cmd_buffer, sizeof(cmd_buffer));
        DBG_UART_PRINTF("[CMD] %s\r\n", cmd_buffer);
    }

#ifdef DMA_TEST
    test_main();
#endif // DMA_TEST
}