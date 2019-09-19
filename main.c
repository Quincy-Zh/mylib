#include "serialporth.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char * argv[])
{
    char portname[128] = {0};
    int baudrate = 9600;
    int databits = 8;
    int stopbits = 1;
    int parity = 0;
    int flowctrl = 0;

    char *param = 0;
    char *end = 0;

    if(argc < 2)
    {
        fprintf(stderr, "Usage:\n  %s <serial port> [baudrate[,databits[,stopbits[,parity[,flowctrl]]]]].\n", argv[0]);
    
        return 1;
    }

    strncpy(portname, argv[1], sizeof(portname));

    if(argc >= 3)
    {
        param = argv[2];

        baudrate = strtol(param, &end, 10);
        if((strlen(param) != (end - param)) || baudrate < 0)
        {
            fprintf(stderr, ":: 'baudrate' is invalid.\n");

            return 1;
        }

        if(argc >= 4)
        {
            param = argv[3];

            databits = strtol(param, &end, 10);
            if((strlen(param) != (end - param)) || databits < 0)
            {
                fprintf(stderr, ":: 'databits' is invalid.\n");

                return 1;
            }
        
            if(argc >= 5)
            {
                param = argv[4];

                stopbits = strtol(param, &end, 10);
                if((strlen(param) != (end - param)) || databits < 0)
                {
                    fprintf(stderr, ":: 'stopbits' is invalid.\n");

                    return 1;
                }
            
                if(argc >= 6)
                {
                    param = argv[5];

                    parity = strtol(param, &end, 10);
                    if((strlen(param) != (end - param)) || databits < 0)
                    {
                        fprintf(stderr, ":: 'parity' is invalid.\n");

                        return 1;
                    }

                    if(argc >= 7)
                    {
                        param = argv[6];

                        flowctrl = strtol(param, &end, 10);
                        if((strlen(param) != (end - param)) || databits < 0)
                        {
                            fprintf(stderr, ":: 'flowctrl' is invalid.\n");

                            return 1;
                        }
                    }
                }
            }
        }
    }
    
    printf(":: %s %d-%d-%d-%d-%d.\n", portname, baudrate, databits, stopbits, parity, flowctrl);

    sSerialportParam _param;
    _param.baudrate = baudrate;
    _param.databits = databits;
    _param.stopbits = stopbits;
    _param.parity = parity;
    _param.flowctrl = flowctrl;
    
    sSerialportHandle *handle = serialport_open(portname, &_param);

    if(handle != NULL)
    {
        serialport_close(handle);
    }

    return 0;
}

// EOF
