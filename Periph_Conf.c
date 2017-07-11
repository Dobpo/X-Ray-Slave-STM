#include "Periph_Conf.h"

/* Инициализация АЦП и DMA.
 * PA5 -> ADC1_Chanel5 -> DMA2_Stream4
 * PA4 -> ADC2_Chanel4 -> DMA2_Stream2
 * PA3 -> ADC3_Chanel3 -> DMA2_Stream1
 */
void Init_ADCs_with_DMA(void){
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 |
							   RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream4 channel0 configuration **************************************/
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Buffer_to_Send_ADC1;
	DMA_InitStructure.DMA_BufferSize = Buffer_to_Send_Lenght;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream4, ENABLE);
	/*DMA2 Stream2 channel1 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC2_DR_Address;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Buffer_to_Send_ADC2;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	/* DMA2 Stream1 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_Address;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Buffer_to_Send_ADC3;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream1, ENABLE);

	/* Configure ADC1 Channel5 | ADC2 Channel4 | ADC3 Channel3 pin as analog input ******************************/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonStructInit(&ADC_CommonInitStructure);
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC2 Init ****************************************************************/
	ADC_Init(ADC2, &ADC_InitStructure);
	/* ADC13 Init ****************************************************************/
	ADC_Init(ADC3, &ADC_InitStructure);

	/*Разрешить прерывание DMA по заполнению буффера*/
	DMA_ITConfig(DMA2_Stream4, DMA_IT_TC | DMA_IT_HT, ENABLE);
	//DMA_ITConfig(DMA2_Stream2, DMA_IT_TC | DMA_IT_HT, ENABLE);
	//DMA_ITConfig(DMA2_Stream1, DMA_IT_TC | DMA_IT_HT, ENABLE);

	/* ADC1 regular channel5 configuration *************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	/* ADC2 regular channel4 configuration *************************************/
	ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);
	/* ADC3 regular channel3 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC1 | ADC2 | ADC3 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMACmd(ADC2, ENABLE);
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC1 | ADC2 | ADC3 */
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
}

/* Инициализация обработчика прерывания для DMA
 * по заполнению буффера данных.
 */
void Init_EXTI_DMA(void)
{   NVIC_InitTypeDef NVIC_InitStruct;

    	/*DMA2_Stream4_IRQn Init ******************************************************/
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
		/*DMA2_Stream2_IRQn Init ******************************************************/
	//NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	//NVIC_Init(&NVIC_InitStruct);
		/*DMA2_Stream1_IRQn Init ******************************************************/
	//NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	//NVIC_Init(&NVIC_InitStruct);

}

/* Инициализация DMA2_Stream3 для передачи данных из RAM в SPI1
 *
 */
void Init_DMA_for_SPI(void)
{
	DMA_InitTypeDef 	DMA_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* Deinitialize DMA Streams */
	DMA_DeInit(DMA2_Stream3);

	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)SPI1_DR_Address;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) & Buffer_to_Send_ADC1;
	DMA_InitStruct.DMA_BufferSize = Buffer_to_Send_Lenght/2;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_Channel = DMA_Channel_3;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_Init(DMA2_Stream3, &DMA_InitStruct);
}

/* Инициализация светодиодов на STM32F4DISCOVERY.
 *
 */
void Init_Leds(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13
			| GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* Програмная задержка для отладки.
 *
 */
void Delay(uint32_t count)
{
	while(count)
	{
		count--;
	}
}

/* Разрешение прерывания для триггов АЦП PE11
 *
 */
void Init_EXT_for_ADCs(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE , EXTI_PinSource11);

	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/* Инициализация SPI
 * SPI1 :
 *   SCK  -> PB3
 *   MISO -> PB4
 *   MOSI -> PB5
 */
void Init_SPI1(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

	GPIO_InitTypeDef 	GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Connect SPI pins to AF_SPI1 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //SCLK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //MOSI
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //MISO

	SPI_InitTypeDef 	SPI_InitStruct;

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;
}

/* Отсылка данных с pRAM в SPI
 * pRAM - указатель на буффер данных
 */
void Send_Data_To_SPI1(uint32_t *pRAM)
{
	DMA2_Stream3->M0AR =pRAM;
	DMA_Cmd(DMA2_Stream3, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	while (DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)==RESET);
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
	DMA_Cmd(DMA2_Stream3, DISABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
}

/*Заполнить данными тест буф.
 * */
void Fill_Test_Buffers(void){
	for(int i = 0; i<Buffer_to_Send_Lenght;i++){
		Buffer_to_Send_TestData1[i] = 0;
		Buffer_to_Send_TestData2[i] = 512;
		Buffer_to_Send_TestData3[i] = 1024;
	}
}
