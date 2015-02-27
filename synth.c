/*
 *	1 OSC Synth for the C64
 *	Filtering by paddle knob
 *

    Moog Like Synthesizer for the Commodore64 computer
    Copyright (C) 2001 Chris McCormick
    
    $Id$
    $Revision$

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *	Note: Compiled with Ullrich von Bassewitz cc65 compiler (great compiler, man!)
 *
 */

#include<stdio.h>
/* #define DEBUG */

/* Define nice macros for writing bytes out */
#define outb(addr,val)	(*(addr)) = (val)
#define inb(addr)	(*(addr))

/* The some memory addresses */
#define	SID	((unsigned char*)0xD400)
#define KEYBOARD ((unsigned char*)0x00c5) /* keyboard read address */
#define SYNC ((unsigned char*)0xDC01) /* paddle address which we use for syncing with the PC */

/* sid addresses */
/* OSC1 */
#define O1_FREQ_LO 0x00
#define O1_FREQ_HI 0x01
#define O1_PW_LO 0x02
#define O1_PW_HI 0x03
#define O1_CTRL 0x04
#define O1_AD 0x05
#define O1_SR 0x06 

/* OSC2 */
#define O2_FREQ_LO 0x07
#define O2_FREQ_HI 0x08
#define O2_PW_LO 0x09
#define O2_PW_HI 0x0A
#define O2_CTRL 0x0B
#define O2_AD 0x0C
#define O2_SR 0x0D 

/* OSC3 */
#define O3_FREQ_LO 0x0E
#define O3_FREQ_HI 0x0F
#define O3_PW_LO 0x10
#define O3_PW_HI 0x11
#define O3_CTRL 0x12
#define O3_AD 0x13
#define O3_SR 0x14

#define O3_FOLLOW 0x1B
#define O3_ENV 0x1C

/* control registers */
#define FC_HI 0x16
#define FC_LO 0x15

#define MODE_VOL 0x18
#define RES_FILT 0x17

#define PADDLE1 0x19
#define PADDLE2 0x1A

/* timer registers */

#define TIMER_B_LO ((unsigned char*)0xDC06)
#define TIMER_B_HI ((unsigned char*)0xDC07)
#define TIMER_B_CTRL ((unsigned char*)0xDC0F)

static unsigned long freq_table [12] =
	{ 34334, 36376, 38539, 40830, 43258, 45830, 48556, 51443, 54502, 57743, 61176, 64814 };
	
/* note key scancodes */
static unsigned char note_key [12] = 
	{ 12, 13, 23, 18, 20, 31, 26, 28, 29, 39, 34, 36 };

#define KEY_OCT_UP 44
#define KEY_OCT_DOWN 47

#define KEYS_OFF 64
#define KEY_Q 62
#define KEY_W 9
#define KEY_E 14
#define KEY_R 17
#define KEY_T 22
#define KEY_Y 25
#define KEY_U 30
#define KEY_P 41
#define KEY_I 33
#define KEY_O 38

/*****************************************************************************/
/*     	      	    	  	     Code	     			     */
/*****************************************************************************/

int main (void)
{
unsigned char key;				/* the current key state */
unsigned char lastkey;			/* the last key state */
unsigned char octave = 1;		/* what octave we are playing */
unsigned char counter = 0x00;
unsigned char mode = 0x00; 		/* stores the current control byte for osc 1 */

/* Initialize the SID so we can play something */
outb(SID + MODE_VOL, 0x9F);
outb(SID + RES_FILT, 0xFF);

/* start the first oscillator going (OSC 2) */	
outb(SID + O1_AD, 0x00);
outb(SID + O1_SR, 0xF0);
outb(SID + O1_FREQ_LO, 191);
outb(SID + O1_FREQ_HI, 44);
outb(SID + O1_PW_LO, 127);
outb(SID + O1_CTRL, 0x11);

while(1)
	{
	/* set the current filter cutoff value to be the current paddle value */
	outb(SID + FC_HI, inb(SID + PADDLE1));

	/* get a new keyboard scancode value */
	lastkey = key;
	key = inb(KEYBOARD);
	
	/* if the key state has changed */
	if (lastkey != key)
		{
		/* if the key state is 'no key' */
		if (key == KEYS_OFF)
			{
			/* make the sid stop OSC 1 */
			outb(SID + O1_CTRL, mode);
			outb(SID + O1_FREQ_HI, 0x00);
			outb(SID + O1_FREQ_LO, 0x00);
			}

		/* if key pressed is 'octave down' */
		if ((key == KEY_OCT_DOWN) & (octave < 8))
			octave++;

		/* if key pressed is 'octave up' */
		if ((key == KEY_OCT_UP) & (octave > 1))
			octave--;

		/* if they press the filter type keys */
		if (key == KEY_Q)
			{
			outb(SID + MODE_VOL, 0x0F | 0x10 | 0x80);
			}

		if (key == KEY_W)
			{
			outb(SID + MODE_VOL, 0x0F | 0x20 | 0x80);
			}
		
		if (key == KEY_E)
			{
			outb(SID + MODE_VOL, 0x0F | 0x40 | 0x80);
			}

		/* the wave form keys */
		if (key == KEY_R) /* triangle */
			{
			mode = 0x10;
			}

		if (key == KEY_T) /* saw */
			{
			mode = 0x20;
			}

		if (key == KEY_Y) /* pulse */
			{
		 	mode = 0x40;
			}

		if (key == KEY_U) /* noise */
			{
			mode = 0x80;
			}

		/* go through each possible note key */
		for (counter = 0; counter < 12; counter++)
			{
			/* if the key pressed is a note key */
			if (key == note_key[counter])
				{
				/* make the frequency values correct */
				outb(SID + O1_FREQ_HI, (((freq_table[counter]) >> 8) >> octave));
				outb(SID + O1_FREQ_LO, ((freq_table[counter]) >> octave));
				/* make the sid play OSC 1 */
				outb(SID + O1_CTRL, mode | 0x01);
				}
			}
		}
	}
return 0;
}



