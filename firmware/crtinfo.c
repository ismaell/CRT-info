#include <at89x51.h> // Definitions of registers, SFRs and Bits

// ISR Prototypes ===================================================
void INT0_ISR(void)	__interrupt 0; // ISR for External __interrupt 0
void T0_ISR(void)	__interrupt 1; // ISR for Timer0/Counter0 Overflow
void INT1_ISR(void)	__interrupt 2; // ISR for External __interrupt 1
void T1_ISR(void)	__interrupt 3; // ISR for Timer1/Counter1 Overflow
void UART_ISR(void)	__interrupt 4; // ISR for UART __interrupt

#define XTAL_11_0592MHz 11059200
#define BAUDRATE 28800
void setup_uart1(){
	// 8 bit, baud=28800, 1 stop-bit, no parity

	PCON &= 0x7F; // PCON.7 = 0
	TMOD = 0x20;
	SCON = 0x50;
	TH1  = 256 - ((XTAL_11_0592MHz / 384) / BAUDRATE); //This is H1=255 :-)
	TL1  = 0x00;
	TR1  = 1;

}

void main(void)
{
	setup_uart1();

	while(1){ };
}

void INT0_ISR(void)	__interrupt 0
{}
void T0_ISR(void)	__interrupt 1
{}
void INT1_ISR(void)	__interrupt 2
{}
void T1_ISR(void)	__interrupt 3
{}
void UART_ISR(void)	__interrupt 4
{}

