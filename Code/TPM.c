#include "TPM.h"
void TPM_Init(void)
{
	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK);				//Dołączenie sygnału zegara do TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);		//źródło taktowania TPMx MCGFLLCLK=47972352Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;					//	TPM0 w trybie "up counting"
	TPM0->SC |= TPM_SC_PS(0);								
	
	TPM0->CONTROLS[1].CnSC = 0x0000;
	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK;
	TPM0->CONTROLS[1].CnV = 0x0000;
	
	TPM0->SC |= TPM_SC_CMOD(1);							//	Wlacz licznik TPM0
}

