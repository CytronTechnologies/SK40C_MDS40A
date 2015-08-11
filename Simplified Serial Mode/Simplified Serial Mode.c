/********************************************************************************
*
* SMARTDRIVE40 SAMPLE CODE (Simplified Serial Mode) by MPLAB IDE V8.85
* Using PIC16F877A
* Compilable with compiler V9.81,V9.82,V9.83
*
* DESCRIPTION:
*
* This is the sample source code to control the SmartDrive40 with PIC16F877A.
* In this example, SW1 and SW2 are used to control the motor:
* SW1	- Rotate clockwise in full speed.
* SW2	- Rotate counter-clockwise in full speed.
* No switch pressed - motor stop rotate
*
* DIP SWITCH SETTINGS: 00100011
* 
*******************************************************************************/

// Include neccessary header files
#include <htc.h>	// include htc for HI-TECH header file for PIC

/*******************************************************************************
* DEVICE CONFIGURATION WORDS                                                   *
*******************************************************************************/

//if Hi-Tech V9.70-V9.80 compiler is being used, open this device configuration words
/*__CONFIG(HS &			// High Speed Crystal.
		 WDTDIS &		// Disable Watchdog Timer.
		 PWRTEN &		// Enable Power Up Timer.
		 BORDIS &		// Disable Brown Out Reset.
		 LVPDIS);		// Disable Low Voltage Programming.
*/

//if Hi-Tech V9.81-V9.83 compiler is being used, open this device configuration words
__CONFIG(FOSC_HS &		// High Speed Crystal.
		 WDTE_OFF &		// Disable Watchdog Timer.
		 PWRTE_ON &		// Enable Power Up Timer.
		 BOREN_OFF &	// Disable Brown Out Reset.
		 LVP_OFF);		// Disable Low Voltage Programming.

/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES                                                  *
*******************************************************************************/

void uart_initialize(void);
void uart_transmit(unsigned char uc_data);


/*******************************************************************************
* PRIVATE CONSTANT DEFINITION                                                  *
*******************************************************************************/

// Oscillator Frequency.
#define	_XTAL_FREQ		20000000

// UART baud rate
#define UART_BAUD		9600

// I/O Connection.
#define SW1				RB0
#define SW2				RB1

#define TX				RC6		// connected to both MDS40A IN1
#define MDS40A_L_IN2	RC0
#define MDS40A_R_IN2	RC1


/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
	// Set all pins on Port A as digital I/O.
	ADCON1 = 0b00000110;

	// Initiate value for output pin
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	// Initialize the I/Os.
	TRISB = 0b00000011;
	TRISC = 0;	

	uart_initialize();
	// The Slave Select (IN2) for MDS40A set HIGH 
	MDS40A_L_IN2 = 1;
	MDS40A_R_IN2 = 1;
	
	// Simplified Serial Mode.
	// The DIP switch must be set to 00100011.
	while (1) {
		// If switch 1 is pressed, forward motor at full speed.
		if (SW1 == 0) {
			uart_transmit(255);
		}	
		
		// If switch 2 is pressed, reverse motor at full speed.
		else if (SW2 == 0) {
			uart_transmit(0);
		}	
		
		// If no switch is pressed, stop the motor.
		else {
			uart_transmit(127);
		}	
	}
	// Infinate loop at the end of program, prevent the PIC from reset
	while(1) continue;			
}

/*******************************************************************************
* PUBLIC FUNCTION: uart_initialize
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the UART transmit module.
*
*******************************************************************************/
void uart_initialize(void)
{
	BRGH = 1;									// Select high speed baud rate.
	SPBRG = (_XTAL_FREQ / 16 / UART_BAUD) - 1;	// Configure the baud rate.
	SPEN = 1;									// Enable serial port.
	TXEN = 1;									// Enable transmission.
}

/*******************************************************************************
* PUBLIC FUNCTION: uart_transmit
*
* PARAMETERS:
* ~ uc_data		- The data that we want to transmit.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* This function will transmit one byte of data using UART. This is a blocking
* function, if the transmit buffer is full, we will wait until the
* data in the buffer has been sent out before we move in the new data.
*
*******************************************************************************/
void uart_transmit(unsigned char uc_data)
{
	// Wait until the transmit buffer is ready for new data.
	while (TXIF == 0);
	
	// Transmit the data.
	TXREG = uc_data;
}	

