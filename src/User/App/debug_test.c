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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "dma.h"

#include "app_main.h"

// --------------------------------
typedef int8_t (*p_func)(void);

typedef struct {
    char *p_test_name;
    p_func p_test_func;
} dbg_test_t;

static uint32_t s_dma_src_buf[8];
static uint32_t s_dma_dst_buf[8];
static volatile bool s_dma_transfer_done = false;

static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma);
static int8_t dma_test(void);

const dbg_test_t g_dbg_test_tbl[] = {
    { "DMA", dma_test },
    { NULL, NULL }
};
const uint8_t g_dbg_test_tbl_size = sizeof(g_dbg_test_tbl) / sizeof(dbg_test_t);

// --------------------------------
static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma)
{
    s_dma_transfer_done = true;
}

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

void dbg_test_init(void)
{
    memset(s_dma_src_buf, 0, sizeof(s_dma_src_buf));
    memset(s_dma_dst_buf, 0, sizeof(s_dma_dst_buf));

    HAL_DMA_RegisterCallback(&hdma_memtomem_dma1_channel1,
                        HAL_DMA_XFER_CPLT_CB_ID,
                        dma_transfer_complete_cb
                        );
}

void dbg_test_main(void)
{
    int8_t ret;
    static uint8_t s_test = 0;
    static bool is_tested = false;

    if (is_tested != true) {
        if (g_dbg_test_tbl[s_test].p_test_name != NULL) {
            DBG_UART_PRINTF("[DEBUG] %s Test: Start\r\n", g_dbg_test_tbl[s_test].p_test_name);
            ret = g_dbg_test_tbl[s_test].p_test_func();
            if (ret == TEST_OK) {
                DBG_UART_PRINTF("[DEBUG] Test Result: OK\r\n");
            } else {
                DBG_UART_PRINTF("[DEBUG] Test Result: NG\r\n");
            }
            s_test++;
        } else {
            is_tested = true;
            DBG_UART_PRINTF("[DEBUG] All Tests Completed\r\n");
        }
    }
}