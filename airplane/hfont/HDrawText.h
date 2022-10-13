#ifndef _PSPHFONT_H_
#define _PSPHFONT_H_ 1

struct {
	int iFontWidth;					// Font Width
	int iFontHeight;				// Font Height
	int iFToFSpaceWidth;		// Font 사이 공백 Width
	int iFToFSpaceHeight;		// Font 사이 공백 Height
	int iEngFontWidth;			// 영문 Font의 Width
	int iEngFToFSpaceWidth;	// 영문 Font 사이 공백 Width
	int iPSPFontWidth;			// PSP 좌우 font의 개수
	int iPSPFontHeight;			// PSP 상하 font의 개수
} LFont[2];

void HFontInit();

int HDrawText(int x,int y,char *buf,int col,int backcol, int fill);
int HDrawTextLimit(int x,int y,char *buf,int col,int backcol, int fill, int offset, int length);

#endif
