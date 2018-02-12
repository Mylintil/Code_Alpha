//msp430-gcc  -mmcu=msp430g2553 msp.c
//msp debug _ prog a.out
//mspdebug rf2500
//prog a.out
#include <msp430.h>
#include "UART.h"
/*int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0b01000000;					// Set P1.0 to output direction

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0b01000000;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}

	return 0;
}*/

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop WDT
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TACTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode
  CCR0 =  10000;                     // CCR0=(f-2)/1.49
  //CCR0 = 1;
  P1OUT = 0x00;               // Shut down everything
  P1DIR &= 0x00;
  P1DIR |= BIT0 + BIT6 + BIT1;            // P1.0 and P1.6 pins output the rest are input
  P1REN |= BIT3;                   // Enable internal pull-up/down resistors
  P1OUT |= BIT3;                   //Select pull-up mode for P1.3
  P1IE |= BIT3;                       // P1.3 interrupt enabled
  P1IES |= BIT3;                     // P1.3 Hi/lo edge
  P1IFG &= ~BIT3;                  // P1.3 IFG cleared
  _BIS_SR(CPUOFF + GIE);          // Enter LPM0 w/ interrupt
  __enable_interrupt();
  UART_start();
  while(1)                          //Loop forever, we work with interrupts!
  {}
  return 0;
}

// Timer A0 interrupt service routine
//#pragma vector=TIMERA0_VECTOR
//__interrupt void Timer_A(void)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
		//P1OUT ^= BIT1;
	UART_emitt(255);
    // Port 1 interrupt service routine
                             // Toggle P1.0
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
/*
   P1OUT ^= BIT0;
   P1OUT ^= BIT6;// Toggle P1.6
   P1IFG &= ~BIT3; */                    // P1.3 IFG cleared
}


void UART_start(void){
        UART_PORT_DIR |= TX + RX;
}

void UART_emitt(void){//To call in the timer routine



  if(word_emitt!=0){
    parity =  word_emitt&1;//take first bit of the word_emitt;
    if(cpt==0){//START
      UART_PORT_OUT &= ~TX;//START BIT
    }
    if (cpt>=1 && cpt<=8){//word_emitt
      if((word_emitt&(1<<(cpt-1))) > 0){
        UART_PORT_OUT |= TX;
      }
      else{
        UART_PORT_OUT &= ~TX;
      }
    }
    if(cpt==9){//Parity BIT
      UART_PORT_OUT &= ~TX;
    }
    if(cpt==10){
      UART_PORT_OUT |= TX;//STOP BIT
    }
    cpt++;
    if (cpt>=11){//FIN
      parity = 0;
      cpt=0;
      word_emitt=0;
    }
  }

}

char UART_receive(void){
  if((UART_PORT_OUT&RX>0) && (!receive_start)){
    receive_start=1;
    word_receive = 0;
    receive_cpt=0;
  }
  if(receive_start){
    if(UART_PORT_OUT&RX>0){
      word_receive |= 1<<receive_cpt;
      receive_cpt++;
    }
  }
  if(receive_cpt>=11){
    receive_start=0;
    word_receive=word_receive>1;
    word_receive &= 255;
    receive_cpt=0;
  }
  return word_receive;
}
