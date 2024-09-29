#include"lcd.h"
#include"Ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void){
	SREG |= (1<<7);  //Enable Global Interrupt I-Bit
	LCD_init();      //intialize lcd driver
	Ultrasonic_init();  //intialize ultrasonic driver
	LCD_displayString(" Distance=    cm");
    while(1){
		uint16 distance =Ultrasonic_readDistance();
		/* dispaly disatnce on lcd*/
		LCD_moveCursor(0,11);
		LCD_intgerToString(distance);

		if(distance<100){
		LCD_displayString(" ");
		}
		else if(distance<10){
			LCD_displayString("  ");
		}
}
}
