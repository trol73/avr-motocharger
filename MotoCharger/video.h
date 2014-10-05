/*
 * video.h
 *
 * Created: 02.10.2014 22:21:54
 *  Author: Trol
 */ 


#ifndef VIDEO_H_
#define VIDEO_H_

/************************************************************************/
/* Video memory has 2 pages with 2x16 chars                             */
/************************************************************************/
char video_memory[2*16*2];

/************************************************************************/
/* Offset of the current page data in video memory   (can be 0 or 32)   */
/************************************************************************/
uint8_t video_currentPageOffset;

/************************************************************************/
/* Current position on the screen (0..31)                               */
/************************************************************************/
uint8_t video_pos;


/************************************************************************/
/* Set current cursor position                                          */
/************************************************************************/
void video_GotoXY(uint8_t x, uint8_t y) {
	video_pos = 16*y + x + video_currentPageOffset;
}

/************************************************************************/
/* Clean up video memory                                                */
/************************************************************************/
void video_Reset() {
	video_currentPageOffset = 0;
	video_pos = 0;
	for (uint8_t i = 0; i < 32*2; i++) {
		video_memory[i] = ' ';
	}
	LCD_Clear();
}


/************************************************************************/
/* Очищает экран и переводит курсов в начальную позицию                 */
/************************************************************************/
void video_Clear() {
	for (uint8_t i = 0; i < 32; i++) {
		video_memory[video_currentPageOffset+i] = ' ';
	}
	video_GotoXY(0, 0);
}

/************************************************************************/
/* Выводит символ в текущую позицию курсора                             */
/************************************************************************/
void video_Write(char ch) {
	video_memory[video_pos++] = ch;
}

/************************************************************************/
/* Write string to current position                                     */
/************************************************************************/
void video_WriteStr(PGM_P str) {
	for (;;) {
		uint8_t c = pgm_read_byte_near(str++);
		if (c == 0) {
			break;
		}
		video_Write(c);
	}
}

/************************************************************************/
/* Print a float                                                        */
/* digits_mask - defines number of digits (1 for one-digits number,		*/
/*		10 - two digits, 100 - for three-digits etc.)					*/
/* digits_whole - number of digits in integral part						*/
/************************************************************************/
void video_WriteFloat(uint16_t val, uint16_t digits_mask, uint8_t digits_whole) {
	for (uint8_t i = 0; ; i++) {
		if (i == digits_whole) {
			video_Write('.');
		}
		video_Write('0' + val / digits_mask);
		val %= digits_mask;
		if (digits_mask == 1) {
			break;
		}
		digits_mask /= 10;
	}
}


void video_WriteUInt8(uint8_t val) {
	uint8_t mask = 100;
	bool firstNotZerro = false;
	while (true) {
		uint8_t i = val / mask;
		if (i != 0 || firstNotZerro || mask == 1) {
			video_Write('0' + i);
		}
		if (i != 0) {
			firstNotZerro = true;
		}
		val %= mask;
		mask /= 10;
		if (mask == 0) {
			break;
		}
	}
}


/************************************************************************/
/* Copy video memory from current page to display and swap pages        */
/************************************************************************/
void video_Repaint() {
	uint8_t otherViewPageOffset = 32 - video_currentPageOffset;

	// compare memory pages data: first line
	bool changed = false;
	for (uint8_t i = 0; i < 16; i++) {
		if (video_memory[i] != video_memory[i+32]) {
			changed = true;
			video_memory[otherViewPageOffset+i] = video_memory[video_currentPageOffset+i];
		}
	}
	if (changed) {
		LCD_GotoXY(0, 0);
		LCD_PrintLine(&video_memory[video_currentPageOffset]);
	}
	// compare memory pages data: second line
	changed = false;
	for (uint8_t i = 16; i < 32; i++) {
		if (video_memory[i] != video_memory[i+32]) {
			changed = true;
			video_memory[otherViewPageOffset+i] = video_memory[video_currentPageOffset+i];
		}
	}
	if (changed) {
		LCD_GotoXY(0, 1);
		LCD_PrintLine(&video_memory[video_currentPageOffset]+16);
	}
	// swap pages
	video_currentPageOffset = otherViewPageOffset;
}



#endif /* VIDEO_H_ */