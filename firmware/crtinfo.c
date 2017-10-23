// (c) Felipe Sanches <juca@members.fsf.org>
// Released under the terms of the GNU General Public License v3 (or later)

#include "crtinfo.h"
#include "video.h"

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

	vdp_clear_screen();

	#define TEXT_BLOCK_WIDTH 25
	#define TEXT_BLOCK_HEIGHT 6
	print(x,y,
	      "   GAROA HACKER CLUBE    \n"
	      "\n"
	      "   10 DE MAIO DE 2017    \n"
	      "\n"
	      "  Melhore este display!  \n"
	      "github.com/garoa/CRT-info");

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
		vdp_clear_screen();
	}
}

__code const char hackers[] =
"The Midnight Computer Wiring Society\n"
"\n"
"-------------- Part I --------------\n"
"\n"
"GREENBLATT was hacker of systems and\n"
"visionary of application; Gosper\n"
"was metaphysical explorer and\n"
"handyman of the esoteric. Together\n"
"they were two legs of a techno-\n"
"cultural triangle which would serve\n"
"as the Hacker Ethic's foundation in\n"
"its rise to cultural supremacy at\n"
"MIT in the coming years. The third\n"
"leg of the triangle arrived in the\n"
"fall of 1963, and his name was\n"
"Stewart Nelson.\n"
"\n"
"Not long after his arrival, Stew\n"
"Nelson displayed his curiosity and\n"
"ability to get into uncharted\n"
"electronic realms, traits which\n"
"indicated his potential to become a\n"
"master magician in service to the\n"
"Hacker Ethic. As was the custom,\n"
"Nelson had come a week early for\n"
"Freshman Rush. He was a short kid,\n"
"generally taciturn, with curly\n"
"hair, darting brown eyes, and a\n"
"large overbite which gave him the\n"
"restlessly curious look of a small\n"
"rodent. Indeed, Stewart Nelson\n"
"was sniffing out sophisticated\n"
"electronics equipment that he could\n"
"play on, and it did not take him\n"
"long to find what he wanted at MIT.\n"
"\n"
"It began at WTBS, the campus radio\n"
"station. Bob Clements, a student\n"
"worker at the station who would\n"
"later do some PDP-6 hacking, was\n"
"showing a group of freshmen the\n"
"control rooms when he opened a door\n"
"that opened to the complex machinery\n"
"and found Stew Nelson, \"a weaselly\n"
"little kid,\" he later remembered,\n"
"\"who had his fingers on the guts\n"
"of our phone lines and our East\n"
"Campus radio transmitter.\"\n"
"\n"
"Eventually, he found his way to the\n"
"PDP-1 in the Kluge Room. The\n"
"machine got Stewart Nelson very\n"
"excited. He saw this friendly\n"
"computer which you could put your\n"
"hands on, and with a confidence\n"
"that came from what Greenblatt\n"
"might call born hackerism he got\n"
"to work. He noticed immediately how\n"
"the One's outside speaker was\n"
"hooked to the computer, and how\n"
"Peter Samson's music program could\n"
"control that speaker. So one night,\n"
"very late, when John McKenzie and\n"
"the people tending the TX-0 next\n"
"door were asleep in their homes,\n"
"Stewart Nelson set about learning\n"
"to program the PDP-1, and it did\n"
"not take him long to teach the\n"
"PDP-1 some new tricks. He had\n"
"programmed some appropriate tones\n"
"to come out of the speaker and\n"
"into the open receiver of the\n"
"campus phone that sat in the\n"
"Kluge Room.\n"
"\n"
"These tones made the phone system\n"
"come to attention, so to speak,\n"
"and dance.\n"
"\n"
"Dance, phone lines, dance!\n"
"\n"
"And the signals did dance. They\n"
"danced from one place on the MIT\n"
"tie-line system to the next and\n"
"then to the Haystack Observatory\n"
"(connected to MIT's system),\n"
"where they danced to an open line\n"
"and, thus liberated, danced out\n"
"into the world.\n"
"\n"
"There was no stopping them,\n"
"because the particular tones\n"
"which Stew Nelson had generated\n"
"on the PDP-1 were the exact tones\n"
"which the phone company used to\n"
"send its internal calls around the\n"
"world, and Stew Nelson knew that\n"
"they would enable him to go all\n"
"around the marvelous system which\n"
"was the phone company without\n"
"paying a penny.\n"
"\n"
"This analog alchemist, the new\n"
"hacker king, was showing a deeply\n"
"impressed group of PDP-1\n"
"programmers how a solitary college\n"
"freshman could wrest control of\n"
"the nearly hundred-year-old phone\n"
"system, using it not for profit\n"
"but for sheer joyriding exploration.\n"
"Word spread of these exploits, and\n"
"Nelson began to achieve heroic\n"
"status around TMRC and the Kluge\n"
"Room; soon some of the more\n"
"squeamish PDP-1 people were doing\n"
"some hand-wringing about whether\n"
"he had gone too far. Greenblatt\n"
"did not think so, nor did any true\n"
"hacker: people had done that sort\n"
"of thing around TMRC for years; and\n"
"if Nelson took things a step\n"
"beyond, that was a positive\n"
"outgrowth of the Hacker Ethic. But\n"
"when John McKenzie heard of it he\n"
"ordered Nelson to stop, probably\n"
"realizing that there was not much\n"
"he could do to slow Stew Nelson's\n"
"eternal quest for systems\n"
"knowledge. \"How can you stop\n"
"talent like that?\" he later\n"
"reflected. As it turned out, things\n"
"were going to go much further\n"
"before Stewart Nelson was through.\n"
"In some ways, they would never stop."
;

void display_hackers_text(){
	print(1, TILE_HEIGHT-1, hackers);
	crtinfo_mode = CRTINFO_DEFAULT_MESSAGE;
	delay(65534);
	delay(65534);
	delay(65534);
	delay(65534);
}

void main(void)
{
	setup_uart1();
	vdp_init();
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
