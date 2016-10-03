#include <HexWS2811z.h>
#include <usb_dev.h>

#define LED_BYTES 4
#define N_LEDS_PER_STRIP 60
#define N_BYTES_PER_STRIP (N_LEDS_PER_STRIP * LED_BYTES)
#define N_STRIPS 16
#define N_LEDS (N_LEDS_PER_STRIP * N_STRIPS)
#define N_BYTES (N_LEDS * LED_BYTES)

/*

import math, sys

n_dith = int(sys.argv[1])

def inv(i, n_bits):
        s = bin(i)[2:]
        s = list('0'*(n_bits-len(s)-1)+s)
        s.reverse()
        return int('0b'+''.join(s), 2)

def inv_list(n_dith):
        return [ inv(i, len(bin(n_dith))-2) for i in range(n_dith) ]

val = [ int(round(n_dith*255*(x/255.)**2.2)) for x in range(256) ]

table = [ [] for x in range(n_dith) ]

for total in val:
        had = 0.
        for i, x in enumerate( inv_list(n_dith) ):
                n = int( total/(n_dith-i) )
                total -= n
                table[x].append(n)

for t in table:
        print '\t' + ', '.join(str(x) for x in t) + ','
*/

uint8_t dith_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 89, 90, 91, 92, 94, 95, 96, 97, 99, 100, 101, 103, 104, 105, 107, 108, 110, 111, 112, 114, 115, 117, 118, 120, 121, 123, 124, 125, 127, 129, 130, 132, 133, 135, 136, 138, 139, 141, 143, 144, 146, 147, 149, 151, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 169, 171, 173, 175, 177, 178, 180, 182, 184, 186, 188, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 235, 237, 239, 242, 244, 246, 248, 250, 252, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 72, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 87, 88, 89, 90, 92, 93, 94, 95, 97, 98, 99, 101, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 125, 126, 127, 129, 131, 132, 134, 135, 137, 138, 140, 141, 143, 145, 146, 148, 150, 151, 153, 155, 156, 158, 160, 161, 163, 165, 166, 168, 170, 172, 174, 175, 177, 179, 181, 183, 184, 186, 188, 190, 192, 194, 196, 198, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 232, 234, 236, 238, 240, 242, 244, 246, 249, 251, 253, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 74, 75, 76, 77, 78, 79, 80, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 98, 99, 100, 102, 103, 104, 106, 107, 108, 110, 111, 113, 114, 116, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 136, 138, 140, 141, 143, 144, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 188, 190, 192, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 235, 238, 240, 242, 244, 246, 248, 250, 253, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 100, 101, 102, 104, 105, 106, 108, 109, 110, 112, 113, 115, 116, 117, 119, 120, 122, 123, 125, 126, 128, 129, 131, 132, 134, 135, 137, 139, 140, 142, 143, 145, 147, 148, 150, 151, 153, 155, 156, 158, 160, 162, 163, 165, 167, 168, 170, 172, 174, 176, 177, 179, 181, 183, 185, 187, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 207, 209, 211, 213, 215, 217, 219, 221, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 245, 247, 249, 251, 253, 255,
};

HexWS2811z *hex;

