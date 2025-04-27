
#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 

#define BLACK 0x0F
#define RED 0x05
#define BROWN 0x16
#define YELLOW 0x27 
 
#pragma bss-name(push, "ZEROPAGE")

unsigned char pad;
unsigned char collision1;
unsigned char collision2;
unsigned char collision3;
unsigned char collision4;
unsigned char collision5;
unsigned char bottomCollision;
unsigned char topCollision;

struct collisionBox {
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
};

struct collisionBox player = {128,0,6,8};

int xpos1;
int xpos2;
int xpos3;
int xpos4;
int xpos5;
int reset = 1;
int score = 0;
int end = 0;
int speed = 1;

struct collisionBox box1 = {128,36,8,8};
struct collisionBox box2 = {128,72,8,8};
struct collisionBox box3 = {128,108,8,8};
struct collisionBox box4 = {128,144,8,8};
struct collisionBox box5 = {128,180,8,8};
struct collisionBox bottomBox = {0,220,250,1};
struct collisionBox topBox = {0,10,250,1};

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

unsigned char i;
unsigned char j;

int seed = 12345;
 
const unsigned char palette_bg[]={
BLACK, RED, BROWN, YELLOW,
0,0,0,0,
0,0,0,0,
0,0,0,0
}; 

const unsigned char palette_sp[]={
BLACK, RED, BLACK, YELLOW,
0,0,0,0,
0,0,0,0,
0,0,0,0
}; 

void main (void) {

	ppu_off(); // screen off
		
	pal_bg(palette_bg);



	while(!pad_poll(0)) {
		// Wait
	}

	pal_bg(palette_bg); 
	pal_spr(palette_sp);

	bank_spr(1);
		
	// set a starting point on the screen
	vram_adr(NTADR_A(0,0)); // screen is 32 x 30 tiles

	for(j = 0; j < 30; j++){
		for(i = 0; i < 32; i++){
			vram_put(i); 
		}
	}

	ppu_on_all();

	while (1){

		set_rand(seed);

		xpos1 = 48 + rand8() * 152 / 255;
		xpos2 = 48 + rand8() * 152 / 255;
		xpos3 = 48 + rand8() * 152 / 255;
		xpos4 = 48 + rand8() * 152 / 255;
		xpos5 = 48 + rand8() * 152 / 255;

		box1.x = xpos1;
		box2.x = xpos2;
		box3.x = xpos3;
		box4.x = xpos4;
		box5.x = xpos5;

		ppu_wait_nmi(); // wait till beginning of the frame
		// the sprites are pushed from a buffer to the OAM during nmi
		

		
		// clear all sprites from sprite buffer
		oam_clear();
		
		pad = pad_poll(0);

        if (pad & PAD_LEFT) 
			if (player.x > 48) player.x--;
        if (pad & PAD_RIGHT)
			if (player.x < 200) player.x++;

		oam_spr(player.x, player.y, 0, 0);

		oam_spr(box1.x, box1.y, 1, 0);
		oam_spr(box2.x, box2.y, 1, 0);
		oam_spr(box3.x, box3.y, 1, 0);
		oam_spr(box4.x, box4.y, 1, 0);
		oam_spr(box5.x, box5.y, 1, 0);
		

		collision1 = check_collision(&player, &box1);
		collision2 = check_collision(&player, &box2);
		collision3 = check_collision(&player, &box3);
		collision4 = check_collision(&player, &box4);
		collision5 = check_collision(&player, &box5);
		bottomCollision = check_collision(&player, &bottomBox);
		topCollision = check_collision(&player, &topBox);

		player.y = player.y + speed;

		if ((collision1 + collision2 + collision3 + collision4 + collision5)) {
			score = 0;
			speed = 1;
		}

		if (bottomCollision) {
				seed++;
				score++;
				player.y = 0;
			}

		switch (score)
		{
		case 0:
			oam_spr(0,0,0x0b,0);
			break;
		case 1:
			oam_spr(0,0,2,0);
			break;
		case 2:
			oam_spr(0,0,3,0);
			speed = 2;
			break;
		case 3:
			oam_spr(0,0,4,0);
			break;
		case 4:
			oam_spr(0,0,5,0);
			speed = 3;
			break;
		case 5:
			oam_spr(0,0,6,0);
			break;
		case 6:
			oam_spr(0,0,7,0);
			speed = 4;
			break;
		case 7:
			oam_spr(0,0,8,0);
			break;
		case 8:
			oam_spr(0,0,9,0);
			speed = 5;
			break;
		case 9:
			oam_spr(0,0,0x0a,0);
			break;
		case 0x0a:
			ppu_off();
		default:
			oam_spr(0,0,12,0);
			break;
		}

	}
}
	
	