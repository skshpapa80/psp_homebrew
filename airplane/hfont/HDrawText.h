#ifndef _PSPHFONT_H_
#define _PSPHFONT_H_ 1

struct {
	int iFontWidth;					// Font Width
	int iFontHeight;				// Font Height
	int iFToFSpaceWidth;		// Font ���� ���� Width
	int iFToFSpaceHeight;		// Font ���� ���� Height
	int iEngFontWidth;			// ���� Font�� Width
	int iEngFToFSpaceWidth;	// ���� Font ���� ���� Width
	int iPSPFontWidth;			// PSP �¿� font�� ����
	int iPSPFontHeight;			// PSP ���� font�� ����
} LFont[2];

void HFontInit();

int HDrawText(int x,int y,char *buf,int col,int backcol, int fill);
int HDrawTextLimit(int x,int y,char *buf,int col,int backcol, int fill, int offset, int length);

#endif
