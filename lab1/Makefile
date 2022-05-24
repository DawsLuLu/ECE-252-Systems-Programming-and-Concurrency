CC = gcc       # compiler
CFLAGS = -Wall -g -std=c99 # compilation flags
LD = gcc       # linker
LDFLAGS = -g   # debugging symbols in build
LDLIBS = -lz   # link with libz

# For students 
LIB_UTIL 	   = zutil.o crc.o util.o
SRCS_PNGINFO   = pnginfo.c crc.c util.c zutil.c
SRCS_FINDPNG   = findpng.c crc.c util.c zutil.c
SRCS_CATPNG    = catpng.c crc.c util.c zutil.c
OBJS_PNGINFO   = pnginfo.o ${LIB_UTIL}
OBJS_FINDPNG   = findpng.o ${LIB_UTIL}
OBJS_CATPNG    = catpng.o ${LIB_UTIL}

TARGETS= pnginfo findpng catpng

all: ${TARGETS}

pnginfo: $(OBJS_PNGINFO) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

findpng: $(OBJS_FINDPNG) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

catpng: $(OBJS_CATPNG) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

util.o: util.c 
	$(CC) $(CFLAGS) -c $<

zutil.o: zutil.c 
	$(CC) $(CFLAGS) -c $<

crc.o: crc.c 
	$(CC) $(CFLAGS) -c $<

-include $(SRCS_PNGINFO:.c=.d)
-include $(SRCS_FINDPNG:.c=.d)
-include $(SRCS_CATPNG:.c=.d)

.PHONY: clean
clean:
	rm -f $(LIB_UTIL) $(TARGETS) pnginfo.o findpng.o catpng.o