void scatter_bits(uint8_t *in, uint8_t *out)
{
	static int x=0;
	x+=1;

	int i;
	uint32_t *o32 = (uint32_t *)out;
	uint8_t *g;

	/* copied from fadecandy, firmware/fc_draw.cpp & adapted
     * to scatter 16x 8 bits instead of 8x 24 bits,
     * using a much simplefied dithering scheme
	 */
	union
	{
		uint32_t word;
		struct
		{
			uint32_t
				p0a:1, p1a:1, p2a:1, p3a:1, p4a:1, p5a:1, p6a:1, p7a:1,
				p8a:1, p9a:1, p10a:1, p11a:1, p12a:1, p13a:1, p14a:1, p15a:1,
				p0b:1, p1b:1, p2b:1, p3b:1, p4b:1, p5b:1, p6b:1, p7b:1,
				p8b:1, p9b:1, p10b:1, p11b:1, p12b:1, p13b:1, p14b:1, p15b:1;
		};
	} o0, o1, o2, o3;

	for (i=0; i<N_BYTES_PER_STRIP; i++)
	{
		g = &dith_table[((i+x)&0x3)<<8];
		uint32_t p0 = g[in[i]];
		o3.p0b = p0;
		o3.p0a = p0 >> 1;
		o2.p0b = p0 >> 2;
		o2.p0a = p0 >> 3;
		o1.p0b = p0 >> 4;
		o1.p0a = p0 >> 5;
		o0.p0b = p0 >> 6;
		o0.p0a = p0 >> 7;
		uint32_t p1 = g[in[i+N_BYTES_PER_STRIP]];
		o3.p1b = p1;
		o3.p1a = p1 >> 1;
		o2.p1b = p1 >> 2;
		o2.p1a = p1 >> 3;
		o1.p1b = p1 >> 4;
		o1.p1a = p1 >> 5;
		o0.p1b = p1 >> 6;
		o0.p1a = p1 >> 7;
		uint32_t p2 = g[in[i+N_BYTES_PER_STRIP*2]];
		o3.p2b = p2;
		o3.p2a = p2 >> 1;
		o2.p2b = p2 >> 2;
		o2.p2a = p2 >> 3;
		o1.p2b = p2 >> 4;
		o1.p2a = p2 >> 5;
		o0.p2b = p2 >> 6;
		o0.p2a = p2 >> 7;
		uint32_t p3 = g[in[i+N_BYTES_PER_STRIP*3]];
		o3.p3b = p3;
		o3.p3a = p3 >> 1;
		o2.p3b = p3 >> 2;
		o2.p3a = p3 >> 3;
		o1.p3b = p3 >> 4;
		o1.p3a = p3 >> 5;
		o0.p3b = p3 >> 6;
		o0.p3a = p3 >> 7;
		uint32_t p4 = g[in[i+N_BYTES_PER_STRIP*4]];
		o3.p4b = p4;
		o3.p4a = p4 >> 1;
		o2.p4b = p4 >> 2;
		o2.p4a = p4 >> 3;
		o1.p4b = p4 >> 4;
		o1.p4a = p4 >> 5;
		o0.p4b = p4 >> 6;
		o0.p4a = p4 >> 7;
		uint32_t p5 = g[in[i+N_BYTES_PER_STRIP*5]];
		o3.p5b = p5;
		o3.p5a = p5 >> 1;
		o2.p5b = p5 >> 2;
		o2.p5a = p5 >> 3;
		o1.p5b = p5 >> 4;
		o1.p5a = p5 >> 5;
		o0.p5b = p5 >> 6;
		o0.p5a = p5 >> 7;
		uint32_t p6 = g[in[i+N_BYTES_PER_STRIP*6]];
		o3.p6b = p6;
		o3.p6a = p6 >> 1;
		o2.p6b = p6 >> 2;
		o2.p6a = p6 >> 3;
		o1.p6b = p6 >> 4;
		o1.p6a = p6 >> 5;
		o0.p6b = p6 >> 6;
		o0.p6a = p6 >> 7;
		uint32_t p7 = g[in[i+N_BYTES_PER_STRIP*7]];
		o3.p7b = p7;
		o3.p7a = p7 >> 1;
		o2.p7b = p7 >> 2;
		o2.p7a = p7 >> 3;
		o1.p7b = p7 >> 4;
		o1.p7a = p7 >> 5;
		o0.p7b = p7 >> 6;
		o0.p7a = p7 >> 7;
		uint32_t p8 = g[in[i+N_BYTES_PER_STRIP*8]];
		o3.p8b = p8;
		o3.p8a = p8 >> 1;
		o2.p8b = p8 >> 2;
		o2.p8a = p8 >> 3;
		o1.p8b = p8 >> 4;
		o1.p8a = p8 >> 5;
		o0.p8b = p8 >> 6;
		o0.p8a = p8 >> 7;
		uint32_t p9 = g[in[i+N_BYTES_PER_STRIP*9]];
		o3.p9b = p9;
		o3.p9a = p9 >> 1;
		o2.p9b = p9 >> 2;
		o2.p9a = p9 >> 3;
		o1.p9b = p9 >> 4;
		o1.p9a = p9 >> 5;
		o0.p9b = p9 >> 6;
		o0.p9a = p9 >> 7;
		uint32_t p10 = g[in[i+N_BYTES_PER_STRIP*10]];
		o3.p10b = p10;
		o3.p10a = p10 >> 1;
		o2.p10b = p10 >> 2;
		o2.p10a = p10 >> 3;
		o1.p10b = p10 >> 4;
		o1.p10a = p10 >> 5;
		o0.p10b = p10 >> 6;
		o0.p10a = p10 >> 7;
		uint32_t p11 = g[in[i+N_BYTES_PER_STRIP*11]];
		o3.p11b = p11;
		o3.p11a = p11 >> 1;
		o2.p11b = p11 >> 2;
		o2.p11a = p11 >> 3;
		o1.p11b = p11 >> 4;
		o1.p11a = p11 >> 5;
		o0.p11b = p11 >> 6;
		o0.p11a = p11 >> 7;
		uint32_t p12 = g[in[i+N_BYTES_PER_STRIP*12]];
		o3.p12b = p12;
		o3.p12a = p12 >> 1;
		o2.p12b = p12 >> 2;
		o2.p12a = p12 >> 3;
		o1.p12b = p12 >> 4;
		o1.p12a = p12 >> 5;
		o0.p12b = p12 >> 6;
		o0.p12a = p12 >> 7;
		uint32_t p13 = g[in[i+N_BYTES_PER_STRIP*13]];
		o3.p13b = p13;
		o3.p13a = p13 >> 1;
		o2.p13b = p13 >> 2;
		o2.p13a = p13 >> 3;
		o1.p13b = p13 >> 4;
		o1.p13a = p13 >> 5;
		o0.p13b = p13 >> 6;
		o0.p13a = p13 >> 7;
		uint32_t p14 = g[in[i+N_BYTES_PER_STRIP*14]];
		o3.p14b = p14;
		o3.p14a = p14 >> 1;
		o2.p14b = p14 >> 2;
		o2.p14a = p14 >> 3;
		o1.p14b = p14 >> 4;
		o1.p14a = p14 >> 5;
		o0.p14b = p14 >> 6;
		o0.p14a = p14 >> 7;
		uint32_t p15 = g[in[i+N_BYTES_PER_STRIP*15]];
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

		handle_io();
    }

}

