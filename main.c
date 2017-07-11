#include "stm32f4xx.h"
#include "main.h"

int main(void)
{
	//Для теста
	Fill_Test_Buffers();

	SystemInit();Init_ADCs_with_DMA();Init_EXT_for_ADCs();
	Init_EXTI_DMA();Init_SPI1();Init_DMA_for_SPI();

    while(1)
    {
    }
}

/* Обработчик прерывания для DMA с ADC1
 *
 */
void DMA2_Stream4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream4, DMA_FLAG_HTIF4))
	{
		//Send_Data_To_SPI1(&Buffer_to_Send_ADC1);
		//Send_Data_To_SPI1(&Buffer_to_Send_ADC2);
		//Send_Data_To_SPI1(&Buffer_to_Send_ADC3);
		Send_Data_To_SPI1(&Buffer_to_Send_TestData1);
		Send_Data_To_SPI1(&Buffer_to_Send_TestData2);
		Send_Data_To_SPI1(&Buffer_to_Send_TestData3);
		DMA_ClearITPendingBit(DMA2_Stream4, DMA_IT_HTIF4);
	}else{
		Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_TestData1 + Buffer_to_Send_Lenght/2));
		Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_TestData2 + Buffer_to_Send_Lenght/2));
		Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_TestData3 + Buffer_to_Send_Lenght/2));
		//Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_ADC1 + Buffer_to_Send_Lenght/2));
		//Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_ADC2 + Buffer_to_Send_Lenght/2));
		//Send_Data_To_SPI1((uint32_t)((uint32_t)&Buffer_to_Send_ADC3 + Buffer_to_Send_Lenght/2));
		DMA_ClearITPendingBit(DMA2_Stream4, DMA_IT_TCIF4);
	}

}

void EXTI15_10_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line11);
}
