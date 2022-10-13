#include <psptypes.h>
#include <pspdebug.h>
#include <string.h>

#include "font_h11.c"
#include "font_e11.c"

#include "HDrawText.h"

static u32* g_vram_base = (u32 *) 0x04000000;

void HFontInit() {
  LFont[0].iFontWidth = 13;				// Font Width
  LFont[0].iFontHeight = 13;			// Font Height
  LFont[0].iFToFSpaceWidth = 3;		// Font 사이 공백 Width
  LFont[0].iFToFSpaceHeight = 1;	// Font 사이 공백 Height
  LFont[0].iEngFontWidth = 7;			// 영문 Font의 Width
  LFont[0].iEngFToFSpaceWidth = 1;// 영문 Font 사이 공백 Width
  LFont[0].iPSPFontWidth = 60;		// PSP 좌우 font의 개수
  LFont[0].iPSPFontHeight = 19;		// PSP 상하 font의 개수
  
  LFont[1].iFontWidth = 11;				// Font Width
  LFont[1].iFontHeight = 12;			// Font Height
  LFont[1].iFToFSpaceWidth = 1;		// Font 사이 공백 Width
  LFont[1].iFToFSpaceHeight = 0;	// Font 사이 공백 Height
  LFont[1].iEngFontWidth = 6;			// 영문 Font의 Width
  LFont[1].iEngFToFSpaceWidth = 0;// 영문 Font 사이 공백 Width
  LFont[1].iPSPFontWidth = 79;		// PSP 좌우 font의 개수
  LFont[1].iPSPFontHeight = 22;		// PSP 상하 font의 개수
}

int font_size_index = 1;

unsigned short *GetHangulFont(unsigned char u,unsigned char d) {
	
	unsigned short *fnt = 0x00;
	
	// mapping
	d -= 0xA1; u -= 0xB0;
	
	if (font_size_index==0) {
//		fnt = (unsigned short *)&hangle_font_13[((u*0x5e)+d)*  LFont[font_size_index].iFontHeight];
	} else {
		fnt = (unsigned short *)&hangle_font_11[((u*0x5e)+d)*  LFont[font_size_index].iFontHeight];
	}
	
	return fnt;
}

unsigned char *GetEnglishFont(unsigned char ch) {
	
	unsigned char  *fnt = 0x00;
	
	// mapping
	if (ch<0x20) {
		ch = 0;
	} else if (ch<0x80) {
		ch -= 0x20;
	} else if (ch<0xa0) {
		ch = 0;
	} else {
		ch -= 0x40;
	}
	if (font_size_index==0) {
//		fnt = (unsigned char *)&english_font_13[ch*LFont[font_size_index].iFontHeight];
	} else {
		fnt = (unsigned char *)&english_font_11[ch*LFont[font_size_index].iFontHeight];
	}
	
	return fnt;
}

// ---------------------------------------------------------------------------------

#define PSP_LINE_SIZE 512

void Draw_Char_Hangul(int x,int y,unsigned char u,unsigned char d,int color,int bg_col) {
	
	int 	i,j;
	unsigned short *fnt;
	u32  pixel;

	u32 *vram;
	u32 *vram_ptr;

	vram = g_vram_base + x;
	vram += (y * PSP_LINE_SIZE);
	
	fnt = GetHangulFont(u, d);	
	
	for (i=0; i<LFont[font_size_index].iFontHeight; i++, fnt++) {
		
		vram_ptr  = vram;
		
		for (j=0; j<LFont[font_size_index].iFontWidth; j++) {
			
			if ((*fnt & (65536 >> j)))
			  pixel = color;
			else
			  pixel = bg_col;
			  
			*vram_ptr++ = pixel; 
		}
		
    vram += PSP_LINE_SIZE;
  }
}

void Draw_Char_English(int x,int y,unsigned char ch,int color,int bg_col) {
	
	int 	i,j;
	unsigned char  *fnt;
	u32  pixel;

	u32 *vram;
	u32 *vram_ptr;
	
  vram = g_vram_base + x;
	vram += (y * PSP_LINE_SIZE);
	
	fnt = GetEnglishFont(ch);
	
	for (i=0; i<LFont[font_size_index].iFontHeight; i++, fnt++) {
		
		vram_ptr  = vram;
		for (j=0; j<LFont[font_size_index].iEngFontWidth; j++) {
			
			if ((*fnt & (128 >> j)))
			  pixel = color;
			else
			  pixel = bg_col;
			  
			*vram_ptr++ = pixel; 
		}
    vram += PSP_LINE_SIZE;
  }
}

int HDrawText(int x,int y,char *buf,int col,int backcol, int fill) {
	return HDrawTextLimit(x, y, buf, col, backcol, fill, 0, strlen(buf));
}

int HDrawTextLimit(int x,int y,char *buf,int col,int backcol, int fill, int offset, int length) {
	unsigned char ch = 0,bef = 0;
	int pr_index = 0;
	unsigned char *str;
	
	str = &buf[offset];

	while(*str != 0) {
		ch = *str++;
		pr_index++;
		if (bef!=0) {
			if (bef==0x88 && ch==0x81) {
			  
			} else {
				Draw_Char_Hangul(x,y,bef,ch,col,backcol);
				x+=(LFont[font_size_index].iFontWidth+LFont[font_size_index].iFToFSpaceWidth);
			}
			
			bef=0;
		} else {
			if (ch==0x0d) {
				ch = *str++;
				if (ch==0x0a) {
					pr_index++;
				}
				break;
			} else if (ch==0x0a) {
				break;
			} else if (((ch>=0x80) && (ch<0xa0)) || (ch>=0xB0)) {
				bef = ch;
			} else {
				Draw_Char_English(x,y,ch,col,backcol);
				x+=(LFont[font_size_index].iEngFontWidth+LFont[font_size_index].iEngFToFSpaceWidth);
			}
		}
		if (pr_index>=length) break;
	}
	
	if (bef!=0) {
		pr_index--;
	}
	
	return pr_index;
}
