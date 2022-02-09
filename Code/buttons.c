#include "MKL05Z4.h" 
#include "buttons.h"
#include "lcd1602.h"
#include <stdio.h>

/* Buttons connected to port A */
typedef enum{COLUMN_4 = 9, COLUMN_3 = 10, COLUMN_2 = 11, COLUMN_1 = 12, ROW_4 = 5, ROW_3 = 6 } ButtonType;

void delay_ms( int n) {
	volatile int i;
	volatile int j;
	for( i = 0 ; i < n; i++)
	for(j = 0; j < 3500; j++) {}
}


void buttonsInit(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Enable clock for GPIO A */
	PORTA->PCR[ROW_3] |= PORT_PCR_MUX(1);
	PORTA->PCR[ROW_4] |= PORT_PCR_MUX(1);
	PORTA->PCR[COLUMN_1] |= PORT_PCR_MUX(1);
	PORTA->PCR[COLUMN_2] |= PORT_PCR_MUX(1);
	PORTA->PCR[COLUMN_3] |= PORT_PCR_MUX(1);
	PORTA->PCR[COLUMN_4] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[COLUMN_1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //pull-up resistors on inputs
	PORTA->PCR[COLUMN_2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[COLUMN_3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[COLUMN_4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	
	SysTick_Config(SystemCoreClock / 100);
}

int WhichButton(void){

	int button_press = 0;
		for(uint8_t i = 0; i < 2 ; i++){ //rows loop
			PTA->PDDR |= (1<<(5 + i)); //set port as output
			PTA->PCOR |= (1<<(5 + i)); //set output as 0
			delay_ms(1);
			for(uint8_t j = 1; j < 5 ; j++) { //columns loop
				if( ( PTA->PDIR & (1<<(j + 8)) ) == 0 ){ 
					button_press = j + 4 * i;
					while( ( PTA->PDIR & (1<<(j + 8)) ) == 0 )
						delay_ms(20);
					return button_press;
				}
			} //end of collumns loop
			PTA->PDDR ^= (1<<(5 + i)); //set port as input
		}	//end of rows loop
		
		return 0;
}

void PrintButton(int button){
	__disable_irq();
	char tab[8] = "0";
	sprintf(tab, "%u", button);
	
	LCD1602_SetCursor(9,0);
	LCD1602_Print("S  ");
	LCD1602_SetCursor(10,0);
	LCD1602_Print(tab);
	__enable_irq();
}


