/**
 *
 * Copyright (c) 2024 HT Micron Semicondutores S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "HT_Timer_Demo.h"

/* Function prototypes  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_Timer_Init(void)
 * \brief Initialize Timer
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Timer_Init(int seconds);

/*!******************************************************************
 * \fn static void HT_Timer0_ISR(void)
 * \brief Timer0 Interrupt Service Routine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Timer0_ISR(void);

/* ---------------------------------------------------------------------------------------*/

volatile uint8_t timerInterrupt = 0;
volatile uint32_t irqn_cnt = 0;

static void HT_Timer0_ISR(void) {
    timerInterrupt = 1;
}

static void HT_Timer_Init(int seconds) {

    timer_config_t timerConfig;

    // Config TIMER clock, source from 32.768KHz and divide by 1
    CLOCK_SetClockSrc(GPR_TIMER0FuncClk, GPR_TIMER0ClkSel_32K);
    CLOCK_SetClockDiv(GPR_TIMER0FuncClk, 1);

    TIMER_DriverInit();

    // Config timer period as 1s, match0 value is 32768 = 0x8000
    TIMER_GetDefaultConfig(&timerConfig);
    timerConfig.reloadOption = TIMER_ReloadOnMatch0;
    timerConfig.match0 = 0x8000*seconds;  //8000 = esta em hexadecimal

    TIMER_Init(0, &timerConfig);

    // Note interrupt flag won't assert in TIMER_InterruptPulse mode
    TIMER_InterruptConfig(0, TIMER_Match0Select, TIMER_InterruptPulse);
    TIMER_InterruptConfig(0, TIMER_Match1Select, TIMER_InterruptDisabled);
    TIMER_InterruptConfig(0, TIMER_Match2Select, TIMER_InterruptDisabled);

    // Enable IRQ
    XIC_SetVector(PXIC_Timer0_IRQn, HT_Timer0_ISR);   //PXIC_Timer0_IRQn É PRE-DEFINIDO. HT_Timer0_ISR = FUNÇÃO 
    XIC_EnableIRQ(PXIC_Timer0_IRQn);
}

void HT_Timer_App(void) {
    ht_printf("Timer Start!\n");

    HT_Timer_Init(TIMER_N_SECONDS); // 1s
    TIMER_Start(TIMER_INSTANCE);  

    while(1) {
      
      if(timerInterrupt) {
        timerInterrupt = 0;
        ht_printf("IRQn: %d\n", ++irqn_cnt);

        TIMER_Start(0);
      }
    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
