/**
 *
 * Copyright (c) 2023 HT Micron Semicondutores S.A.
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

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "slpman_qcx212.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
//#include "HT_ADC_Demo.h"
#include "queue.h" 

QueueHandle_t xFila;

static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                                ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

extern USART_HandleTypeDef huart1;

//GPIO10 - BUTTON
#define BUTTON_INSTANCE          0                  /**</ Button pin instance. */
#define BUTTON_PIN               10                 /**</ Button pin number. */
#define BUTTON_PAD_ID            25                 /**</ Button Pad ID. */
#define BUTTON_PAD_ALT_FUNC      PAD_MuxAlt0        /**</ Button pin alternate function. */

//GPIO3 - LED
#define LED_INSTANCE             0                  /**</ LED pin instance. */
#define LED_GPIO_PIN             3                  /**</ LED pin number. */   //PIN03 = GPIO3
#define LED_PAD_ID               14                 /**</ LED Pad ID. */       //PAD ID 14 = GPIO3
#define LED_PAD_ALT_FUNC         PAD_MuxAlt0        /**</ LED pin alternate function. */

#define LED_ON  1                                   /**</ LED on. */
#define LED_OFF 0                                   /**</ LED off. */

volatile bool button_state = false;

static void HT_GPIO_InitButton(void) {
  GPIO_InitType GPIO_InitStruct = {0};

  GPIO_InitStruct.af = PAD_MuxAlt0;
  GPIO_InitStruct.pad_id = BUTTON_PAD_ID;
  GPIO_InitStruct.gpio_pin = BUTTON_PIN;
  GPIO_InitStruct.pin_direction = GPIO_DirectionInput;
  GPIO_InitStruct.pull = PAD_InternalPullUp;
  GPIO_InitStruct.instance = BUTTON_INSTANCE;
  GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;
  GPIO_InitStruct.interrupt_config = GPIO_InterruptFallingEdge;

  HT_GPIO_Init(&GPIO_InitStruct);
}

static void HT_GPIO_InitLed(void) {
  GPIO_InitType GPIO_InitStruct = {0};

  GPIO_InitStruct.af = PAD_MuxAlt0;
  GPIO_InitStruct.pad_id = LED_PAD_ID;
  GPIO_InitStruct.gpio_pin = LED_GPIO_PIN;
  GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
  GPIO_InitStruct.init_output = 0;
  GPIO_InitStruct.pull = PAD_AutoPull;
  GPIO_InitStruct.instance = LED_INSTANCE;
  GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;

  HT_GPIO_Init(&GPIO_InitStruct);
}

void Task1(void *pvParameters) {
    int valor = 0;
    while (1) {
        //button_state = (bool) HT_GPIO_PinRead(BUTTON_INSTANCE, BUTTON_PIN);
        button_state = HT_GPIO_PinRead(BUTTON_INSTANCE, BUTTON_PIN); //inverte quando pressionado = 0
        valor = button_state;
        xQueueSend(xFila, &valor, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500)); //antes = 50
    }
}

void Task2(void *pvParameters){
    int recebido = 0;
    while (1){   
        if (xQueueReceive(xFila, &recebido, portMAX_DELAY)){
        HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, recebido);
        }
        // HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, button_state);
        // vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
  \fn          int main_entry(void)
  \brief       main entry function.
  \return
*/
void main_entry(void) {
    xFila = xQueueCreate(5,sizeof(int));

    if(xFila == NULL){
        printf("ERRO AO CRIAR FILA. \n");
        while (1){}    
    }

    HT_GPIO_InitButton();
    HT_GPIO_InitLed();
    slpManNormalIOVoltSet(IOVOLT_3_30V);

    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    printf("Exemplo FreeRTOS\n");

    xTaskCreate(Task1, "Blink", 128, NULL, 1, NULL);
    xTaskCreate(Task2, "Print", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    
    printf("Nao deve chegar aqui.\n");

    while(1);

}

/******** HT Micron Semicondutores S.A **END OF FILE*/