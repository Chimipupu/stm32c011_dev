/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメインのヘッダ
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

// --------------------------------
// [コンパイルスイッチ]
// NOTE: UARTでのprintf()用　※ただこれはROMを7KBも食う大食い
#define DBG_USE_UART
// NOTE: テスト関連
#define DBG_TEST
// --------------------------------
void DBG_UART_PRINTF(const char *format, ...);
extern void app_main_init(void);
extern void app_main(void);

#endif // APP_MAIN_H