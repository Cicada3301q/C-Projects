//Author: Quenten Welch
//Student number: 30054505
//date: march 26th 2020


//Description: this program displays 2 LED patterns across 3 LED outputs (pins 17, 22, 27), the led pattern displayed 
//as well as the speed transitions in the display are controlled by 2 inputs, pins 23 and 34

// Include files
#include "uart.h"
#include "gpio.h"

// Function prototypes
void init_GPIO17_to_output();
void set_GPIO17();
void clear_GPIO17();
void init_GPIO22_to_output();
void set_GPIO22();
void clear_GPIO22();
void init_GPIO27_to_output();
void set_GPIO27();
void clear_GPIO27();

void init_GPIO23_to_input();
unsigned int get_GPIO23();

void init_GPIO24_to_input();
unsigned int get_GPIO24();

void pattern_1();
void pattern_2();
    
  
////////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function initializes GPIO pins, 17, 22, 27 to output pins
//                  without a pull-up or pull-down resistor. It then turns
//                  the output pin on and off (1 and 0) in an infinite loop.
//
////////////////////////////////////////////////////////////////////////////////

  void main()
{
    register unsigned int r;


    // Set up the UART serial port
    uart_init();
    
    // Set up GPIO pin #17 for output
    init_GPIO17_to_output();
	
	// Set up GPIO pin #22 for output
    init_GPIO22_to_output();
	
	// Set up GPIO pin #27 for output
    init_GPIO27_to_output();
	
	// Set up GPIO pin #23 for input
    init_GPIO23_to_input();
	
	// Set up GPIO pin #24 for input
    init_GPIO24_to_input();
    
    // Print out a message to the console
    uart_puts("Blinking LED Program starting.\n");***** TODO
    
    // Loop forever, switching between pattern 1 and 2 upon checking if button a has been pressed
	while (1) {
       pattern_1();
	   pattern_2();
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       patten_1
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    LEDs(outputpins 17, 27, 22) are illuminated one after the other
//					in the pattern:  1, 2, 3, 1, 2, 3, etc. 
//					Each will be on for about 0.5 second
//
////////////////////////////////////////////////////////////////////////////////
void patten_1(){
	register unsigned int r;
	register unsigned int a;
	register unsigned int b;
	//infinite while loop 
	while(1){
		set_GPIO17()
		
		//check if b was pressed and determine blink speed
		b = get_GPIO24();
		if(b == 0x1){
		r = 0x165a0bc0} //0.25s
		else{
		r = 2CB41780; //0.5s
		}
		
		//busy loop to hold pattern for 0.5 or 0.25 seconds
		while(r--){
			asm volatile("nop");
			}
		clear_GPIO17()
		
		//check if button A has been pressed and switch pattern iff yes
			a = get_GPIO24();
			if(a == 0x0){
			break;
			}
			
		//check if b was pressed and determine blink speed
		b = get_GPIO24();
		if(b == 0x1){
		r = 0x165a0bc0} //0.25s
		else{
		r = 2CB41780; //0.5s
		}
		set_GPIO27()
		
		
		//busy loop to hold pattern for 0.5 or 0.25 seconds
		while(r--){
			asm volatile("nop");
			}
		clear_GPIO27()
		
		//check if button A has been pressed and switch pattern iff yes
		a = get_GPIO24();
			if(a == 0x0){
			break;
			}
			
		//check if b was pressed and determine blink speed
		b = get_GPIO24();
		if(b == 0x1){
		r = 0x165a0bc0} //0.25s
		else{
		r = 2CB41780; //0.5s
		}
		set_GPIO22()
		//busy loop to hold pattern for 0.5 or 0.25 seconds
		while(r--){
			asm volatile("nop");
			}
		clear_GPIO22()
		
		//check if button A has been pressed and switch pattern iff yes
		a = get_GPIO24();
			if(a == 0x0){
			break;
			}
		
	
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       pattern_2
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    LEDs(outputpins 17, 27, 22) will alternate between the states
//					where LED's 1 and 3 are on but 2 is on, and vice versa.
//
////////////////////////////////////////////////////////////////////////////////
void patten_2(){
	register unsigned int r;
	register unsigned int a;
	register unsigned int b;
	//infinite while loop 
	while(1){
		set_GPIO17()
		set_GPIO22()
		
		//check if b was pressed and determine blink speed
		b = get_GPIO24();
		if(b == 0x1){
		r = 0x165a0bc0} //0.25s
		else{
		r = 2CB41780; //0.5s
		}
		//busy loop to hold pattern for 0.5 or 0.25 seconds
		while(r--){
			asm volatile("nop");
			}
		clear_GPIO17()
		clear_GPIO22()
		
		//check if button A has been pressed and switch pattern iff yes
		a = get_GPIO24();
			if(a == 0x0){
			break;}

		set_GPIO27()
		
		//check if b was pressed and determine blink speed
		b = get_GPIO24();
		if(b == 0x1){
		r = 0x165a0bc0} //0.25s
		else{
		r = 2CB41780; 0.5s
		}
		
		
		//busy loop to hold pattern for 0.5 or 0.25 seconds
		while(r--){
			asm volatile("nop");
			}
		clear_GPIO27()
		//check if button A has been pressed and switch pattern iff yes
		a = get_GPIO24();
			if(a == 0x0){
			break;}
		
	
	}
}




////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO17_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 17 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO17_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 21 - 23. This is the field FSEL17, which maps to GPIO pin 17.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 21);

    // Set the field FSEL23 to 001, which sets pin 17 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 21);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 17.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN1 register
    r = *GPPUPPDN1;

    // Zero out bits 2-3 in this bit pattern, since this maps to
    // GPIO pin 17. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 2);

    // Write the modified bit pattern back to the
    // GPPUPPDN1 register
    *GPPUPPDN1 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO22_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 22 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO22_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 6-8. This is the field FSEL17, which maps to GPIO pin 22.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 6);

    // Set the field FSEL23 to 001, which sets pin 22 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 6);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 22.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN1 register
    r = *GPPUPPDN1;

    // Zero out bits 12-13 in this bit pattern, since this maps to
    // GPIO pin 22. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 12);

    // Write the modified bit pattern back to the
    // GPPUPPDN1 register
    *GPPUPPDN1 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO27_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 27 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO27_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 21-23. This is the field FSEL17, which maps to GPIO pin 27.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 21);

    // Set the field FSEL23 to 001, which sets pin 27 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 21);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 27.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN1 register
    r = *GPPUPPDN1;

    // Zero out bits 22-23 in this bit pattern, since this maps to
    // GPIO pin 27. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 22);

    // Write the modified bit pattern back to the
    // GPPUPPDN1 register
    *GPPUPPDN1 = r;
}
////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO17
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 17
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO17()
{
    register unsigned int r;
	  
    // Put a 1 into the SET23 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 17);
    *GPSET0 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO22
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 22
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO22()
{
    register unsigned int r;
	  
    // Put a 1 into the SET23 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 22);
    *GPSET0 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO27
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 27
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO27()
{
    register unsigned int r;
	  
    // Put a 1 into the SET23 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 27);
    *GPSET0 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO17
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 17
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO17()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR23 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 17);
    *GPCLR0 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO22
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 22
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO22()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR23 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 22);
    *GPCLR0 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO27
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 23
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO27()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR23 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 27);
    *GPCLR0 = r;
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO23_to_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 23 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 3.3V (definitely NOT 5V).
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO23_to_input()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 9 - 11. This is the field FSEL17, which maps to GPIO pin 17.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 9);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 23.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN1 register
    r = *GPPUPPDN1;

    // Zero out bits 14-15 in this bit pattern, since this maps to
    // GPIO pin 23. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 14);

    // Write the modified bit pattern back to the
    // GPPUPPDN1 register
    *GPPUPPDN1 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO24_to_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 24 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 3.3V (definitely NOT 5V).
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO24_to_input()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 12 - 14. This is the field FSEL17, which maps to GPIO pin 17.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 12);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 24.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN1 register
    r = *GPPUPPDN1;

    // Zero out bits 16-17 in this bit pattern, since this maps to
    // GPIO pin 23. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 16);

    // Write the modified bit pattern back to the
    // GPPUPPDN1 register
    *GPPUPPDN1 = r;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_GPIO23
//
//  Arguments:      none
//
//  Returns:        1 if the pin level is high, and 0 if the pin level is low.
//
//  Description:    This function gets the current value of pin 23.
//
////////////////////////////////////////////////////////////////////////////////

unsigned int get_GPIO23()
{
    register unsigned int r;
	  
	  
    // Get the current contents of the GPIO Pin Level Register 0
    r = *GPLEV0;
	  
    // Isolate pin 23, and return its value (a 0 if low, or a 1 if high)
    return ((r >> 23) & 0x1);
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_GPIO24
//
//  Arguments:      none
//
//  Returns:        1 if the pin level is high, and 0 if the pin level is low.
//
//  Description:    This function gets the current value of pin 24.
//
////////////////////////////////////////////////////////////////////////////////

unsigned int get_GPIO24()
{
    register unsigned int r;
	  
	  
    // Get the current contents of the GPIO Pin Level Register 0
    r = *GPLEV0;
	  
    // Isolate pin 24, and return its value (a 0 if low, or a 1 if high)
    return ((r >> 24) & 0x1);
}










