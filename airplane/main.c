/*
	PSP AirPlane
	선경선향아빠의 블로그
	https://skshpapa80.blogspot.kr/
*/
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "psp.h"
#include "hfont/HDrawText.h"
#include "airplane.h"

PSP_MODULE_INFO("PSP Airplane", 0x1000, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int done = 0;

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	done = 1;
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread,
				     0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

int main(int argc, char *argv[])
{
	SceCtrlData ctl;
	
	// 한글 Font 로딩
	HFontInit();

	SetupCallbacks();
	pspInit(1);
	pspInputInit();

	int x,y;
	x = 200;
	y = 200;
	while(!done)
	{
		sceCtrlReadBufferPositive(&ctl, 1); 

		pspFillvram(0x00);
		HDrawText(6, 15, "선경선향아빠 PSP Homebrew #1", 0xFFFFFFFF, 0x00000000, 0);
		pspBitBlt(x,y,airplaneWidth,airplaneHeight,1,airplane);
		pspScreenFlip();
		
		if (ctl.Buttons & PSP_CTRL_UP){
			if (y>0)
			{
				y--;
			}			
		} 
		if (ctl.Buttons & PSP_CTRL_DOWN){
			if (y<270)
			{
				y++;
			}			
		} 
		if (ctl.Buttons & PSP_CTRL_LEFT){
			if (x>0)
			{
				x--;
			}			
		} 
		if (ctl.Buttons & PSP_CTRL_RIGHT){
			if (x<480)
			{
				x++;
			}			
		}   
		if (ctl.Buttons & PSP_CTRL_CIRCLE)
		{
			screenshot("ms0:/screenshot.png");
		}		
	}
	sceKernelExitGame();
	return(0);
}
