//EMIF.LCD.c Partial program for display of spectrum through EMIF using LCDs
#define IOPORT 0xA1111111						//EMIF address
int *ioport = (int *)IOPORT;					//pointer to get data out
int input, output;								//temp storage
void set_LCD_characters();						//prototypes
void send_LCD_characters();
void init_LCD();
void LCD_PUT_CMD(int data);
void LCD_PUT_CHAR(int data);
void delay();
float bandage[16];								//holds FFT array after downsizing
short k=0, j=0;
int toprow[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int botrow[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
short rowselect = 1;								//start on top row
short colselect = 0;								//start on left of LCD
#define LCD_CTRL_INIT 0x38						//initialization for LCD
#define LCD_CTRL_OFF 0x08
#define LCD_CTRL_ON 0x0C
#define LCD_AUTOINC 0x06
#define LCD_ON 0x0C
#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xC0   				//address of second line

main()
{
 .
 .
 init_LCD();										//init LCD
 while(1)				    						//infinite loop
  {
	for(k=0; k<16; k++){							//for 16 bands
		float sum = 0;								//temp storage
		for(j=0; j<8; j++)						//for 8 samples per band
			sum += x1[8*k+j];						//sum up samples
		bandage[k] = (sum/8);					//take average
   }
	set_LCD_characters();						//set up character arrays
	send_LCD_characters();						//put them on LCD
  }                               			//end of infinite loop
} 					    								//end of main

interrupt void c_int11()	    				//ISR
{
  output_sample(bandage[buffercount/16]); //out from iobuffer
  .
  .
}

void set_LCD_characters()						//to fill arrays with characters
{
	int n = 0;										//temp index variable

	for (n=0; n<16; n++)
	{
		if(bandage[n] > 40000)					//first threshold
		{
			toprow[n] = 0xFF;						//block character
			botrow[n] = 0xFF;
		}
		else if(bandage[n] > 20000)			//second threshold
		{
			toprow[n] = 0x20;						//blank space
			botrow[n] = 0xFF;
		}
		else 											//below second threshold
		{
			toprow[n] = 0x20;
			botrow[n] = 0x20;
		}
	}
}

void send_LCD_characters()
{
	int m=0;
	LCD_PUT_CMD(LCD_FIRST_LINE);				//start address
	for (m=0; m<16; m++)							//display top row
		LCD_PUT_CHAR(toprow[m]);
	LCD_PUT_CMD(LCD_SECOND_LINE);				//second line
	for (m=0; m<16; m++)							//display bottom row
		LCD_PUT_CHAR(botrow[m]);
}

void init_LCD()
{
	LCD_PUT_CMD(LCD_CTRL_INIT);				//put command
	LCD_PUT_CMD(LCD_CTRL_OFF);					//off display
	LCD_PUT_CMD(LCD_CTRL_ON);					//turn on
	LCD_PUT_CMD(0x01);							//clear display
	LCD_PUT_CMD(LCD_AUTOINC);					//set address mode
	LCD_PUT_CMD(LCD_CTRL_ON);					//set it
}

void LCD_PUT_CMD(int data)
{
	*ioport = (data & 0x000000FF);			//RS=0, RW=0
	delay();
	*ioport = (data | 0x20000000); 			//bring enable line high
	delay();
	*ioport = (data & 0x000000FF); 			//bring enable line low
	delay();
}

void LCD_PUT_CHAR(int data)
{
	*ioport = ((data & 0x000000FF)| 0x80000000);		//RS=1, RW=0
	*ioport = ((data & 0x000000FF)| 0xA0000000);		//enable high
	*ioport = ((data & 0x000000FF)| 0x80000000);		//enable Low
	delay();
}

void delay()													//create 1 ms delay
{
	int q=0, junk=2;
   for (q=0; q<8000; q++)
   	junk = junk*junk;
}
	

