/**
 * @file serialport_win.c
 * 
 * @author  Quincy-Zh (wangqy31@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-18
 * 
 */

#if defined(__WIN32)

#include "serialporth.h"

#include <stdio.h>
#include <windows.h>

#ifdef _DEBUG
#ifndef DBG_PRINTF 
#define DBG_PRINTF printf
#endif
#endif

struct SerialportHandle
{
    HANDLE hComm;
};

sSerialportHandle *serialport_open(const char *port, const sSerialportParam *param)
{
    sSerialportHandle *handle = malloc(sizeof(sSerialportHandle));
    if(handle == NULL)
    {
        return NULL;
    }

    handle->hComm = CreateFile(port,
                       GENERIC_READ | GENERIC_WRITE, //Read/Write
                       0,                            // No Sharing
                       NULL,                         // No Security
                       OPEN_EXISTING,                // Open existing port only
                       0,                            // Non Overlapped I/O
                       NULL);                        // Null for Comm Devices

    if (handle->hComm == INVALID_HANDLE_VALUE)
    {
        free(handle);

        DBG_PRINTF(":: Cannot open serial port \"%s\".\n", port);

        return NULL;
    }
    
    return handle;
}

int serialport_read(sSerialportHandle *handle, char *data, int max_len)
{
    return -1;
}

int serialport_write(sSerialportHandle *handle, const char *data, int len)
{
    return -1;
}

void serialport_flush(sSerialportHandle *handle)
{
    ;
}

void serialport_close(sSerialportHandle *handle)
{
    if(handle)
    {
        CloseHandle(handle->hComm);

        free(handle);
    }
}

#endif

// EOF
