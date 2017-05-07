// (c) Felipe Sanches <juca@members.fsf.org>
// Released under the terms of the GNU General Public License v3 (or later)

#include "crtinfo.h"

#define XTAL_11_0592MHz 11059200
#define BAUDRATE 28800
void setup_uart1(){
	// 8 bit, baud=28800, 1 stop-bit, no parity

	PCON &= 0x7F; // PCON.7 = 0
	TMOD = 0x20;
	SCON = 0x50;
	TH1  = 255; //256 - ((XTAL_11_0592MHz / 384) / BAUDRATE);
	TL1  = 0x00;
	TR1  = 1;
}

#define VIDEO_MODE VDP_TEXT_MODE
void init_video(){
	const unsigned char *ptr;

	clear_screen();
	
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

int border_time = 0;
#define BORDER_SPACING 3
void draw_border(const char* c){
	int x,y;
	border_time = (border_time+1)%BORDER_SPACING;
	for (x=0; x<TILE_WIDTH; x++){
		print(x, 0, (x + border_time) % BORDER_SPACING == 0 ? c: " ");
		print(x, TILE_HEIGHT - 1, (x - border_time) % BORDER_SPACING == 0 ? c: " ");
	}
	for (y=0; y<TILE_HEIGHT; y++){
		print(0, y, (y - border_time) % BORDER_SPACING == 0 ? c: " ");
		print(TILE_WIDTH - 1, y, (y + border_time) % BORDER_SPACING == 0 ? c: " ");
	}
}

void main(void)
{
	int x=5, y=1, vx=1, vy=1;
	setup_uart1();
	init_video();

	while(1){
		clear_screen();

		//TODO: receive and process remote commands sent by
                //an external device over the serial port.

		#define TEXT_BLOCK_WIDTH 25
		#define TEXT_BLOCK_HEIGHT 6
		print(x,y,   "   GAROA HACKER CLUBE    ");

		print(x,y+2, "   07 DE MAIO DE 2017    ");

		print(x,y+4, "  Melhore este display!  ");
		print(x,y+5, "github.com/garoa/CRT-info");

                draw_border("X");
		delay(20000);
                draw_border("-");
		delay(20000);
                draw_border(".");
		delay(20000);

		x+=vx;
		y+=vy;
		if (x >= TILE_WIDTH - TEXT_BLOCK_WIDTH){
			x = (TILE_WIDTH - TEXT_BLOCK_WIDTH - vx);
			vx = -vx;
		}
		if (y >= TILE_HEIGHT - TEXT_BLOCK_HEIGHT){
			y = (TILE_HEIGHT - TEXT_BLOCK_HEIGHT - vy);
			vy = -vy;
		}
		if (x < 1){x = 1; vx = -vx;}
		if (y < 1){y = 1; vy = -vy;}


	};
}
