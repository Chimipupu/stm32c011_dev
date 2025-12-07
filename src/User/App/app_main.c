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

// --------------------------------
// C Std library
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

// ST SDK
#include "usart.h"
#include "main.h"

// My App
#ifdef DEBUG_TEST
#include "debug_test.h"
#endif // DEBUG_TEST

// --------------------------------
#ifdef DEBUG_UART_USE
typedef void (*p_cbk)(uint8_t *p_arg);

typedef struct {
    char *p_cmd_str;
    p_cbk p_callback;
} dbg_cmd_t;

static void cmd_dbg(uint8_t *p_arg);
static void cmd_reg(uint8_t *p_arg);

const dbg_cmd_t g_dbg_cmd_tbl[] = {
    { "dbg", cmd_dbg },
    { "reg", cmd_reg },
};

const uint8_t g_dbg_cmd_tbl_size = sizeof(g_dbg_cmd_tbl) / sizeof(dbg_cmd_t);

uint8_t s_cmd_buf[256];

static void dbg_cmd_exec(uint8_t *p_buf);
// --------------------------------
static void cmd_dbg(uint8_t *p_arg)
{
    DBG_UART_PRINTF("[DEBUG] DBG Command\r\n");
    // TODO:
}

static void cmd_reg(uint8_t *p_arg)
{
    DBG_UART_PRINTF("[DEBUG] REG Command\r\n");
    // TODO:
}

static void dbg_cmd_exec(uint8_t *p_buf)
{
    uint8_t i;
    char *p_cmd;
    char *p_arg;

    if (p_buf == NULL || *p_buf == '\0') {
        return;
    }

    p_cmd = (char *)p_buf;
    p_arg = strchr(p_cmd, ' ');

    if (p_arg != NULL) {
        *p_arg = '\0';
        p_arg++;

        while (*p_arg == ' ') {
            p_arg++;
        }
    } else {
        p_arg = NULL;
    }

    // テーブルと照合
    for(i = 0; i < g_dbg_cmd_tbl_size; i++)
    {
        if (strcmp(p_cmd, (char *)g_dbg_cmd_tbl[i].p_cmd_str) == 0)
        {
            DBG_UART_PRINTF("[DEBUG] cmd: %s\r\n", p_cmd);
            DBG_UART_PRINTF("[DEBUG] arg: %s\r\n", p_arg ? p_arg : "None");

            // コマンド実行
            if(p_arg == NULL) {
                g_dbg_cmd_tbl[i].p_callback(NULL);
            } else {
                g_dbg_cmd_tbl[i].p_callback((uint8_t *)p_arg);
            }
            return;
        }
    }
}
#endif // DEBUG_UART_USE

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

void app_main_init(void)
{
#ifdef DEBUG_TEST
    dbg_test_init();
#endif // DEBUG_TEST
}

void app_main(void)
{
    // DBG_UART_PRINTF("App Main\r\n");

#ifdef DEBUG_TEST
    dbg_test_main();
#endif // DEBUG_TEST

#ifdef DEBUG_UART_USE
    /* USART1からコマンド受信時の処理 */
    if (usart1_is_cmd_ready()) {
        memset(&s_cmd_buf[0], 0, sizeof(s_cmd_buf));
        usart1_get_cmd(s_cmd_buf, sizeof(s_cmd_buf));
        dbg_cmd_exec(s_cmd_buf);
    }
#endif // DEBUG_UART_USE
}