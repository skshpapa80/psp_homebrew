TARGET = airplane
OBJS = main.o psp.o hfont/HDrawText.o 

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpng -lz 

EXTRA_TARGETS = EBOOT.PBP 
PSP_EBOOT_TITLE = PSP Airplane
#PSP_EBOOT_ICON = ICON0.PNG 

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 