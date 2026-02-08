/**
 * @file debug_test.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグ関連
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef DBG_TEST_H
#define DBG_TEST_H

#define TEST_NG     (-1)
#define TEST_OK     (0)

// --------------------------------
// [コンパイル]
// #define DMA_TEST
// --------------------------------
void dbg_test_init(void);
void dbg_test_main(void);

#endif // DBG_TEST_H