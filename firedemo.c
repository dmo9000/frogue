#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH	640
#define DISPLAY_HEIGHT 384 

#define FIRE_WIDTH 640 
#define FIRE_HEIGHT 384 

//#define FIRE_WIDTH (64*2)
//#define FIRE_HEIGHT (38 *2)

extern uint8_t *screenData;

typedef struct _rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

uint8_t firePixels[FIRE_WIDTH * FIRE_HEIGHT];

int y_scrolling = 440;

RGB colors[37]= {
    { 0x07,0x07,0x07 },
    { 0x1F,0x07,0x07 },
    { 0x2F,0x0F,0x07 },
    { 0x47,0x0F,0x07 },
    { 0x57,0x17,0x07 },
    { 0x67,0x1F,0x07 },
    { 0x77,0x1F,0x07 },
    { 0x8F,0x27,0x07 },
    { 0x9F,0x2F,0x07 },
    { 0xAF,0x3F,0x07 },
    { 0xBF,0x47,0x07 },
    { 0xC7,0x47,0x07 },
    { 0xDF,0x4F,0x07 },
    { 0xDF,0x57,0x07 },
    { 0xDF,0x57,0x07 },
    { 0xD7,0x5F,0x07 },
    { 0xD7,0x5F,0x07 },
    { 0xD7,0x67,0x0F },
    { 0xCF,0x6F,0x0F },
    { 0xCF,0x77,0x0F },
    { 0xCF,0x7F,0x0F },
    { 0xCF,0x87,0x17 },
    { 0xC7,0x87,0x17 },
    { 0xC7,0x8F,0x17 },
    { 0xC7,0x97,0x1F },
    { 0xBF,0x9F,0x1F },
    { 0xBF,0x9F,0x1F },
    { 0xBF,0xA7,0x27 },
    { 0xBF,0xA7,0x27 },
    { 0xBF,0xAF,0x2F },
    { 0xB7,0xAF,0x2F },
    { 0xB7,0xB7,0x2F },
    { 0xB7,0xB7,0x37 },
    { 0xCF,0xCF,0x6F },
    { 0xDF,0xDF,0x9F },
    { 0xEF,0xEF,0xC7 },
    { 0xFF,0xFF,0xFF }
};

int grx_setpixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b)
{
//  unsigned char *p = (unsigned char *) screenData; 
  unsigned char *p = screenData; 

//	printf("x =%u, y=%u, p=0x%lx\n", x, y, p);

  p += (y * DISPLAY_WIDTH * 3);
  p += (x * 3);

  p[0] = r;
  p[1] = g;
  p[2] = b;

  return 0;
}

int grx_bufferswap()
{
	ansitty_canvas_setdirty(true);
	return 0;
}


void fireMain(void)
{

		printf("fireMain()\n");
    setup();
    while (1) {
        hostFrame();
        grx_bufferswap();
    }
    exit(1);
}


int setup()
{

    int i = 0, j = 0;

    for (i = 0 ; i < FIRE_HEIGHT*FIRE_WIDTH; i++) {
        firePixels[i] = 0;
    }

    for (i = 0; i < FIRE_WIDTH; i++) {
        firePixels[(FIRE_HEIGHT-1)*FIRE_WIDTH + i] = 36;
    }
    y_scrolling = 440;

    return 0;

}

int spreadFire(uint32_t src) {
    uint8_t pixel = firePixels[src];
    if(pixel == 0) {
        firePixels[src - FIRE_WIDTH] = 0;
    } else {
        uint32_t randIdx = (uint32_t) round(random() % 3) & (uint32_t )3;
        uint32_t  dst = src - randIdx + 1;
        firePixels[dst - FIRE_WIDTH ] = pixel - (randIdx & 1);
    }
	return;
}

int doFire()
{
    int x = 0;
    int y = 0;
    for(x=0 ; x < FIRE_WIDTH; x++) {
        for (y = 1; y < FIRE_HEIGHT; y++) {
            spreadFire(y * FIRE_WIDTH + x);
        }
    }
	return 0;
}



int hostFrame()
{
    int x = 0;
    int y = 0;
    doFire();

    for(y = 0; y < FIRE_HEIGHT;  y++) {
        for(x = 0; x < FIRE_WIDTH; x++) {
            uint8_t index = firePixels[y * FIRE_WIDTH + x];
            grx_setpixel(x, y, colors[index].r, colors[index].g, colors[index].b);
        }
    }
    return 0;
}


