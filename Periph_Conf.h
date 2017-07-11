#ifndef __PERIPH_CONF_H
#define __PERIPH_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define ADC1_DR_Address 0x4001204C
#define ADC2_DR_Address 0x4001214C
#define ADC3_DR_Address 0x4001224C

#define SPI1_DR_Address 0x4001300C

#define Buffer_to_Send_Lenght 256

uint16_t Buffer_to_Send_ADC1[Buffer_to_Send_Lenght];
uint16_t Buffer_to_Send_ADC2[Buffer_to_Send_Lenght];
uint16_t Buffer_to_Send_ADC3[Buffer_to_Send_Lenght];

//Буфферы данных для тестирования для тестирования
uint16_t Buffer_to_Send_TestData1[Buffer_to_Send_Lenght];
uint16_t Buffer_to_Send_TestData2[Buffer_to_Send_Lenght];
uint16_t Buffer_to_Send_TestData3[Buffer_to_Send_Lenght];

//ADC
void Init_ADCs_with_DMA(void);
void Init_EXT_for_ADCs(void);
void Init_EXTI_DMA(void);
//SPI
void Init_SPI1(void);
void Init_DMA_for_SPI(void);
void Send_Data_To_SPI1(uint32_t *pRAM);
//Test
void Init_Leds(void);
void Delay(uint32_t count);


#ifdef __cplusplus
}
#endif

#endif
