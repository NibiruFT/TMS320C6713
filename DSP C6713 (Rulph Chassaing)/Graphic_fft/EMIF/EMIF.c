//EMIF.c Illustrates output through EMIF 80-pin connector J4

#define OUTPUT 0xA0000000				//EMIF output address
int *output = (int *)OUTPUT;

void main()
{
	*output = 0x00000001;				//outputs 0x1 to EMIF bus
}
 

