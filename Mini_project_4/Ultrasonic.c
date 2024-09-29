#include "Ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>

uint8 g_edgeCount = 0;   //edge counter
uint16 g_timeHigh = 0;   //pulse width time


void Ultrasonic_init(void){
	ICU_ConfigType ICU_Configurations={F_CPU_8,RAISING}; //configure ICU with FCPU/8 and detect rising edge
	ICU_init(&ICU_Configurations);   //send configurations to ICU_init function
	ICU_setCallBack(Ultrasonic_edgeProcessing);   //set callback function
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT); // set up trigger pin of Ultrasonic as output pin
	//GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_INPUT);  // set up echo pin of Ultrasonic as input pin
}

void Ultrasonic_Trigger(void){
	GPIO_writePin(PORTB_ID,PIN5_ID,1); //send pulse to trigger pin
	_delay_us(10); //ultrasonic sensors need at least 10us to process
	GPIO_writePin(PORTB_ID,PIN5_ID,0); //reset trigger pin
}

uint16 Ultrasonic_readDistance(void){
	uint16 distance=0;
	Ultrasonic_Trigger(); //call trigger function to send pulse
	distance=(g_timeHigh/58)+1; //calculate distance and the (+1) is to remove error factor
	return distance;
}

void Ultrasonic_edgeProcessing(void){
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		ICU_clearTimerValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = ICU_getInputCaptureValue();
		/* Detect rising edge */
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount=0;  //reset g_edgeCount
	}

}



