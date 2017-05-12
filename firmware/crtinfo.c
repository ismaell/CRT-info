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
	TH1  = (unsigned int)(256 - XTAL_11_0592MHz / 384 / BAUDRATE);
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

enum {
	CRTINFO_DEFAULT_MESSAGE=0,
	DISPLAY_HACKERS_TEXT=1
} crtinfo_mode;

static void draw_bouncing_message(){
	static int x=5, y=1, vx=1, vy=1;
	static int t=0;

	clear_screen();

	#define TEXT_BLOCK_WIDTH 25
	#define TEXT_BLOCK_HEIGHT 6
	print(x,y,   "   GAROA HACKER CLUBE    ");
	print(x,y+2, "   10 DE MAIO DE 2017    ");

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

	if (t++ > 50){
		crtinfo_mode = DISPLAY_HACKERS_TEXT;
		t = 0;
		clear_screen();
	}
}

__code const char* __code hackers[] = {
"The Midnight Computer Wiring Society",
"",
"-------------- Part I --------------",
"",
"GREENBLATT was hacker of systems and",
" visionary of application; Gosper ",
"was metaphysical explorer and ",
"handyman of the esoteric. Together ",
"they were two legs of a techno-",
"cultural triangle which would serve",
" as the Hacker Ethic's foundation in",
"its rise to cultural supremacy at ",
"MIT in the coming years. The third ",
"leg of the triangle arrived in the ",
"fall of 1963, and his name was ",
"Stewart Nelson.",
"",
"Not long after his arrival, Stew ",
"Nelson displayed his curiosity and ",
"ability to get into uncharted ",
"electronic realms, traits which ",
"indicated his potential to become a",
"master magician in service to the ",
"Hacker Ethic. As was the custom, ",
"Nelson had come a week early for ",
"Freshman Rush. He was a short kid,",
" generally taciturn, with curly ",
"hair, darting brown eyes, and a ",
"large overbite which gave him the",
" restlessly curious look of a small",
" rodent. Indeed, Stewart Nelson ",
"was sniffing out sophisticated ",
"electronics equipment that he could",
" play on, and it did not take him",
"long to find what he wanted at MIT.",
"",
"It began at WTBS, the campus radio ",
"station. Bob Clements, a student ",
"worker at the station who would ",
"later do some PDP-6 hacking, was ",
"showing a group of freshmen the ",
"control rooms when he opened a door",
"that opened to the complex machinery",
"and found Stew Nelson, \"a weaselly",
" little kid,\" he later remembered,",
" \"who had his fingers on the guts",
" of our phone lines and our East",
" Campus radio transmitter.\"",
"",
"Eventually, he found his way to the",
" PDP-1 in the Kluge Room. The ",
"machine got Stewart Nelson very",
" excited. He saw this friendly ",
"computer which you could put your",
" hands on, and with a confidence ",
"that came from what Greenblatt ",
"might call born hackerism he got ",
"to work. He noticed immediately how",
" the One's outside speaker was ",
"hooked to the computer, and how ",
"Peter Samson's music program could",
" control that speaker. So one night,",
" very late, when John McKenzie and",
" the people tending the TX-0 next ",
"door were asleep in their homes,",
" Stewart Nelson set about learning",
" to program the PDP-1, and it did ",
"not take him long to teach the ",
"PDP-1 some new tricks. He had ",
"programmed some appropriate tones",
" to come out of the speaker and ",
"into the open receiver of the",
" campus phone that sat in the",
" Kluge Room.",
"",
"These tones made the phone system",
" come to attention, so to speak,",
" and dance.",
"",
"Dance, phone lines, dance!",
"",
"And the signals did dance. They ",
"danced from one place on the MIT",
" tie-line system to the next and",
" then to the Haystack Observatory",
" (connected to MIT's system),",
"where they danced to an open line",
" and, thus liberated, danced out",
" into the world.",
"",
"There was no stopping them,",
" because the particular tones ",
"which Stew Nelson had generated",
" on the PDP-1 were the exact tones",
" which the phone company used to",
"send its internal calls around the",
" world, and Stew Nelson knew that ",
"they would enable him to go all",
" around the marvelous system which",
" was the phone company without ",
"paying a penny.",
"",
"This analog alchemist, the new ",
"hacker king, was showing a deeply",
" impressed group of PDP-1 ",
"programmers how a solitary college",
" freshman could wrest control of ",
"the nearly hundred-year-old phone",
" system, using it not for profit",
"but for sheer joyriding exploration.",
"Word spread of these exploits, and ",
"Nelson began to achieve heroic ",
"status around TMRC and the Kluge",
" Room; soon some of the more ",
"squeamish PDP-1 people were doing",
" some hand-wringing about whether",
" he had gone too far. Greenblatt ",
"did not think so, nor did any true",
" hacker: people had done that sort",
" of thing around TMRC for years; and",
" if Nelson took things a step",
" beyond, that was a positive ",
"outgrowth of the Hacker Ethic. But",
" when John McKenzie heard of it he",
" ordered Nelson to stop, probably ",
"realizing that there was not much",
" he could do to slow Stew Nelson's",
" eternal quest for systems",
" knowledge. \"How can you stop ",
"talent like that?\" he later ",
"reflected. As it turned out, things",
" were going to go much further ",
"before Stewart Nelson was through.",
"In some ways, they would never stop."
};

void display_hackers_text(){
	static int t=-TILE_HEIGHT+2;
	int y;
	for (y=1; y<TILE_HEIGHT-1; y++){
                clear_line(y);
		if ((t+y>=0) && t+y < (sizeof(hackers) / sizeof(__code const char*))){
			print(1, y, hackers[t+y]);
		}
	}
	delay(65534);
	delay(65534);
	delay(45000);
	t++;
	if (t > (sizeof(hackers) / sizeof(__code const char*))){
		t=-TILE_HEIGHT+2;
		crtinfo_mode = CRTINFO_DEFAULT_MESSAGE;
	}
}

void main(void)
{
	setup_uart1();
	init_video();
	crtinfo_mode = CRTINFO_DEFAULT_MESSAGE;

	while(1){
		//TODO: receive and process remote commands sent by
                //an external device over the serial port.

		switch(crtinfo_mode){
		case DISPLAY_HACKERS_TEXT:
			display_hackers_text(); break;
		default:
			draw_bouncing_message(); break;
		}
	};
}
