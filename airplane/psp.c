//////////////////////////////////////////////////////////////////////////////////////////
//   PSP Lib 
//   선경선향아빠의 블로그
//   https://skshpapa80.blogspot.kr/
//////////////////////////////////////////////////////////////////////////////////////////

#include <pspkernel.h>

#include <pspdisplay.h>
#include <pspctrl.h>
#include <png.h>
#include "psp.h"

long pg_showframe;
long pg_drawframe=0;

void pspInit(int mode)
{
	u16 *VRAM=(void *)(0x44000000);

	sceDisplaySetMode(mode,SCR_WIDTH,SCR_HEIGHT); // Sets the display mode.
    sceDisplaySetFrameBuf(VRAM,BUF_WIDTH,1,1); // Sets the address of the frame buffer.
}

void pspInputInit()
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

char *pspGetVramAddr(unsigned long x,unsigned long y)
{
	return VRAM_ADDR+pg_drawframe*PSP_FRAME_SIZE+x*PSP_PIXEL_FORMAT*2+y*PSP_LINE_SIZE*2+0x40000000;
}

void pspFillvram(unsigned long color)
{
	unsigned char *vptr0; //pointer to vram
	unsigned long i;

	vptr0=pspGetVramAddr(0,0);
	for (i=0; i<PSP_FRAME_SIZE/2; i++) {
		*(unsigned short *)vptr0=color;
		vptr0+=PSP_PIXEL_FORMAT*2;
	}
}

void pspBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d)
{
	unsigned char *vptr0; //pointer to vram
	unsigned char *vptr;  //pointer to vram
	unsigned long xx,yy,mx,my;
	const unsigned short *dd;

	vptr0=pspGetVramAddr(x,y);
	for (yy=0; yy<h; yy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			dd=d;
			for (xx=0; xx<w; xx++) {
				for (mx=0; mx<mag; mx++) {
					*(unsigned short *)vptr=*dd;
					vptr+=PSP_PIXEL_FORMAT*2;
				}
				dd++;
			}
			vptr0+=PSP_LINE_SIZE*2;
		}
		d+=w;
	}

}

void pspScreenFlip()
{
	pg_showframe=pg_drawframe;
	pg_drawframe++;
	pg_drawframe&=1;	
	sceDisplaySetFrameBuf(VRAM_ADDR+pg_showframe*PSP_FRAME_SIZE,PSP_LINE_SIZE,PSP_PIXEL_FORMAT,0);
}

// Saves a screenshot of the screen to filename.
void screenshot(const char* filename)
{ 
   png_structp png_ptr;
   png_infop info_ptr; 
   FILE* fp; 
   int i, x, y;
   u16* data = VRAM_ADDR;
   int width=SCR_WIDTH;
   int height=SCR_HEIGHT;
   int linesize=BUF_WIDTH;
   int saveAlpha=0;
   u8* line;

   if ((fp = fopen(filename, "wb")) == NULL) return;
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
   if (!png_ptr) return;
   info_ptr = png_create_info_struct(png_ptr); 
   if (!info_ptr) { 
      png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
      return;
   } 
   png_init_io(png_ptr, fp);
   png_set_IHDR(png_ptr, info_ptr, width, height, 8,
      saveAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB, 
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 
   png_write_info(png_ptr, info_ptr); 
   line = (u8*) malloc(width * (saveAlpha ? 4 : 3)); 
   for (y = 0; y < height; y++) { 
      for (i = 0, x = 0; x < width; x++) { 
         u16 color = data[x + y * linesize];
         int r = (color & 0x1f) << 3; 
         int g = ((color >> 5) & 0x1f) << 3 ; 
         int b = ((color >> 10) & 0x1f) << 3 ; 
         line[i++] = r; 
         line[i++] = g; 
         line[i++] = b; 
         if (saveAlpha) { 
            int a = color & 0x8000 ? 0xff : 0; 
            line[i++] = a; 
         } 
      } 
      png_write_row(png_ptr, line); 
   } 
   free(line); 
   png_write_end(png_ptr, info_ptr); 
   png_destroy_write_struct(&png_ptr, (png_infopp)NULL); 
   fclose(fp); 
}
