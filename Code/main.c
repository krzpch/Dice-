/******************************************************************************
 * This file is a part of the TMp Project.                                   *
 ******************************************************************************/

/**
 * @file main.c
 * @author Krzysztof Polchlopek
 * @date Dec 2020 
 * @brief File containing the main function. 
 */
					
#include "MKL05Z4.h"
#include "TPM.h"
#include "ADC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "buttons.h"
#include "PCG.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t	dice[5] = {0,0,0,0,0};
uint8_t selected_dices[5] = {0,0,0,0,0};
uint8_t selected_place = 0;
uint8_t rolls_left = 0;
uint64_t random;
volatile int button = 0;
volatile int is_ready = 1;
char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

void SysTick_Handler(void);
void SysTick_Handler(void) {
	if (is_ready == 1)
		{
		button = WhichButton();
		is_ready = 0;
		}
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}

int main(void)
{
	
	LCD1602_Init();
	LCD1602_Backlight(TRUE);
	TPM_Init();
	pcg32_fast_init(random);
	buttonsInit();
	LCD1602_SetCursor(0,0);
	LCD1602_Print("PRESS S4");
	LCD1602_SetCursor(0,1);
	LCD1602_Print("TO START");
	
	while(1)
	{
		if (is_ready == 0) {
			if(button == 1) { // shift curosr left
				if (rolls_left > 0 ) {
					selected_place -= 1;
					if (selected_place > 4)
						selected_place = 4;
					LCD1602_SetCursor(2*selected_place,1);
				}
			} // end of if(button == 1)
			
			if(button == 2) { // shift curosr right
				if (rolls_left > 0 ) {
					selected_place += 1;
					if (selected_place > 4)
						selected_place = 0;
					LCD1602_SetCursor(2*selected_place,1);
				}
			} // end of if(button == 2)
			
			if(button == 5) { // select dices to rethrow
				if (rolls_left > 0 ) {
					LCD1602_SetCursor(2*selected_place,1);
					selected_dices[selected_place] ^= 1;
					if (selected_dices[selected_place] == 1)
						LCD1602_Print("X");
					else
						LCD1602_Print(" ");
					LCD1602_SetCursor(2*selected_place,1);
				}
			} // end of if(button == 5)
			
			if(button == 6) { // rethrow selected dices
				if(rolls_left > 0 ) {
					int check = 0;
					for(int i = 0;i < 5; i++) // throw dices
					{
						if(selected_dices[i] == 1){
							dice[i] = fast_pcg32() % 6 + 1;
							
							selected_dices[i] = 0;
							LCD1602_SetCursor(2*i,1);
							LCD1602_Print(" ");
							
							sprintf(display,"%u",dice[i]);
							LCD1602_SetCursor(2*i,0);
							LCD1602_Print(display);
							
							selected_place = 0;
							LCD1602_SetCursor(selected_place,1);
							check ++;
						} // end of if(selected_dices[i] == 1)
					}
					if(check > 0)
						rolls_left -= 1;
				} // end of if(rolls_left > 0 )
				if(rolls_left == 0 )
				{
					LCD1602_Blink_Off();
					DELAY(300);
					LCD1602_SetCursor(0,1);
					LCD1602_Print("No rolls left");
				} // end of if(rolls_left == 0 )
			} // end of if(button == 6)
			
			if(button == 4){ // throw all dices
				LCD1602_ClearAll();
				LCD1602_Blink_Off();
				selected_place = 0; // set cursor position to dice 1
				random = TPM0->CNT; // seed for PCG generator
				random = (random << 48)+(random << 32)+(random << 16)+(random);
				pcg32_fast_init(random);
				
				for(int i = 0;i < 5; i++) // generate random numbers
				{
					dice[i] = fast_pcg32() % 6 + 1;
					sprintf(display,"%u",dice[i]);
					LCD1602_SetCursor(2*i,0);
					LCD1602_Print(display);
				}
				
				rolls_left = 2;
				LCD1602_Blink_On();
				LCD1602_SetCursor(0,1);
			} // end of if(button == 4)
			
			is_ready = 1;
		} // end of if(is_ready == 0)
		__wfi();
	} //enf of while(1)
}
