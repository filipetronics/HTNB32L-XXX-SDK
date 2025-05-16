/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutores S.A.
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/

/*!
 * \file main.h
 * \brief Main file of HTNB32L-XXX Slpman Example. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves, Christian Roberto Lehmen,  Matheus da Silva Zorzeto, Felipe Kalinski Ferreira,
 *         Leandro Borges, Mauricio Carlotto Ribeiro, Henrique Kuhn, Cleber Haack, Eduardo Mendel
 *         Gleiser Alvarez Arrojo
 * 
 * \link https://github.com/htmicron
 * \version 1.0
 * \date July 31, 2023
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "bsp.h"
#include "HT_BSP_Custom.h"
#include "osasys.h"
#include "ostask.h"
#include "slpman_qcx212.h"
#include "debug_log.h"
#include "plat_config.h"
#include "mw_config.h"
#include "netmgr.h"
#include "ps_event_callback.h"
#include "cmips.h"
#include "ps_lib_api.h"
#include "HT_Peripheral_Config.h"
#include "hal_uart.h"
#include "stdio.h"
#include "htnb32lxxx_hal_usart.h"

/* Defines  ------------------------------------------------------------------*/

/* at cmd task static stack and control block */
#define USRAPP_TASK_STACK_SIZE        (2*1024)

/* AON configuration constants. Please refer to Table 1 available at HT_Peripheral_Config.h file. */
#define AONIO0_INSTANCE_NUM	1
#define AONIO0_GPIO_PIN	4			//GPIO20  20%16=4

/* AON pin enable preprocessor */
#define USE_AON_PIN 0

/* RTC enable preprocessor */
#define USE_DEEP_SLEEP_TIMER 1

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum FSM_State#ifdef USE_AON_PIN
 * \brief Slpman FSM states.
 */
typedef enum {
    EXAMPLE_STEP_1 = 0,         // keep in sleep1 state
    EXAMPLE_STEP_2,
    EXAMPLE_STEP_3,
    EXAMPLE_STEP_4
} FSM_State;

#endif /*__MAIN_H__*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
