PROGRAM = cloudapp
OBJS =	./CloudApp.o ./TransitApp.o ../Account.o ../ResponseData.o ../StoredValue.o ../md5.o ../HistoryData.o ../Transfer.o ../Pass.o ../Util.o ../udp.o 

SRCS = ./CloudApp.c ./TransitApp.c ../Account.c ../ResponseData.c ../StoredValue.c ../md5.c ../HistoryData.c ../Transfer.c ../Pass.c ../Util.c ../udp.c 

CC = /Volumes/arm-x/bin/arm-none-linux-gnueabi-gcc -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -mthumb-interwork -mno-thumb --sysroot=/Volumes/arm-sysrootfs/armv7a-arago-linux-gnueabi
TARGET = libCloudApp.so

all: $(TARGET)



$(TARGET):$(OBJS) $(SRCS)
	$(CC) -shared -Wall -fPIC  -o $(TARGET) $(OBJC) $(SRCS) 
	@echo "libCloudApp make success"