uint8_t io_buf1[N_BYTES];
uint8_t io_buf2[N_BYTES];
uint8_t io_buf3[N_BYTES];
uint8_t *draw_buf, *in_buf, *unused_buf;
unsigned int in_offset = 0;

void handle_io()
{
	usb_packet_t *rx_packet = usb_rx(CDC_RX_ENDPOINT);
	if (!rx_packet)
		return;

	for (int i=rx_packet->index; i<rx_packet->len; i++)
	{
		uint8_t c = rx_packet->buf[i];
		if (c != 1)
		{
			if (in_offset < N_BYTES)
			{
				in_buf[in_offset] = c;
				in_offset++;
			}
		}
		else
		{
			uint8_t *x = draw_buf;
			draw_buf = in_buf;
			in_buf = unused_buf;
			unused_buf = x;
			in_offset = 0;
		}
	}

	usb_free(rx_packet);
}

void setup()
{
	usb_init();
}

void loop()
{
	uint8_t buf1[N_BYTES];
	uint8_t buf2[N_BYTES];

    uint8_t *x, *old_frame = buf1, *new_frame = buf2;

	draw_buf = io_buf1;
	in_buf = io_buf2;
	unused_buf = io_buf3;
	memset(io_buf1, 0, N_BYTES);
	memset(io_buf2, 0, N_BYTES);
	memset(io_buf3, 0, N_BYTES);

    hex = new HexWS2811z(N_BYTES);
    hex->begin();

/*
int i=0;
uint32_t t0, t1=micros();
*/
    for (;;)
    {
        scatter_bits(draw_buf, new_frame);
        hex->show(new_frame);
        x=old_frame;
        old_frame = new_frame;
        new_frame = x;
/*
i++;
if (i==100)
{
	i=0;
	t0=t1;
	t1=micros();
	Serial.println(t1-t0);
}
*/
    }
}
