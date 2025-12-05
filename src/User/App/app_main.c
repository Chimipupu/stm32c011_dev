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
#include "dma.h"
#include "main.h"

#ifdef DEBUG_TEST

#define TEST_NG     (-1)
#define TEST_OK     (0)

static uint32_t s_dma_src_buf[8];
static uint32_t s_dma_dst_buf[8];
static volatile bool s_dma_transfer_done = false;

static void dma_transfer_complete_cb(DMA_HandleTypeDef *p_hdma);
static int8_t dma_test(void);
static void test_main(void);

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
    verify = memcmp((const void *)s_dma_src_buf, (const void *)s_dma_dst_buf, str_len);
    ret = (verify == 0) ? TEST_OK : TEST_NG;

    return ret;
}

static void test_main(void)
{
    int8_t ret;
    static bool is_tested = false;

    if (is_tested != true) {
        ret = dma_test();
        if (ret == TEST_OK) {
            is_tested = true;
        } else {
            is_tested = false;
        }
    }
}
#endif // DEBUG_TEST

void app_main_init(void)
{
#ifdef DEBUG_TEST
    memset(s_dma_src_buf, 0, sizeof(s_dma_src_buf));
    memset(s_dma_dst_buf, 0, sizeof(s_dma_dst_buf));

    HAL_DMA_RegisterCallback(&hdma_memtomem_dma1_channel1,
                        HAL_DMA_XFER_CPLT_CB_ID,
                        dma_transfer_complete_cb
                        );
#endif
}

void app_main(void)
{
#ifdef DEBUG_TEST
    test_main();
#endif // DEBUG_TEST
}