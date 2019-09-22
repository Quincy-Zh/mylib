#include "serialport.h"
#include "plc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void * serialPortOpen(const char *port, const sSerialParam *param);
static int serialPortRead(void *handle, char *buff, int max_length);
static int serialPortWrite(void *handle, const char *buff, int length);
static void serialPortClose(void *handel);

int main(int argc, char * argv[])
{
    int value = 23;
    char buff[128] = {0};
    int baudrate = 9600;
    int databits = SP_DATA_BITS_8;
    int stopbits = SP_STOP_BITS_1;
    int parity = SP_PARITY_NONE;
    int flowctrl = SP_FLOWCTRL_NONE;

    int i = 0;

    printf(":: Value: %xH, %dD,%oO.\n", value, value, value);
    
    if(argc < 2)
    {
        fprintf(stderr, "Usage:\n  %s <serial port> [baudrate[,databits[,stopbits[,parity[,flowctrl]]]]].\n", argv[0]);
    
        return 1;
    }

    if(argc >= 3)
    {
        i = sscanf(argv[2], "%d,%d,%d,%d,%d", &baudrate, &databits, &stopbits, &parity, &flowctrl);

        if(i == 0)
        {
            fprintf(stderr, ":: Params is invalid.\n");

            return 1;
        }
    }
    
    printf(":: %s %d,%d,%d,%d,%d.\n", argv[1], baudrate, databits, stopbits, parity, flowctrl);

    sSerialportParam _param;
    _param.baudrate = baudrate;
    _param.databits = databits;
    _param.stopbits = stopbits;
    _param.parity = parity;
    _param.flowctrl = flowctrl;
    
    sSerialportHandle *handle = serialport_open(argv[1], &_param);

    if(handle != NULL)
    {
        i = serialport_write(handle, "hello", 5);
        printf(":: Write %d-byte.\n", i);

        i = 0;
        while( i < 10)
        {
            i += serialport_read(handle, buff+i, sizeof(buff)-i);
        }

        printf(":: read %d-byte.\n", i);

        serialport_close(handle);
    }

    return 0;
}

// EOF
