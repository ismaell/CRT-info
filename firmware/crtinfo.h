// (c) Felipe Sanches <juca@members.fsf.org>
// Released under the terms of the GNU General Public License v3 (or later)

#include "bitmapfont.h"
#include <at89x51.h> // Definitions of registers, SFRs and Bits

// ISR Prototypes ===================================================
void INT0_ISR(void)	__interrupt 0; // ISR for External __interrupt 0
void T0_ISR(void)	__interrupt 1; // ISR for Timer0/Counter0 Overflow
void INT1_ISR(void)	__interrupt 2; // ISR for External __interrupt 1
void T1_ISR(void)	__interrupt 3; // ISR for Timer1/Counter1 Overflow
void UART_ISR(void)	__interrupt 4; // ISR for UART __interrupt

#define VDP_DATA 0xff0e
#define VDP_COMMAND 0xff0f
#define TILE_WIDTH 40
#define TILE_HEIGHT 24

#define NAME_TABLE_BASE_ADDR             0x0000
#define COLOR_TABLE_BASE_ADDR            0x0400
#define PATTERN_TABLE_BASE_ADDR          0x0800
#define SPRITE_PATTERN_TABLE_BASE_ADDR   0x0C00
#define SPRITE_ATTRIBUTE_TABLE_BASE_ADDR 0x1000
#define TEXT_BACKGROUND(c) (c & 0x0f)
#define TEXT_FOREGROUND(c) ((c & 0x0f) << 4)

//Video modes:
#define VDP_GRAPHICS_1_MODE 0b000
#define VDP_GRAPHICS_2_MODE 0b001
#define VDP_MULTICOLOR_MODE 0b010
#define VDP_TEXT_MODE       0b100

//Register #0 bits:
#define M3(_mode) ((_mode & 0x01) << 1)

//Register #1 bits:
#define RAM_16K (1 << 7)
#define ENABLE_ACTIVE_DISPLAY (1 << 6)
#define INTERRUPT_ENABLE (1 << 5)
#define M1_M2(_mode) (((_mode >> 1) & 0x03) << 3)

#define BLACK 0
#define TRANSPARENT 1
#define MEDIUM_GREEN 2
#define LIGHT_GREEN 3
#define DARK_BLUE 4
#define LIGHT_BLUE 5
#define DARK_RED 6
#define CYAN 7
#define MEDIUM_RED 8
#define LIGHT_RED 9
#define DARK_YELLOW 10
#define LIGHT_YELLOW 11
#define DARK_GREEN 12
#define MAGENTA 13
#define GRAY 14 
#define WHITE 15

void vdp_set_vram_addr(unsigned int address);

void clear_line(int line_number){
	int x;
        vdp_set_vram_addr(NAME_TABLE_BASE_ADDR + TILE_WIDTH*line_number);
        for (x=0; x<TILE_WIDTH; x++){
                *((__xdata unsigned char*) VDP_DATA) = TILEID_SPACE;
        }
}

void clear_screen(){
        int i;
        vdp_set_vram_addr(NAME_TABLE_BASE_ADDR);
        for (i=0; i<TILE_WIDTH*TILE_HEIGHT; i++){
                *((__xdata unsigned char*) VDP_DATA) = TILEID_SPACE;
        }
}

void write_register(unsigned char reg, unsigned char value){
	*((__xdata unsigned char*) (VDP_COMMAND)) = value;
	*((__xdata unsigned char*) VDP_COMMAND) = 0x80 | (reg & 0x7);
}

void vdp_set_vram_addr(unsigned int address){
	address--; //Why?!
	*((__xdata unsigned char*) VDP_COMMAND) = (address & 0xff);
	*((__xdata unsigned char*) VDP_COMMAND) = ((address >> 8) & 0x3f);
}

//These are the decoding rules of our bitmap font:
inline int decode_char_into_tileid(char ch){
	if (ch >= 'A' && ch <= 'Z')		return (TILEID_A + ch - 'A');
	else if (ch >= 'a' && ch <= 'z')	return (TILEID_a + ch - 'a');
	else if (ch >= '0' && ch <= '9')	return (TILEID_0 + ch - '0');
	else if (ch == ' ')	return TILEID_SPACE;
	else if (ch == '/')	return TILEID_SLASH;
	else if (ch == '-')	return TILEID_DASH;
	else if (ch == '.')	return TILEID_PERIOD;
	else if (ch == ',')	return TILEID_COMMA;
	else if (ch == ';')	return TILEID_SEMICOLON;
	else if (ch == '!')	return TILEID_EXCLAMATION;
	else if (ch == '\'')	return TILEID_SINGLEQUOTE;
	else if (ch == '\"')	return TILEID_DOUBLEQUOTE;
	else	return TILEID_MISSING_GLYPH;
}

void print_len(int x, int y, const char* str, int len){
	int l;
	vdp_set_vram_addr(NAME_TABLE_BASE_ADDR + x + y * TILE_WIDTH);

	for (l=0; l<len; l++){
		*((__xdata char*) VDP_DATA) = decode_char_into_tileid(str[l]);
	}
}

void print(int x, int y, const char* str){
	vdp_set_vram_addr(NAME_TABLE_BASE_ADDR + x + y * TILE_WIDTH);

	for (; *str != 0; str++){
		*((__xdata char*) VDP_DATA) = decode_char_into_tileid(*str);
	}
}

void delay(unsigned int delta){
	//Note: The maximum allowed delta value here is 65534.
	//TODO: This function could be improved to compute msecs or seconds...
	//      Alternatively we could also use hardware interrupts
        //      to keep track of time with better precision.
	unsigned int t = 0;
	while (t<delta){
		t++;
	}
}

void INT0_ISR(void)	__interrupt 0
{}
void T0_ISR(void)	__interrupt 1
{}
void INT1_ISR(void)	__interrupt 2
{}
void T1_ISR(void)	__interrupt 3
{}
void UART_ISR(void)	__interrupt 4
{}

