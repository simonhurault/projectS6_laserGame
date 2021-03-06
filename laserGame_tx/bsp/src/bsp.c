/*
 * bsp.c
 *
 *  Created on: 4 mars 2020
 *      Author: simon.hurault
 */


/*
 * BSP_TIMER_PWM_Init()
 * TIM1 as Output Compare PWM mode
 * Channel 1 -> PA8 (AF2)
 * Channel 2 -> PA9 (AF2)
 */

#include "stm32f0xx.h"

void BSP_TIMER_PWM_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA8 and PA9 as Alternate Function
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
	GPIOA->MODER |=  (0x02 <<GPIO_MODER_MODER8_Pos) | (0x02 <<GPIO_MODER_MODER9_Pos);

	// Set PA8 and PA9 to AF2 (TIM1)
	GPIOA->AFR[1] &= ~(0x000000FF);
	GPIOA->AFR[1] |=  (0x00000022);

	// Enable TIM1 clock
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;

	// Reset TIM1 configuration
	TIM1->CR1  = 0x0000;
	TIM1->CR2  = 0x0000;
	TIM1->CCER = 0x0000;

	// Set TIM1 prescaler
	// Fck = 48MHz -> /48 = 1MHz counting frequency (1�s resolution)
	TIM1->PSC = (uint16_t) 48 -1;

	// Set Auto-Reload to period = 11ms
	TIM1->ARR = (uint16_t) 11000;

	// Enable Auto-Reload Preload register
	TIM1->CR1 |= TIM_CR1_ARPE;

	// Setup Input Capture
	TIM1->CCMR1 = 0x0000;
	TIM1->CCMR2 = 0x0000;

	// Setup PWM mode 1 output
	TIM1->CCMR1 |= (0x06 <<TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
	TIM1->CCMR1 |= (0x06 <<TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;

	// Set default PWM values
	TIM1->CCR1 = 1500;
	TIM1->CCR2 = 1500;

	// Enable Outputs
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	// Enable Main output
	TIM1->BDTR |= TIM_BDTR_MOE;

	// Enable TIM1
	TIM1->CR1 |= TIM_CR1_CEN;
}
