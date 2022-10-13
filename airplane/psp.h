
#define BUF_WIDTH          (512)
#define SCR_WIDTH          (480)
#define SCR_HEIGHT         (272)

#define PSP_LINE_SIZE      512
#define PSP_PIXEL_FORMAT   1
#define	PSP_FRAME_SIZE     0x44000

#define VRAM_ADDR          ((char *)0x04000000)

void pspInit(int mode);
void pspInputInit();

void pspFillvram(unsigned long color);
void pspBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d);
char *pspGetVramAddr(unsigned long x,unsigned long y);
void pspScreenFlip();
void screenshot(const char* filename);

