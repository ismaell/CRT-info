#include "delay.h"
#include "video.h"
#include "bitmapfont.h"


/**
 * Write to VRAM address.
 */
inline static
void vdp_waddr(unsigned int address){
	*((__xdata unsigned char*) VDP_COMMAND) = (address & 0xff);
	*((__xdata unsigned char*) VDP_COMMAND) = 0x40 | ((address >> 8) & 0x3f);
}

/**
 * Read from VRAM address.
 */
inline static
void vdp_raddr(unsigned int address){
	*((__xdata unsigned char*) VDP_COMMAND) = address & 0xff;
	*((__xdata unsigned char*) VDP_COMMAND) = (address >> 8) & 0x3f;
}

inline static
void _vram_write_run(char ch, unsigned int len){
	while (len--)
		*((__xdata unsigned char*) VDP_DATA) = ch;
}

inline static
void _vram_write(const char *src, unsigned int len){
	while(len--)
		*((__xdata unsigned char*) VDP_DATA) = *src++;
}

inline static
void _vram_read(char *dst, unsigned int len){
	while (len--)
		*dst++ = *((__xdata unsigned char*) VDP_DATA);
}

void vdp_clear_line(int line_number){
	vdp_waddr(NAME_TABLE_BASE_ADDR + TILE_WIDTH * line_number);
	_vram_write_run(0, TILE_WIDTH);
}

void vdp_clear_screen(void){
	vdp_waddr(NAME_TABLE_BASE_ADDR);
	_vram_write_run(0, TILE_WIDTH * TILE_HEIGHT);
}

inline static
void vdp_reg_write(unsigned char reg, unsigned char value){
	*((__xdata unsigned char*) (VDP_COMMAND)) = value;
	*((__xdata unsigned char*) VDP_COMMAND) = 0x80 | (reg & 0x7);
}

#define VIDEO_MODE VDP_TEXT_MODE
void vdp_init(void){
	const unsigned char *ptr;

	vdp_clear_screen();

	// Load patterns:
	vdp_waddr(PATTERN_TABLE_BASE_ADDR);
	for (ptr = bitmap_font; ptr < bitmap_font_end; ptr++){
		*((__xdata unsigned char*) VDP_DATA) = (*ptr);
	}

	// Setup VDP register values:
	vdp_reg_write(0, M3(VIDEO_MODE));
	vdp_reg_write(1, RAM_16K | ENABLE_ACTIVE_DISPLAY | M1_M2(VIDEO_MODE));
	vdp_reg_write(2, NAME_TABLE_BASE_ADDR >> 10);
	vdp_reg_write(3, COLOR_TABLE_BASE_ADDR / 0x40);
	vdp_reg_write(4, PATTERN_TABLE_BASE_ADDR / 0x800);
	vdp_reg_write(5, SPRITE_ATTRIBUTE_TABLE_BASE_ADDR / 0x80);
	vdp_reg_write(6, SPRITE_PATTERN_TABLE_BASE_ADDR / 0x800);
	vdp_reg_write(7, TEXT_BACKGROUND(BLACK) | TEXT_FOREGROUND(LIGHT_GREEN));
}

#define LAST_LINE (NAME_TABLE_BASE_ADDR + TILE_WIDTH * (TILE_HEIGHT - 1))

static
void scroll1(){
	char buf[TILE_WIDTH];
	unsigned int addr = NAME_TABLE_BASE_ADDR;

	delay(65534);
	delay(65534);
	delay(32767);

	for (; addr < LAST_LINE; addr += TILE_WIDTH) {
		vdp_raddr(addr + TILE_WIDTH);
		_vram_read(buf, sizeof(buf));
		vdp_waddr(addr);
		_vram_write(buf, sizeof(buf));
	}
	_vram_write_run(0, TILE_WIDTH);
}

inline static
void _putchar(char ch){
	*((__xdata char*) VDP_DATA) = ch > 126 ? 0 : ch - ' ';
}

void vdp_puts(unsigned int addr, const char* str){
	char ch;

	vdp_waddr(addr);

	while (ch = *str++){
		if (ch < ' '){
			switch (ch){
			case '\n':
				if (addr < LAST_LINE)
					addr += TILE_WIDTH;
				else
					scroll1();
			case '\r':
				vdp_waddr(addr);
				break;
			}
		} else {
			_putchar(ch);
		}
	}
}
