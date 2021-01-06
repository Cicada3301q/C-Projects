// Author: Quenten Welch 30054505
// Date: april 14th 2020
// description: A maze game that is implemented in C, will recieve output from a SNES controller 



#include "uart.h"
#include "framebuffer.h"
#include "gpio.h"
#include "systimer.h"

#define GRAY      0x00C0C0C0
#define WHITE     0x00FFFFFF
#define RED       0x00FF0000
#define GREEN     0x00008000
#define BUTTON_B  (1<<0)
#define BUTTON_Y  (1<<1)
#define BUTTON_SEL  (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_UP  (1<<4)
#define BUTTON_DOWN  (1<<5)
#define BUTTON_LEFT  (1<<6)
#define BUTTON_RIGHT  (1<<7)
#define BUTTON_A  (1<<8)
#define BUTTON_X  (1<<9)
#define BUTTON_L  (1<<10)
#define BUTTON_R  (1<<11)


// Function prototypes
unsigned short get_SNES();
void init_GPIO9_to_output();
void set_GPIO9();
void clear_GPIO9();
void init_GPIO11_to_output();
void set_GPIO11();
void clear_GPIO11();
void init_GPIO10_to_input();
unsigned int get_GPIO10();
void drawSquare(int rowStart, int columnStart, int squareSize, unsigned int color);



//int for checking current player position
int CURRENT_X, CURRENT_Y;


