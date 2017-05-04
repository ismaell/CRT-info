#include "crtinfo.h"

#if 0
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
#endif

#define VIDEO_MODE VDP_TEXT_MODE
void init_video(){
	const unsigned char *ptr;
	int i;

	// Clear Screen:
	vdp_set_vram_addr(NAME_TABLE_BASE_ADDR);
	for (i=0; i<TILE_WIDTH*TILE_HEIGHT; i++){
		*((__xdata unsigned char*) VDP_DATA) = 0;
	}
	
	// Load patterns:
	vdp_set_vram_addr(PATTERN_TABLE_BASE_ADDR);
	for (ptr = bitmap_font; ptr < (bitmap_font+PATTERNS_DATA_LENGTH); ptr++){
		*((__xdata unsigned char*) VDP_DATA) = (*ptr);
	}

	// Setup VDP register values:
	write_register(0, M3(VIDEO_MODE));
	write_register(1, RAM_16K | ENABLE_ACTIVE_DISPLAY | M1_M2(VIDEO_MODE));
	write_register(2, NAME_TABLE_BASE_ADDR / 0x400);
	write_register(3, COLOR_TABLE_BASE_ADDR / 0x40);
	write_register(4, PATTERN_TABLE_BASE_ADDR / 0x800);
	write_register(5, SPRITE_ATTRIBUTE_TABLE_BASE_ADDR / 0x80);
	write_register(6, SPRITE_PATTERN_TABLE_BASE_ADDR / 0x800);
	write_register(7, TEXT_BACKGROUND(BLACK) | TEXT_FOREGROUND(LIGHT_GREEN));
}

void main(void)
{
	int i=5;
	//setup_uart1();
	init_video();

	while(1){
		print(i,i, "ABACATE");
		i++;
		delay();
	};
}
