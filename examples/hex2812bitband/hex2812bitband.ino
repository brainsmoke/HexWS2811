#include <HexWS2811.h>

#define N_LEDS_PER_STRIP 60
#define N_STRIPS 16
#define N_LEDS (N_LEDS_PER_STRIP * N_STRIPS)
#define N_BITS (N_LEDS * 32)

uint8_t fade[] =
{
//[ int(((.5-cos(2*pi*x/240.)*.5)**2.2)*255) for x in range(240) ]
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,3,3,4,4,5,6,7,7,8,9,10,12,13,14,16,17,19,20,22,24,26,28,30,33,35,38,40,43,46,49,52,55,58,62,65,69,72,76,80,84,88,92,96,100,104,108,113,117,121,126,130,135,139,144,148,153,157,162,166,171,175,179,184,188,192,196,200,204,208,211,215,218,222,225,228,231,234,236,239,241,243,245,247,248,250,251,252,253,254,254,254,255,254,254,254,253,252,251,250,248,247,245,243,241,239,236,234,231,228,225,222,218,215,211,208,204,200,196,192,188,184,179,175,171,166,162,157,153,148,144,139,135,130,126,121,117,113,108,104,100,96,92,88,84,80,76,72,69,65,62,58,55,52,49,46,43,40,38,35,33,30,28,26,24,22,20,19,17,16,14,13,12,10,9,8,7,7,6,5,4,4,3,3,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

HexWS2811 *hex;


int r=0,g=80,b=160;
int ri,gi,bi;
uint32_t i,j;

void scatter_bits(uint8_t *in, uint8_t *out)
{
  uint32_t *bandout = (uint32_t *)( 0x22000000 + ((uint32_t)out-0x20000000)*32 );
  int i, j, n;
  uint32_t c;

  for (i=0,n=0; i<16; i++)
    for (j=i; j<N_BITS; j+=128,n++)
    {
      c = in[n];
      bandout[j] = c>>7;
      bandout[j+16] = c>>6;
      bandout[j+32] = c>>5;
      bandout[j+48] = c>>4;
      bandout[j+64] = c>>3;
      bandout[j+80] = c>>2;
      bandout[j+96] = c>>1;
      bandout[j+112] = c;
    }
}

void setup()
{
  Serial.begin(115200);
}

void loop() {

    uint8_t drawbuf[4 * N_LEDS];
    uint8_t dbuf[4 * N_LEDS];
    uint8_t framebuf[4 * N_LEDS];

    hex = new HexWS2811(N_LEDS_PER_STRIP, framebuf, dbuf, WS2811_GRBW);
    hex->begin();

    for (;;)
    {
        ri=r+=1;gi=g+=1;bi=b+=1;
        if (r >= 240) r-=240;
        if (g >= 240) g-=240;
        if (b >= 240) b-=240;

        for (i=0; i<N_LEDS; i++)
        {
            ri+=2; gi+=2; bi+=2;
            if (ri >= 240) ri-=240;
            if (gi >= 240) gi-=240;
            if (bi >= 240) bi-=240;
            drawbuf[i*3] = fade[ri];
            drawbuf[i*3+1] = fade[gi];
            drawbuf[i*3+2] = fade[bi];
        }

uint32_t t0 = micros();
        scatter_bits(drawbuf, dbuf);
uint32_t t1 = micros();
Serial.println(t1-t0);
        hex->show();
        yield();
    }
}
