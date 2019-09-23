#include "serialport.h"
#include "plc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void * serialPortOpen(const char *port, const sSerialportParam *param);
static int serialPortRead(void *handle, char *buff, int max_length);
static int serialPortWrite(void *handle, const char *buff, int length);
static void serialPortClose(void *handel);

int main(int argc, char * argv[])
{
    const char cfg_filepath = "cfg_plc_fx.ini";
    uint8_t x_val[5] = {0};

    sPLC plc = {
        cfg_filepath,
        serialPortOpen,
        serialPortClose,
        serialPortRead,
        serialPortWrite,
        NULL
    };

    if(plc_init(&plc, MITSUBISHI_FX))
    {
        printf(":: 初始化PLC失败.\n");

        return 1;
    }

    if(plc_read_bits(&plc, 1, "X0", 5, x_val))
    {
        printf(":: 读取 PLC 失败.\n");
    }
    else
    {
        for(int i = 0 ; i < sizeof(x_val); ++i)
        {
            printf("  [X%d]: %d.\n", i, x_val[i]);
        }
    }
    
    
    plc_deinit(&plc);

    return 0;
}

void * serialPortOpen(const char *port, const sSerialportParam *param)
{
    return serialport_open(port, param);
}

int serialPortRead(void *handle, char *buff, int max_length)
{
    sSerialportHandle *handle = (sSerialportHandle *)handle;

    if(handle == NULL)
    {
        return -1;
    }

    return serialport_read(handle, buff, max_length);
}

int serialPortWrite(void *handle, const char *buff, int length)
{
    sSerialportHandle *handle = (sSerialportHandle *)handle;

    if(handle == NULL)
    {
        return -1;
    }

    return serialport_write(handle, buff, length);
}

void serialPortClose(void *handel)
{
    sSerialportHandle *handle = (sSerialportHandle *)handle;

    if(handle == NULL)
    {
        return;
    }

    serialport_close(handle);
}

// EOF
