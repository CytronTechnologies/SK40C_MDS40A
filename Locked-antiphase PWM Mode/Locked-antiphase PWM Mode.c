/********************************************************************************
*
* SMARTDRIVE40 SAMPLE CODE (Locked Antiphase PWM Mode) by MPLAB IDE V8.85
* Using PIC16F877A
* Compilable with compiler V9.81,V9.82,V9.83
*
* DESCRIPTION:
*
* This is the sample source code to control the SmartDrive40 with PIC16F877A.
* In this example, SW1 and SW2 are used to control the motor:
* SW1	- Forward in full speed.
* SW2	- Reverse in full speed.
* No switch pressed - motor stop rotate
*
* DIP SWITCH SETTINGS: 10010000
*
*******************************************************************************/

#include <htc.h>

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
void pwm_initialize(void);

/*******************************************************************************
* PRIVATE CONSTANT DEFINITION                                                  *
*******************************************************************************/

// Oscillator Frequency.
#define	_XTAL_FREQ		20000000

// I/O Connection.
#define SW1				RB0
#define SW2				RB1

//#define MDS40A_L_IN1	RC2
//#define MDS40A_L_IN2	RC3
//#define MDS40A_R_IN1	RC1
//#define MDS40A_R_IN2	RC0
#define SPEED1			CCPR1L
#define SPEED2			CCPR2L

/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
	// Set all pins on Port A as digital IO.
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
	
	// Initialize PWM
	pwm_initialize();				
	
	// PWM Mode (Locked-Antiphase).
	// The DIP switch must be set to 10010000.
	while (1) {
		// If switch 1 is pressed, forward motor at full speed.
		if (SW1 == 0) {
			SPEED1 = 255;
			SPEED2 = 255;
		}	
		
		// If switch 2 is pressed, reverse motor at full speed.
		else if (SW2 == 0) {
			SPEED1 = 0;
			SPEED2 = 0;
		}	
		
		// If no switch is pressed, stop the motor.
		else {
			SPEED1 = 127;
			SPEED2 = 127;
		}	
	}	
	// Infinate loop at the end of program, prevent the PIC from reset
	while(1) continue;		
}

/*******************************************************************************
* PUBLIC FUNCTION: pwm_initialize
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the CCP1 module to operate in PWM mode.
*
*******************************************************************************/
void pwm_initialize(void)
{
	// Setting PWM frequency = 4.88khz
	// using PIC16F877A with 20MHz Crystal
	PR2 = 0xFF;
	T2CKPS1 = 0;
	T2CKPS0 = 1;	// Timer 2 prescale = 4.
	
	// Stop the motor in locked-antiphase mode.
	SPEED1 = 127;		
	SPEED2 = 127;	// Duty cycle = 50%;
	TMR2ON = 1;		// Turn on Timer 2.
	
	// Configure CCP1 module to operate in PWM mode.
	CCP1M3 = 1;
	CCP1M2 = 1;	
	// Configure CCP2 module to operate in PWM mode.	
	CCP2M3 = 1;
	CCP2M2 = 1;		
}