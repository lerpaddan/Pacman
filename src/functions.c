
/* functions.c */

#include <pic32mx.h>
#include "functions.h"



/* BEGINNING OF COPIED CODE */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}



uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

/* END OF COPIED CODE */



/* Integer to decimal string conversion function
 */
void itodsconv(char *buffer, int x)
{
	/* Check negative flag */
	if(x < 0)
	{
		x = -x;
		*(buffer++) = '-';
	}
	
	/* Compute length */
	int y = x;
	do
	{
		++buffer;
		y /= 10;
	}while(y);
	*(buffer--) = '\0';
	
	/* Write */
	do
	{
		*(buffer--) = x % 10 + '0';
		x /= 10;
	}while(x);
}




/* Reverse every bit in one byte 
 */
int8_t reverse_byte(int8_t x)
{
	int8_t y = 0;
	
	for(int i = 0; i < 7; ++i)
	{
		y |= ((x & (1 << i)) >> i) << (7 - i);
	}
	return y;
}
