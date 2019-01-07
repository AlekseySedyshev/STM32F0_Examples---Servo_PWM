#include "stm32f0xx.h"        // Device header

#define LED_ON()  GPIOB->BRR  |=1
#define LED_OFF() GPIOB->BSRR |=1

void delay_ms (uint16_t value)
{
    TIM17->ARR = value;
    TIM17->CNT = 0;
    TIM17->CR1 = TIM_CR1_CEN;
    while((TIM17->SR & TIM_SR_UIF)==0){}
		TIM17->SR &= ~TIM_SR_UIF;
}

void initial (void)
{
	/*timer config*/
  RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    						//HSI 8 MHz - 1 msek
  TIM17->PSC = 8000-1;
  TIM17->CR1 |= TIM_CR1_OPM;                          //One time run.
	
	/*GPIOB PIN 0 - Alternative mode */ 
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN; 								//
	GPIOB->MODER |= GPIO_MODER_MODER0_0; 								// Output mode
	
	
	GPIOB->MODER |= GPIO_MODER_MODER1_1; 								// Pb1 - alternative
	GPIOB->AFR[0] |= 0x01 << GPIO_AFRL_AFRL1_Pos;  			// Pb1 - AFR1 (TIM3_CH4)
	
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 128-1;  																		// 8000000 / 128 = 62500 Hz
	TIM3->ARR = 1250; 																			// 62500 /1260 = 50Hz (20 mS Period)  
	
	TIM3->CCR4 = 36;  																			// 544 us 
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
	TIM3->CCER |= TIM_CCER_CC4E; 
	
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM3->EGR |= TIM_EGR_UG;
	} 

int main(void)
{
initial();

//-----------------------------initial data----------------------------------

while (1)  /* Main loop */
{
TIM3->CCR4 = 36;                   // PWM - 544 uS, 20mS Period
LED_OFF();				
delay_ms(2000);
TIM3->CCR4 = 160;                 // PWM - 2400 uS, 20mS Period
LED_ON();				
delay_ms(2000);
	
} // end - main loop 
} // end - Main  
	
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ while (1)  {  } }
#endif
