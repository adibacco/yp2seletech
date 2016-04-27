LDFLAGS = 
CC = $(CROSS_PREFIX)gcc
CXX = $(CROSS_PREFIX)g++
LD = $(CROSS_PREFIX)ld
AR = $(CROSS_PREFIX)ar

OBJECTS   = test.o sEthernet.o sTcpip.o s_utils.o
LIBS = 
INCLUDES = 

test: $(OBJECTS)
	$(CC) -g -O0 $(CFLAGS) $(LIBS) -o test $(OBJECTS)

test.o: test.c
	$(CC) -c -g -O0 $(CFLAGS) $(LIBS) -o test.o test.c

sEthernet.o: sEthernet.c
	$(CC) -c -g -O0 $(CFLAGS) $(LIBS) -o sEthernet.o sEthernet.c

sTcpip.o: sTcpip.c
	$(CC) -c -g -O0 $(CFLAGS) $(LIBS) -o sTcpip.o sTcpip.c

.PHONY:		test