//array to init maze
int maze[12][16] = {

{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

{1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},

{2, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1},

{1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},

{1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},

{1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},

{1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},

{1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},

{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3},

{1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},

{1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},

{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};



////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function initializes the UART terminal and initializes
//                  a frame buffer for a 1024 x 768 display. It also initializes 
//                  pins 9, 11 to output, and 10 to input. Each pixel in the
//                  frame buffer is 32 bits in size, which encodes an RGB value
//                  (plus an 8-bit alpha channel that is not used). The program
//                  will also draw and display an 12 x 16 maze, defines the games logic 
//
/////////////////////////////////////////////////////////////////////////////////////


void main()
{
    unsigned short data, currentState = 0xFFFF;

    // Initialize the UART terminal
    uart_init();

    // Set up GPIO pin #9 for output (LATCH output)
    init_GPIO9_to_output();
    
    // Set up GPIO pin #11 for output (CLOCK output)
    init_GPIO11_to_output();
    
    // Set up GPIO pin #10 for input (DATA input)
    init_GPIO10_to_input();
    
    // Clear the LATCH line (GPIO 9) to low
    clear_GPIO9();
    
    // Set CLOCK line (GPIO 11) to high
    set_GPIO11();

    // Initialize the frame buffer
    initFrameBuffer();

    // draw game maze
    drawMaze(); 
    drawPlayer(); 

    // Loop forever, echoing characters received from the console
    // on a separate line with : : around the character
    while (1) {
      data = get_SNES(); 

        if (data != currentState){
            if((data & BUTTON_START) == BUTTON_START){
                defaultState();
                refreshSquare(CURRENT_X, CURRENT_Y);
                drawPlayer(RED);
            }  
            if((data & BUTTON_LEFT) == BUTTON_LEFT){
                if((CURRENT_X > 0 )  && (CURRENT_X - 1 != 1)){
                  refreshSquare(CURRENT_X, CURRENT_Y); 
                  CURRENT_X -= 1; 
                  drawPlayer(RED); 
                }
            }
            if((data & BUTTON_RIGHT) == BUTTON_RIGHT){
                if((CURRENT_X < 15 )  && (CURRENT_X += 1 != 1)){
                  refreshSquare(CURRENT_X, CURRENT_Y);
                  CURRENT_X += 1; 
                  drawPlayer(RED); 
                }
            }

            if((data & BUTTON_UP) == BUTTON_UP){
                if((CURRENT_Y > 0 )  && (CURRENT_Y - 1 != 1)){
                  refreshSquare(CURRENT_X, CURRENT_Y); 
                  CURRENT_Y -= 1; 
                  drawPlayer(RED); 
                }
            }
            if(((data & BUTTON_DOWN) == BUTTON_DOWN) ){
                if((CURRENT_Y < 11) && (CURRENT_Y += 1 != 1)){
                  refreshSquare(CURRENT_X, CURRENT_Y);
                  CURRENT_Y += 1; 
                  drawPlayer(RED); 
                }
            }  
            currentState = data;
        }


        if (maze[CURRENT_Y] == 8 && [CURRENT_X] == 15){
            refreshSquare(CURRENT_X,CURRENT_Y);
            drawPlayer(GREEN);
        }

        microsecond_delay(16667); 
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_SNES
//
//  Arguments:      none
//
//  Returns:        A short integer with the button presses encoded with 16
//                  bits. 1 means pressed, and 0 means unpressed. Bit 0 is
//                  button B, Bit 1 is button Y, etc. up to Bit 11, which is
//                  button R. Bits 12-15 are always 0.
//
//  Description:    This function samples the button presses on the SNES
//                  controller, and returns an encoding of these in a 16-bit
//                  integer. We assume that the CLOCK output is already high,
//                  and set the LATCH output to high for 12 microseconds. This
//                  causes the controller to latch the values of the button
//                  presses into its internal register. We then clock this data
//                  to the CPU over the DATA line in a serial fashion, by
//                  pulsing the CLOCK line low 16 times. We read the data on
//                  the falling edge of the clock. The rising edge of the clock
//                  causes the controller to output the next bit of serial data
//                  to be place on the DATA line. The clock cycle is 12
//                  microseconds long, so the clock is low for 6 microseconds,
//                  and then high for 6 microseconds. 
//
////////////////////////////////////////////////////////////////////////////////

unsigned short get_SNES()
{
    int i;
    unsigned short data = 0;
    unsigned int value;
	
	
    // Set LATCH to high for 12 microseconds. This causes the controller to
    // latch the values of button presses into its internal register. The
    // first serial bit also becomes available on the DATA line.
    set_GPIO9();
    microsecond_delay(12);
    clear_GPIO9();
	
    // Output 16 clock pulses, and read 16 bits of serial data
    for (i = 0; i < 16; i++) {
	// Delay 6 microseconds (half a cycle)
	microsecond_delay(6);
		
	// Clear the CLOCK line (creates a falling edge)
	clear_GPIO11();
		
	// Read the value on the input DATA line
	value = get_GPIO10();
		
	// Store the bit read. Note we convert a 0 (which indicates a button
	// press) to a 1 in the returned 16-bit integer. Unpressed buttons
	// will be encoded as a 0.
	if (value == 0) {
	    data |= (0x1 << i);
	}
		
	// Delay 6 microseconds (half a cycle)
	microsecond_delay(6);
		
	// Set the CLOCK to 1 (creates a rising edge). This causes the
	// controller to output the next bit, which we read half a
	// cycle later.
	set_GPIO11();
    }
	
    // Return the encoded data
    return data;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO9_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 9 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO9_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 0
    r = *GPFSEL0;

    // Clear bits 27 - 29. This is the field FSEL9, which maps to GPIO pin 9.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 27);

    // Set the field FSEL9 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 27);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL0 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 9.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 18-19 in this bit pattern, since this maps to
    // GPIO pin 9. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 18);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 9
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO9()
{
    register unsigned int r;
	  
    // Put a 1 into the SET9 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 9);
    *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 9
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO9()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR9 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 9);
    *GPCLR0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO11_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 11 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO11_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 3 - 5. This is the field FSEL11, which maps to GPIO pin 11.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 3);

    // Set the field FSEL11 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 3);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;

    
    // Disable the pull-up/pull-down control line for GPIO pin 11.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 22-23 in this bit pattern, since this maps to
    // GPIO pin 11. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 22);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 11
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO11()
{
    register unsigned int r;
	  
    // Put a 1 into the SET11 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 11);
    *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 11
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO11()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR11 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 11);
    *GPCLR0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO10_to_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 10 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 3.3V (definitely NOT 5V).
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO10_to_input()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 0 - 2. This is the field FSEL10, which maps to GPIO pin 10.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin.
    r &= ~(0x7 << 0);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 10.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 20-21 in this bit pattern, since this maps to
    // GPIO pin 10. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 20);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_GPIO10
//
//  Arguments:      none
//
//  Returns:        1 if the pin level is high, and 0 if the pin level is low.
//
//  Description:    This function gets the current value of pin 10.
//
////////////////////////////////////////////////////////////////////////////////

unsigned int get_GPIO10()
{
    register unsigned int r;
	  
	  
    // Get the current contents of the GPIO Pin Level Register 0
    r = *GPLEV0;
	  
    // Isolate pin 10, and return its value (a 0 if low, or a 1 if high)
    return ((r >> 10) & 0x1);
}

// Frame buffer global variables
unsigned int frameBufferWidth, frameBufferHeight, frameBufferPitch;
unsigned int frameBufferDepth, frameBufferPixelOrder, frameBufferSize;
unsigned int *frameBuffer;

// This funciton sets the frame buffer and allows for drawing to the screen later
void initFrameBuffer()
{
    // Initialize the mailbox data structure.
    // It contains a series of tags that specify the
    // desired settings for the frame buffer.
    mailbox_buffer[0] = 35 * 4;
    mailbox_buffer[1] = MAILBOX_REQUEST;

    mailbox_buffer[2] = TAG_SET_PHYSICAL_WIDTH_HEIGHT;
    mailbox_buffer[3] = 8;
    mailbox_buffer[4] = 0;
    mailbox_buffer[5] = FRAMEBUFFER_WIDTH;
    mailbox_buffer[6] = FRAMEBUFFER_HEIGHT;

    mailbox_buffer[7] = TAG_SET_VIRTUAL_WIDTH_HEIGHT;
    mailbox_buffer[8] = 8;
    mailbox_buffer[9] = 0;
    mailbox_buffer[10] = FRAMEBUFFER_WIDTH;
    mailbox_buffer[11] = FRAMEBUFFER_HEIGHT;
    
    mailbox_buffer[12] = TAG_SET_VIRTUAL_OFFSET;
    mailbox_buffer[13] = 8;
    mailbox_buffer[14] = 0;
    mailbox_buffer[15] = VIRTUAL_X_OFFSET;
    mailbox_buffer[16] = VIRTUAL_Y_OFFSET;
    
    mailbox_buffer[17] = TAG_SET_DEPTH;
    mailbox_buffer[18] = 4;
    mailbox_buffer[19] = 0;
    mailbox_buffer[20] = FRAMEBUFFER_DEPTH;

    mailbox_buffer[21] = TAG_SET_PIXEL_ORDER;
    mailbox_buffer[22] = 4;
    mailbox_buffer[23] = 0;
    mailbox_buffer[24] = PIXEL_ORDER_BGR;

    mailbox_buffer[25] = TAG_ALLOCATE_BUFFER;
    mailbox_buffer[26] = 8;
    mailbox_buffer[27] = 0;
    // Request: alignment; Response: frame buffer address 
    mailbox_buffer[28] = FRAMEBUFFER_ALIGNMENT;
    mailbox_buffer[29] = 0;    // Response: Frame buffer size

    mailbox_buffer[30] = TAG_GET_PITCH;
    mailbox_buffer[31] = 4;
    mailbox_buffer[32] = 0;
    mailbox_buffer[33] = 0;    // Response: Pitch

    mailbox_buffer[34] = TAG_LAST;


    // Make a mailbox request using the above mailbox data structure
    if (mailbox_query(CHANNEL_PROPERTY_TAGS_ARMTOVC)) {
	// If here, the query succeeded, and we can check the response

	// Get the returned frame buffer address, masking out 2 upper bits
        mailbox_buffer[28] &= 0x3FFFFFFF;
        frameBuffer = (void *)((unsigned long)mailbox_buffer[28]);

	// Read the frame buffer settings from the mailbox buffer
        frameBufferWidth = mailbox_buffer[5];
        frameBufferHeight = mailbox_buffer[6];
        frameBufferPitch = mailbox_buffer[33];
        frameBufferDepth = mailbox_buffer[20];
        frameBufferPixelOrder = mailbox_buffer[24];
        frameBufferSize = mailbox_buffer[29];
    } else {
        uart_puts("Cannot initialize frame buffer\n");
    }
}

// Draws a square in the maze given a color, drawn from top left of square to bottom right
// and filled with pixels used nested loops
void drawSquare(int rowStart, int columnStart, int squareSize, unsigned int color)
{
    int row, column, rowEnd, columnEnd;
    unsigned int *pixel = frameBuffer;


    // Calculate where the row and columns end
    rowEnd = rowStart + squareSize;
    columnEnd = columnStart + squareSize;

    // Draw the square row by row, from the top down
    for (row = rowStart; row < rowEnd; row++) {
	// Draw each pixel in the row from left to right
        for (column = columnStart; column < columnEnd; column++) {
	    // Draw the individual pixel by setting its
	    // RGB value in the frame buffer
            pixel[(row * frameBufferWidth) + column] = color;
        }
    }
}




// Draws the maze by going through the maze array horizontally row by row and drawing the
// corresponding square according to the value in the maze array at the specified index

void drawMaze(int numberOfRows, int numberOfColumns, int squareSize,
		       unsigned int color1, unsigned int color2)
{
    int i, j;
    
    // Go through array drawing each square, going from each rowing drawing every column
    // AKA drawing the maze horizontally
    for (i = 0; i < numberOfRows; i++) {
        for (j = 0; j < numberOfColumns; j++) {
            if (maze[i][j] == 0 || maze[i][j] == 2 || maze[i][j] == 3) {
                // The square is a path, entrance or exit
                drawSquare(i * squareSize, j * squareSize, squareSize, WHITE);
            } else {
                // The square is a wall
                drawSquare(i * squareSize, j * squareSize, squareSize, GRAY);
            }
        }
    }
}

void displayFrameBuffer()
{
    int squareSize, numberOfRows, numberOfColumns;

    // Set square size as described in assignment
    squareSize = 64;

    // Calculate the number of rows and columns given the square size
    numberOfRows = frameBufferHeight / squareSize;
    numberOfColumns = frameBufferWidth / squareSize;
 
    drawMaze(numberOfRows, numberOfColumns, squareSize, WHITE, GRAY);

}


void defaultState(){
    CURRENT_X = 0;
    CURRENT_Y = 2;
}


void drawMaze(){
    for(unsigned int x = 0; x <16; x++){
        for(unsigned int y = 0; y <12; y++){
            refreshSquare(x,y); 
        }
    }
}

void drawPlayer(unsigned int color){
    drawSquare(CURRENT_Y*64, CURRENT_X*64, 64, color);
}

void refreshSquare(int x, int y){
  if (maze[y][x]== 0 || maze[y][x] == 2 || maze[y][x] ==3) {
        drawSquare(y*64, x*64, 64, WHITE);
    }else if (maze[y][x]==1){
        drawSquare(y*64, x*64, 64, GRAY);
    }
}   

