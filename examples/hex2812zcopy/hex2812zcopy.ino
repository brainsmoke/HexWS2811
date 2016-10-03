#include <HexWS2811z.h>


#define LED_BYTES 3
#define N_LEDS_PER_STRIP 80
#define N_BYTES_PER_STRIP (N_LEDS_PER_STRIP * LED_BYTES)
#define N_STRIPS 16
#define N_LEDS (N_LEDS_PER_STRIP * N_STRIPS)
#define N_BYTES (N_LEDS * LED_BYTES)
#define N_BITS (N_LEDS * LED_BYTES * 8)

uint8_t fade[] =
{
//[ int(((.5-cos(2*pi*x/240.)*.5)**2.2)*255) for x in range(240) ]
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,3,3,4,4,5,6,7,7,8,9,10,12,13,14,16,17,19,20,22,24,26,28,30,33,35,38,40,43,46,49,52,55,58,62,65,69,72,76,80,84,88,92,96,100,104,108,113,117,121,126,130,135,139,144,148,153,157,162,166,171,175,179,184,188,192,196,200,204,208,211,215,218,222,225,228,231,234,236,239,241,243,245,247,248,250,251,252,253,254,254,254,255,254,254,254,253,252,251,250,248,247,245,243,241,239,236,234,231,228,225,222,218,215,211,208,204,200,196,192,188,184,179,175,171,166,162,157,153,148,144,139,135,130,126,121,117,113,108,104,100,96,92,88,84,80,76,72,69,65,62,58,55,52,49,46,43,40,38,35,33,30,28,26,24,22,20,19,17,16,14,13,12,10,9,8,7,7,6,5,4,4,3,3,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

HexWS2811z *hex;


int r=0,g=80,b=160;
int ri,gi,bi;
uint32_t i,j;

void scatter_bits_old(uint8_t *in, uint8_t *out)
{
  uint32_t *bandout = (uint32_t *)( 0x22000000 + ((uint32_t)out-0x20000000)*32 );
  int i, j, n;
uint32_t *i32 = (uint32_t *)in;
  uint32_t c;

  for (i=0,n=0; i<16; i++)
    for (j=i; j<N_BITS; j+=512,n++)
    {
      c = i32[n];
      bandout[j] = c>>7;
      bandout[j+16] = c>>6;
      bandout[j+32] = c>>5;
      bandout[j+48] = c>>4;
      bandout[j+64] = c>>3;
      bandout[j+80] = c>>2;
      bandout[j+96] = c>>1;
      bandout[j+112] = c;
      bandout[j+128] = c>>15;
      bandout[j+144] = c>>14;
      bandout[j+160] = c>>13;
      bandout[j+176] = c>>12;
      bandout[j+192] = c>>11;
      bandout[j+208] = c>>10;
      bandout[j+224] = c>>9;
      bandout[j+240] = c>>8;
      bandout[j+256] = c>>23;
      bandout[j+272] = c>>22;
      bandout[j+288] = c>>21;
      bandout[j+304] = c>>20;
      bandout[j+320] = c>>19;
      bandout[j+336] = c>>18;
      bandout[j+352] = c>>17;
      bandout[j+368] = c>>16;
      bandout[j+384] = c>>31;
      bandout[j+400] = c>>30;
      bandout[j+416] = c>>29;
      bandout[j+432] = c>>28;
      bandout[j+448] = c>>27;
      bandout[j+464] = c>>26;
      bandout[j+480] = c>>25;
      bandout[j+496] = c>>24;
    }
}

void scatter_bits(uint8_t *in, uint8_t *out)
{
  int i;
uint32_t *o32 = (uint32_t *)out;

      union {
            uint32_t word;
            struct {
                uint32_t
                p0a:1, p1a:1, p2a:1, p3a:1, p4a:1, p5a:1, p6a:1, p7a:1,
                p8a:1, p9a:1, p10a:1, p11a:1, p12a:1, p13a:1, p14a:1, p15a:1,
                p0b:1, p1b:1, p2b:1, p3b:1, p4b:1, p5b:1, p6b:1, p7b:1,
                p8b:1, p9b:1, p10b:1, p11b:1, p12b:1, p13b:1, p14b:1, p15b:1;
            };
        } o0, o1, o2, o3;

for (i=0; i<N_BYTES_PER_STRIP; i++)
    {
      uint32_t p0 = in[i];
      o3.p0b = p0;
      o3.p0a = p0 >> 1;
      o2.p0b = p0 >> 2;
      o2.p0a = p0 >> 3;
      o1.p0b = p0 >> 4;
      o1.p0a = p0 >> 5;
      o0.p0b = p0 >> 6;
      o0.p0a = p0 >> 7;
      uint32_t p1 = in[i+N_BYTES_PER_STRIP];
      o3.p1b = p1;
      o3.p1a = p1 >> 1;
      o2.p1b = p1 >> 2;
      o2.p1a = p1 >> 3;
      o1.p1b = p1 >> 4;
      o1.p1a = p1 >> 5;
      o0.p1b = p1 >> 6;
      o0.p1a = p1 >> 7;
      uint32_t p2 = in[i+N_BYTES_PER_STRIP*2];
      o3.p2b = p2;
      o3.p2a = p2 >> 1;
      o2.p2b = p2 >> 2;
      o2.p2a = p2 >> 3;
      o1.p2b = p2 >> 4;
      o1.p2a = p2 >> 5;
      o0.p2b = p2 >> 6;
      o0.p2a = p2 >> 7;
      uint32_t p3 = in[i+N_BYTES_PER_STRIP*3];
      o3.p3b = p3;
      o3.p3a = p3 >> 1;
      o2.p3b = p3 >> 2;
      o2.p3a = p3 >> 3;
      o1.p3b = p3 >> 4;
      o1.p3a = p3 >> 5;
      o0.p3b = p3 >> 6;
      o0.p3a = p3 >> 7;
      uint32_t p4 = in[i+N_BYTES_PER_STRIP*4];
      o3.p4b = p4;
      o3.p4a = p4 >> 1;
      o2.p4b = p4 >> 2;
      o2.p4a = p4 >> 3;
      o1.p4b = p4 >> 4;
      o1.p4a = p4 >> 5;
      o0.p4b = p4 >> 6;
      o0.p4a = p4 >> 7;
      uint32_t p5 = in[i+N_BYTES_PER_STRIP*5];
      o3.p5b = p5;
      o3.p5a = p5 >> 1;
      o2.p5b = p5 >> 2;
      o2.p5a = p5 >> 3;
      o1.p5b = p5 >> 4;
      o1.p5a = p5 >> 5;
      o0.p5b = p5 >> 6;
      o0.p5a = p5 >> 7;
      uint32_t p6 = in[i+N_BYTES_PER_STRIP*6];
      o3.p6b = p6;
      o3.p6a = p6 >> 1;
      o2.p6b = p6 >> 2;
      o2.p6a = p6 >> 3;
      o1.p6b = p6 >> 4;
      o1.p6a = p6 >> 5;
      o0.p6b = p6 >> 6;
      o0.p6a = p6 >> 7;
      uint32_t p7 = in[i+N_BYTES_PER_STRIP*7];
      o3.p7b = p7;
      o3.p7a = p7 >> 1;
      o2.p7b = p7 >> 2;
      o2.p7a = p7 >> 3;
      o1.p7b = p7 >> 4;
      o1.p7a = p7 >> 5;
      o0.p7b = p7 >> 6;
      o0.p7a = p7 >> 7;
      uint32_t p8 = in[i+N_BYTES_PER_STRIP*8];
      o3.p8b = p8;
      o3.p8a = p8 >> 1;
      o2.p8b = p8 >> 2;
      o2.p8a = p8 >> 3;
      o1.p8b = p8 >> 4;
      o1.p8a = p8 >> 5;
      o0.p8b = p8 >> 6;
      o0.p8a = p8 >> 7;
      uint32_t p9 = in[i+N_BYTES_PER_STRIP*9];
      o3.p9b = p9;
      o3.p9a = p9 >> 1;
      o2.p9b = p9 >> 2;
      o2.p9a = p9 >> 3;
      o1.p9b = p9 >> 4;
      o1.p9a = p9 >> 5;
      o0.p9b = p9 >> 6;
      o0.p9a = p9 >> 7;
      uint32_t p10 = in[i+N_BYTES_PER_STRIP*10];
      o3.p10b = p10;
      o3.p10a = p10 >> 1;
      o2.p10b = p10 >> 2;
      o2.p10a = p10 >> 3;
      o1.p10b = p10 >> 4;
      o1.p10a = p10 >> 5;
      o0.p10b = p10 >> 6;
      o0.p10a = p10 >> 7;
      uint32_t p11 = in[i+N_BYTES_PER_STRIP*11];
      o3.p11b = p11;
      o3.p11a = p11 >> 1;
      o2.p11b = p11 >> 2;
      o2.p11a = p11 >> 3;
      o1.p11b = p11 >> 4;
      o1.p11a = p11 >> 5;
      o0.p11b = p11 >> 6;
      o0.p11a = p11 >> 7;
      uint32_t p12 = in[i+N_BYTES_PER_STRIP*12];
      o3.p12b = p12;
      o3.p12a = p12 >> 1;
      o2.p12b = p12 >> 2;
      o2.p12a = p12 >> 3;
      o1.p12b = p12 >> 4;
      o1.p12a = p12 >> 5;
      o0.p12b = p12 >> 6;
      o0.p12a = p12 >> 7;
      uint32_t p13 = in[i+N_BYTES_PER_STRIP*13];
      o3.p13b = p13;
      o3.p13a = p13 >> 1;
      o2.p13b = p13 >> 2;
      o2.p13a = p13 >> 3;
      o1.p13b = p13 >> 4;
      o1.p13a = p13 >> 5;
      o0.p13b = p13 >> 6;
      o0.p13a = p13 >> 7;
      uint32_t p14 = in[i+N_BYTES_PER_STRIP*14];
      o3.p14b = p14;
      o3.p14a = p14 >> 1;
      o2.p14b = p14 >> 2;
      o2.p14a = p14 >> 3;
      o1.p14b = p14 >> 4;
      o1.p14a = p14 >> 5;
      o0.p14b = p14 >> 6;
      o0.p14a = p14 >> 7;
      uint32_t p15 = in[i+N_BYTES_PER_STRIP*15];
      o3.p15b = p15;
      o3.p15a = p15 >> 1;
      o2.p15b = p15 >> 2;
      o2.p15a = p15 >> 3;
      o1.p15b = p15 >> 4;
      o1.p15a = p15 >> 5;
      o0.p15b = p15 >> 6;
      o0.p15a = p15 >> 7;

      *(o32++) = o0.word;
      *(o32++) = o1.word;
      *(o32++) = o2.word;
      *(o32++) = o3.word;
if (Serial.available()) Serial.read();
    }

}

void setup()
{
  Serial.begin(115200);
}

uint8_t drawbuf[N_BYTES];

void loop() {

    uint32_t t0, t1;

uint8_t buf1[N_BYTES];
uint8_t buf2[N_BYTES];
    uint8_t *x, *old_frame = buf1, *new_frame = buf2;

    hex = new HexWS2811z(N_BYTES);
    hex->begin();

    for (;;)
    {
        ri=r+=1;gi=g+=1;bi=b+=1;
        if (r >= 240) r-=240;
        if (g >= 240) g-=240;
        if (b >= 240) b-=240;

        for (i=0; i<N_BYTES; i+=LED_BYTES)
        {
            ri+=2; gi+=2; bi+=2;
            if (ri >= 240) ri-=240;
            if (gi >= 240) gi-=240;
            if (bi >= 240) bi-=240;
            drawbuf[i] = fade[ri];
            drawbuf[i+1] = fade[gi];
            drawbuf[i+2] = fade[bi];
        }
        
t0 = micros();
        scatter_bits(drawbuf, new_frame);
t1 = micros();
Serial.println(t1-t0);
        hex->show(new_frame);
        x=old_frame;
        old_frame = new_frame;
        new_frame = x;
        yield();
    }
}
