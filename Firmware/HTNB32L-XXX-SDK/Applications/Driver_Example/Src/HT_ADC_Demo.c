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

#include "HT_ADC_Demo.h"

static volatile uint32_t callback = 0;
static volatile uint32_t user_adc_channel = 0;
static volatile uint32_t vbat_result = 0;
static volatile uint32_t thermal_result = 0;

static adc_config_t adcConfig, vbatConfig, thermalConfig;

/*!******************************************************************
 * \fn static void HT_ADC_ConversionCallback(uint32_t result)
 * \brief ADC convertion callback. Function that will be called 
 * after the ADC finishes its conversion.
 *
 * \param[in]  uint32_t result              ADC conversion output.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
static void HT_ADC_ConversionCallback(uint32_t result);

/*!******************************************************************
 * \fn static void HT_ADC_VbatConversionCallback(uint32_t result)
 * \brief ADC VBAT conversion callback. Function that will be called 
 * after the ADC finishes to convert VBAT.
 *
 * \param[in]  uint32_t result              ADC conversion output.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
static void HT_ADC_VbatConversionCallback(uint32_t result);

/*!******************************************************************
 * \fn static void HT_ADC_ThermalChannelCallback(uint32_t result)
 * \brief ADC Thermal conversion callback. Function that will be called 
 * after the ADC finishes to convert the Thermal channel.
 *
 * \param[in]  uint32_t result              ADC conversion output.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
static void HT_ADC_ThermalChannelCallback(uint32_t result);

/*!******************************************************************
 * \fn static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value)
 * \brief Calculates the voltage from the respective AD value.
 *
 * \param[in]  uint32_t ad_value              ADC output value.
 * \param[out] none
 *
 * \retval Voltage value calculated.
 *******************************************************************/
static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value);

/*!******************************************************************
 * \fn static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value)
 * \brief Calculates the temperature from the respective AD value.
 *
 * \param[in]  uint32_t ad_value              ADC output value.
 * \param[out] none
 *
 * \retval Voltage value calculated.
 *******************************************************************/
static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value);

/*!******************************************************************
 * \fn static void HT_ADC_InitVbat(void)
 * \brief Initialize VBAT ADC channel.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_ADC_InitVbat(void);

/*!******************************************************************
 * \fn static void HT_ADC_Init(uint8_t channel)
 * \brief Initialize ADC user channel.
 *
 * \param[in]  uint8_t channel              ADC channel that will be used.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_ADC_Init(uint8_t channel);

/*!******************************************************************
 * \fn static void HT_ADC_InitThermal(void)
 * \brief Initialize ADC thermal channel.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_ADC_InitThermal(void);

static void HT_ADC_ConversionCallback(uint32_t result) {
    callback |= DEMO_ADC_CHANNEL;
    user_adc_channel = result;
}

static void HT_ADC_VbatConversionCallback(uint32_t result) {
    callback |= ADC_ChannelVbat;
    vbat_result = result;
}

static void HT_ADC_ThermalChannelCallback(uint32_t result) {
    callback |= ADC_ChannelThermal;
    thermal_result = result;
}

static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value) {
    uint32_t value;
    value = HAL_ADC_CalibrateRawCode(ad_value);
    return (uint32_t)(value*16/3);
}

static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value) {
    return (int)HAL_ADC_ConvertThermalRawCodeToTemperature(ad_value);
}

static void HT_ADC_InitVbat(void) {
    ADC_GetDefaultConfig(&vbatConfig);

    vbatConfig.channelConfig.vbatResDiv = ADC_VbatResDivRatio3Over16;
    ADC_ChannelInit(ADC_ChannelVbat, ADC_UserAPP, &vbatConfig, HT_ADC_VbatConversionCallback);
}

//copiar para MQTT EXAMPLE
static void HT_ADC_Init(uint8_t channel) { 
    ADC_GetDefaultConfig(&adcConfig);

    adcConfig.channelConfig.aioResDiv = ADC_AioResDivRatio3Over16; 

    ADC_ChannelInit(channel, ADC_UserAPP, &adcConfig, HT_ADC_ConversionCallback);
}
//ate aqui

static void HT_ADC_InitThermal(void) {
    ADC_GetDefaultConfig(&thermalConfig);

    thermalConfig.channelConfig.thermalInput = ADC_ThermalInputVbat;

    HT_ADC_ChannelInit(ADC_ChannelThermal, ADC_UserAPP, &thermalConfig, HT_ADC_ThermalChannelCallback);
}

void HT_ADC_App(void) {
    ht_printf("ADC example start!\n");

    HT_ADC_InitVbat();
    HT_ADC_Init(DEMO_ADC_CHANNEL);
    HT_ADC_InitThermal();

    while(1) {
        callback = 0;

        HT_ADC_StartConversion(DEMO_ADC_CHANNEL, ADC_UserAPP);
        HT_ADC_StartConversion(ADC_ChannelVbat, ADC_UserAPP);
        HT_ADC_StartConversion(ADC_ChannelThermal, ADC_UserAPP);

        while(callback != (DEMO_ADC_CHANNEL | ADC_ChannelVbat | ADC_ChannelThermal));

        ht_printf("\nThermal: %d\n", (int)HT_ADC_GetTemperatureValue(thermal_result));
        ht_printf("Vbat: %dmv\n", (int)HT_ADC_GetVoltageValue(vbat_result));
        ht_printf("ADC Value: %dmv\n", (int)HT_ADC_GetVoltageValue(user_adc_channel));
    
        delay_us(1000000);
    }
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
