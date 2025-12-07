/**
 * @file debug_test.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグ関連
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "debug_test.h"

// --------------------------------
// C Std library
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// ST SDK
#include "dma.h"

// My App
#include "app_main.h"
#include "stm32c011xx.h"

// --------------------------------
#define LOOPS           10
#define TOTAL_DIGITS    (LOOPS * 4)
#define PI_BUF_SIZE     ((TOTAL_DIGITS * 10) / 3 + 2)
// --------------------------------
typedef int8_t (*p_func)(void);

typedef struct {
    char *p_test_name;
    p_func p_test_func;
} dbg_test_t;

#ifdef DMA_TEST
static uint32_t s_dma_src_buf[8];
static uint32_t s_dma_dst_buf[8];
static volatile bool s_dma_transfer_done = false;

static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma);
static int8_t dma_test(void);
#endif // DMA_TEST

static int8_t math_test(void);

const dbg_test_t g_dbg_test_tbl[] = {
#ifdef DMA_TEST
    { "DMA", dma_test },
#endif // DMA_TEST
    { "Math", math_test },
    { NULL, NULL }
};
const uint8_t g_dbg_test_tbl_size = sizeof(g_dbg_test_tbl) / sizeof(dbg_test_t);

static void calc_pi_spigot(void);
// --------------------------------
/**
 * @brief 円周率の計算(スピゴット・アルゴリズム)
 * @note 整数のみで円周率の各桁を順番に計算
 */
static void calc_pi_spigot(void)
{
    int pi[PI_BUF_SIZE];
    int i, k;
    int b, d;
    int c = 0;

    for (i = 0; i < PI_BUF_SIZE; i++)
    {
        pi[i] = 2000;
    }

    for (k = 0; k < LOOPS; k++)
    {
        d = 0;
        i = PI_BUF_SIZE - 1;

        while (i >= 0)
        {
            d += pi[i] * 10000;
            b = 2 * i + 1;
            pi[i] = d % b;
            d /= b;
            if (i > 0) {
                d *= i;
            }
            i--;
            // ------------------
        }

        int val = c + d / 10000;
        if (k == 0) {
            DBG_UART_PRINTF("%d.%03d", val / 1000, val % 1000);
        } else {
            DBG_UART_PRINTF("%04d", val);
        }
        c = d % 10000;
    }
    DBG_UART_PRINTF("...\r\n");
}

static int8_t math_test(void)
{
    // 円周率を計算(スピゴット・アルゴリズム)
    // NOTE: LOOPS = 10だと「3.141592653589793238462643383279502884197...」と表示されるはず
    calc_pi_spigot();
    return TEST_OK;
}

#ifdef DMA_TEST
static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma)
{
    s_dma_transfer_done = true;
}

static int8_t dma_test(void)
{
    int8_t ret = TEST_NG;
    int8_t verify;
    const char test_stpi[] = "DMA TEST";
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
#endif // DMA_TEST

void dbg_test_init(void)
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

void dbg_test_main(void)
{
    int8_t ret;
    static uint8_t s_test = 0;
    static bool is_test_end = false;

    if (is_test_end != true) {
        if (g_dbg_test_tbl[s_test].p_test_name != NULL) {
            DBG_UART_PRINTF("[DEBUG] %s Test: Start\r\n", g_dbg_test_tbl[s_test].p_test_name);
            ret = g_dbg_test_tbl[s_test].p_test_func();
            DBG_UART_PRINTF("[DEBUG] Test Result: %s\r\n", (ret == TEST_OK) ? "OK" : "NG");
            s_test++;
        } else {
            is_test_end = true;
        }
    }
}